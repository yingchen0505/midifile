#include "MusicSegment.h"

using namespace music_segment;

MusicSegment::MusicSegment() {
	valence = 0;
	arousal = 0;
	prep = NULL;
	mainLoop = NULL;
	mainLoopEnd = NULL;
	finalEnd = NULL;
}

MusicSegment::MusicSegment(int valence, int arousal, MidiFile* prep, MidiFile* mainLoop, MidiFile* mainLoopEnd, MidiFile* finalEnd) {
	this->valence = valence;
	this->arousal = arousal;
	this->prep = prep;
	this->mainLoop = mainLoop;
	this->mainLoopEnd = mainLoopEnd;
	this->finalEnd = finalEnd;
}

bool MusicSegment::isInvalid() {
	return !mainLoop;
}

MidiFile MusicSegment::repeat(double timeInSeconds, bool isAbsoluteStart, bool isAbsoluteEnd) {
	if(!mainLoop) return MidiFile(); // No mainLoop to repeat, something went wrong.
	
	double durationOfPrepAndEnd = 0.0; 
	MidiCat midiCat;
	vector<MidiFile> concatList; // list of files waiting to be concatenated into output
	
	if(isAbsoluteStart && prep) {
		concatList.push_back(*prep);
		durationOfPrepAndEnd += (*prep).getFileDurationInSeconds();
		//cout << "(*prep).getFileDurationInSeconds() = " << (*prep).getFileDurationInSeconds() << "\n";
	
	}

	if(isAbsoluteEnd && finalEnd) {
		// Account for the duration first, but only add finalEnd to the list after mainLoop is added.
		durationOfPrepAndEnd += (*finalEnd).getFileDurationInSeconds(); 
		//cout << "(*finalEnd).getFileDurationInSeconds() = " << (*finalEnd).getFileDurationInSeconds() << "\n";
	
	}

	double mainLoopDuration = (*mainLoop).getFileDurationInSeconds();
	
	// If mainLoopEnd is not null, it should be repeated together with mainLoop
	if(mainLoopEnd) {
		mainLoopDuration += (*mainLoopEnd).getFileDurationInSeconds();
	}

	// Calculate how many times the mainLoop (and mainLoopEnd if exists) should be repeated
	int repeatRounds = round((timeInSeconds - durationOfPrepAndEnd) / mainLoopDuration);
	/*cout << "timeInSeconds = " << timeInSeconds << "\n";
	cout << "durationOfPrepAndEnd = " << durationOfPrepAndEnd << "\n";
	cout << "mainLoopDuration = " << mainLoopDuration << "\n";
	
	cout << "calculated repeat rounds = " << repeatRounds << "\n";*/
	// Need to play at least once, even if time given is too short
	repeatRounds = max(repeatRounds, 1);
	//cout << "final repeat rounds = " << repeatRounds << "\n";
	
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
			//cout << "blurp \n";
			concatList.pop_back();
			//concatList.pop_back();
		}
		concatList.push_back(*finalEnd);
	}
	
	return midiCat.run(concatList, 0.0);
}