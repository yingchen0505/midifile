#include "Bridge.h"

using namespace bridge;

Bridge::Bridge(MusicSegment prevSegment, MusicSegment nextSegment) {
	if(prevSegment.isInvalid() || nextSegment.isInvalid()){
		std::cout << "Invalid input to bridge constructor! \n";
		return;
	}
	MidiFile newMidi;
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
	if(nextSegment.finalEnd) {
		nextMidi = *(nextSegment.finalEnd);
	}
	else if (nextSegment.mainLoopEnd) {
		nextMidi = *(nextSegment.mainLoopEnd);
	}
	else {
		nextMidi = *(nextSegment.mainLoop);
	}
	
	prevMidi = midiExcerptByBar.run(max(1, prevMidi.getTotalBars() - 1), prevMidi.getTotalBars(), prevMidi);
	nextMidi = midiExcerptByBar.run(1, min(nextMidi.getTotalBars(), 2), nextMidi);
	vector<MidiFile> catList;
	catList.push_back(prevMidi);
	catList.push_back(nextMidi);
	newMidi = midiCat.run(catList, 0.0);
	
	this->bridgeMidi = newMidi;
	this->barErosionIntoPrevSeg = 0;
	this->barErosionIntoNextSeg = 0;
	this->valid = true;
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