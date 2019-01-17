#include "MusicSegment.h"

using namespace music_segment;

MusicSegment::MusicSegment(MidiFile* prep, MidiFile* mainLoop, MidiFile* mainLoopEnd, MidiFile* finalEnd) {
	this->prep = prep;
	this->mainLoop = mainLoop;
	this->mainLoopEnd = mainLoopEnd;
	this->finalEnd = finalEnd;
}

/*
MidiFile MusicSegment::repeat(double timeInSeconds, bool isAbsoluteStart, bool isAbsoluteEnd) {
	MidiCat midiCat;
	
	
}*/