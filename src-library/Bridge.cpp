#include "Bridge.h"

using namespace bridge;

Bridge::Bridge(MusicSegment prevSegment, MusicSegment nextSegment) {
	if(prevSegment.isInvalid() || nextSegment.isInvalid()){
		std::cout << "Invalid input to bridge constructor! \n";
		this->valid = false;
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
	prevMidi = tempoDilation(prevMidi, 0);
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

MidiFile Bridge::tempoDilation(MidiFile inputFile, int finalTempo) {
	inputFile.joinTracks();
	int eventCount = inputFile.getEventCount(0);
	for(int i=0; i < eventCount; i++) {
		if(inputFile.getEvent(0, i).isTempo()) {
			int originalTempo = inputFile.getEvent(0, i).getTempoBPM();
			inputFile.getEvent(0, i).setTempo(originalTempo/2);
		}
	}
	return inputFile;
}