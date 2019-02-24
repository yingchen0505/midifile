#include "MusicSegment.h"

using namespace music_segment;

MusicSegment::MusicSegment() {
	valence = 0;
	arousal = 0;
	ID = 0;
	prep = NULL;
	mainLoop = NULL;
	mainLoopEnd = NULL;
	finalEnd = NULL;
}

MusicSegment::MusicSegment(int valence, int arousal, int ID, MidiFile* prep, MidiFile* mainLoop, MidiFile* mainLoopEnd, MidiFile* finalEnd) {
	this->valence = valence;
	this->arousal = arousal;
	this->ID = ID;
	this->prep = prep;
	this->mainLoop = mainLoop;
	this->mainLoopEnd = mainLoopEnd;
	this->finalEnd = finalEnd;
}

bool MusicSegment::isInvalid() {
	return !mainLoop;
}

MidiFile MusicSegment::repeat(double timeInSeconds, int beginningBarErosion, int endBarErosion) {
	if(!mainLoop) return MidiFile(); // No mainLoop to repeat, something went wrong.
	
	double durationOfPrepAndEnd = 0.0; 
	MidiCat midiCat;
	vector<MidiFile> concatList; // list of files waiting to be concatenated into output

	if(prep) {
		concatList.push_back(*prep);
		durationOfPrepAndEnd += (*prep).getFileDurationInSeconds();
	}
	
	if(finalEnd) {
		durationOfPrepAndEnd += (*finalEnd).getFileDurationInSeconds();
	}
	
	double mainLoopDuration = (*mainLoop).getFileDurationInSeconds();
	
	// If mainLoopEnd is not null, it should be repeated together with mainLoop
	if(mainLoopEnd) {
		mainLoopDuration += (*mainLoopEnd).getFileDurationInSeconds();
	}

	// Calculate how many times the mainLoop (and mainLoopEnd if exists) should be repeated
	int repeatRounds = round((timeInSeconds - durationOfPrepAndEnd) / mainLoopDuration);

	// Need to play at least once, even if time given is too short
	repeatRounds = max(repeatRounds, 1);

	// If mainLoopEnd is not null, it should be repeated together with mainLoop
	if(mainLoopEnd) {
		for (int i=0; i< repeatRounds; i++){
			concatList.push_back(*mainLoop);
			concatList.push_back(*mainLoopEnd);
		}
	}
	else {
		for (int i=0; i< repeatRounds; i++){
			concatList.push_back(*mainLoop);
		}
	}
	
	if(finalEnd) {
		if(mainLoopEnd) {
			concatList.pop_back();
		}
		concatList.push_back(*finalEnd);
	}
	
	while(endBarErosion > 0 && !(concatList.empty())) {
		MidiFile lastMidi = concatList.back();

		int totalBars = lastMidi.getTotalBars();
		if(totalBars > endBarErosion) {
			int newEndBar = totalBars - endBarErosion;
			MidiExcerptByBar midiExcerptByBar;
			lastMidi = midiExcerptByBar.run(1, newEndBar, lastMidi);
			concatList.pop_back();
			concatList.push_back(lastMidi);

			break; // All required bars have been dropped
		}
		
		else {
			concatList.pop_back();
			endBarErosion = endBarErosion - totalBars;
		}
	}
	
	while(beginningBarErosion > 0 && !(concatList.empty())) {
		MidiFile firstMidi = concatList.front();

		int totalBars = firstMidi.getTotalBars();
		if(totalBars > beginningBarErosion) {
			int newBeginningBar = beginningBarErosion + 1;
			MidiExcerptByBar midiExcerptByBar;
			firstMidi = midiExcerptByBar.run(newBeginningBar, totalBars, firstMidi);
			concatList.erase(concatList.begin());
			concatList.insert(concatList.begin(), firstMidi);

			break; // All required bars have been dropped
		}
		
		else {
			concatList.erase(concatList.begin());
			beginningBarErosion = beginningBarErosion - totalBars;
		}
	}
	
	return midiCat.run(concatList, 0.0);
}