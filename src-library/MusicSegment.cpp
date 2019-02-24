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

MidiFile MusicSegment::repeat(double timeInSeconds, bool isAbsoluteStart, bool isAbsoluteEnd, int numberOfEndBarsToDrop) {
	if(!mainLoop) return MidiFile(); // No mainLoop to repeat, something went wrong.
	
	double durationOfPrepAndEnd = 0.0; 
	MidiCat midiCat;
	vector<MidiFile> concatList; // list of files waiting to be concatenated into output
	
	if(isAbsoluteStart && prep) {
		concatList.push_back(*prep);
		durationOfPrepAndEnd += (*prep).getFileDurationInSeconds();
	}

	if(isAbsoluteEnd && finalEnd) {
		// Account for the duration first, but only add finalEnd to the list after mainLoop is added.
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
	
	if(isAbsoluteEnd && finalEnd) {
		// If there is mainLoopEnd, 
		// need to replace the last one of it with the finalEnd
		if(mainLoopEnd) {
			concatList.pop_back();
		}
		concatList.push_back(*finalEnd);
	}
	
	while(numberOfEndBarsToDrop > 0 && !(concatList.empty())) {
		MidiFile lastMidi = concatList.back();
		
		std::ofstream outfile1; // without std::, reference would be ambiguous because of Boost
		outfile1.open((to_string(valence) + to_string(arousal) + to_string(ID) + "_lastMidi_orig.mid").c_str());
		lastMidi.write(outfile1);
		outfile1.close();
		
		std::ofstream outfile1txt; // without std::, reference would be ambiguous because of Boost
		outfile1txt.open((to_string(valence) + to_string(arousal) + to_string(ID) + "_lastMidi_orig.txt").c_str());
		outfile1txt << lastMidi;
		outfile1txt.close();
		
		int totalBars = lastMidi.getTotalBars();
		if(totalBars > numberOfEndBarsToDrop) {
			int newEndBar = totalBars - numberOfEndBarsToDrop;
			MidiExcerptByBar midiExcerptByBar;
			lastMidi = midiExcerptByBar.run(1, newEndBar, lastMidi);
			concatList.pop_back();
			concatList.push_back(lastMidi);
			
			break; // All required bars have been dropped
		}
		
		else {
			concatList.pop_back();
			numberOfEndBarsToDrop = numberOfEndBarsToDrop - totalBars;
		}
		
		
/*
		if(newEndBar >= 1) {
			lastMidi = midiExcerptByBar.run(1, newEndBar, lastMidi);
			concatList.pop_back();
			concatList.push_back(lastMidi);
			
			std::ofstream outfile; // without std::, reference would be ambiguous because of Boost
			outfile.open((to_string(valence) + to_string(arousal) + to_string(ID) + "_lastMidi.mid").c_str());
			lastMidi.write(outfile);
			outfile.close();
			
			std::ofstream outfiletxt; // without std::, reference would be ambiguous because of Boost
			outfiletxt.open((to_string(valence) + to_string(arousal) + to_string(ID) + "_lastMidi.txt").c_str());
			outfiletxt << lastMidi;
			outfiletxt.close();
			
			numberOfEndBarsToDrop = 0; // All required bars have been dropped
		}
		else {
			concatList.pop_back();
			numberOfEndBarsToDrop = numberOfEndBarsToDrop - totalBars;
		}*/
	}
	
	return midiCat.run(concatList, 0.0);
}