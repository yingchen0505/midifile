#include "MusicSegment.h"

using namespace music_segment;

MusicSegment::MusicSegment(MidiFile* prepBar, MidiFile* mainLoop, MidiFile* mainLoopEndBar, MidiFile* finalEndBar) {
	this->prepBar = prepBar;
	this->mainLoop = mainLoop;
	this->mainLoopEndBar = mainLoopEndBar;
	this->finalEndBar = finalEndBar;
}