#include "MusicSegment.h"

using namespace music_segment;

MusicSegment::MusicSegment(MidiFile* prep, MidiFile* mainLoop, MidiFile* mainLoopEnd, MidiFile* finalEnd) {
	this->prep = prep;
	this->mainLoop = mainLoop;
	this->mainLoopEnd = mainLoopEnd;
	this->finalEnd = finalEnd;
}


MidiFile MusicSegment::repeat(double timeInSeconds, bool isAbsoluteStart, bool isAbsoluteEnd) {
	//if(!mainLoop) return NULL; // No mainLoop to repeat, something went wrong.
	
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
	if(repeatRounds == 0) {
		repeatRounds++;
	}
	
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
		concatList.push_back(*finalEnd);
	}
	
	return midiCat.run(concatList, 0.0);
}