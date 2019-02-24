#include "Bridge.h"

using namespace bridge;

Bridge::Bridge(MusicSegment prevSegment, MusicSegment nextSegment) {
	if(prevSegment.isInvalid() || nextSegment.isInvalid()){
		std::cout << "Invalid input to bridge constructor! \n";
		return;
	}
	MidiFile newMidi;
	//newMidi.setTPQ(nextSegment.mainLoop->getTPQ());
	MidiExcerptByBar midiExcerptByBar;
	MidiCat midiCat;
	MidiFile prevMidi;
	if(prevSegment.finalEnd) {
		prevMidi = *(prevSegment.finalEnd);
	}
	else if (prevSegment.mainLoopEnd) {
		prevMidi = *(prevSegment.mainLoopEnd);
	}
	else {
		prevMidi = *(prevSegment.mainLoop);
	}
	MidiFile nextMidi;
	//newMidi = midiCat.run(prevSegment
	
	this->bridgeMidi = newMidi;
	this->valid = false;
}

Bridge::Bridge(string ID, MidiFile bridgeMidi, int barErosionIntoPrevSeg, int barErosionIntoNextSeg) {
	this->ID = ID;
	this->bridgeMidi = bridgeMidi;
	this->valid = true;
	this->barErosionIntoPrevSeg = barErosionIntoPrevSeg;
	this->barErosionIntoNextSeg = barErosionIntoNextSeg;
}

bool Bridge::isInvalid() {
	return !valid;
}