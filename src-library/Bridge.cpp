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
	if(nextSegment.prep) {
		nextMidi = *(nextSegment.prep);
	}
	else {
		nextMidi = *(nextSegment.mainLoop);
	}
	
	prevMidi = midiExcerptByBar.run(max(1, prevMidi.getTotalBars() - 1), prevMidi.getTotalBars(), prevMidi);
	prevMidi = tempoDilation(prevMidi, findFirstTempo(nextMidi));
	nextMidi = midiExcerptByBar.run(1, min(nextMidi.getTotalBars(), 2), nextMidi);
	vector<MidiFile> catList;
	catList.push_back(prevMidi);
	catList.push_back(nextMidi);
	newMidi = midiCat.run(catList, 0.0);
	
	this->bridgeMidi = newMidi;
	this->barErosionIntoPrevSeg = min(prevMidi.getTotalBars(), 2);
	this->barErosionIntoNextSeg = min(nextMidi.getTotalBars(), 2);
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

MidiFile Bridge::tempoDilation(MidiFile inputFile, double finalTempo) {
	inputFile.joinTracks();
	inputFile.deltaTicks();
	int eventCount = inputFile.getEventCount(0);
	double currentTempo = 120.0; // this is the midi default tempo if no tempo marking is found
	double slowDownFactor = 1.0;	// default no tempo dilation
	for(int i=0; i < eventCount; i++) {
		double progression = (double)i/(double)eventCount;
		if(inputFile.getEvent(0, i).isTempo()) {
			double originalTempo = inputFile.getEvent(0, i).getTempoBPM();
			double newTempo = originalTempo - (originalTempo - finalTempo) * progression;
			currentTempo = newTempo;
			inputFile.getEvent(0, i).setTempo(newTempo);
		}
		else if (inputFile.getEvent(0, i).tick != 0) {
			double newTempo = currentTempo - (currentTempo - finalTempo) * progression;
			slowDownFactor = newTempo/currentTempo;
			inputFile.getEvent(0, i).tick /= slowDownFactor;
		}
	}
	return inputFile;
}

double Bridge::findFirstTempo(MidiFile inputFile) {
	inputFile.joinTracks();
	int eventCount = inputFile.getEventCount(0);
	for(int i=0; i<eventCount; i++) {
		if(inputFile.getEvent(0, i).isTempo()) {
			return inputFile.getEvent(0, i).getTempoBPM();
		}
	}
	return 120.0; // default tempo
}