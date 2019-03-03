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
	
	this->barErosionIntoPrevSeg = 5;
	this->barErosionIntoNextSeg = 3;
	
	MidiFile prevMidi = getLastBarsFromSegment(prevSegment, barErosionIntoPrevSeg);
	MidiFile nextMidi = getFirstBarsFromSegment(nextSegment, barErosionIntoNextSeg);
	
	if(getLastKeySignature(prevMidi).isEmpty() || getFirstKeySignature(nextMidi).isEmpty()){
		this->valid = false;
		return;
	}

	int prevKeySig = getLastKeySignature(prevMidi)[3];
	int nextKeySig = getFirstKeySignature(nextMidi)[3];

	if((prevKeySig > 7 && nextKeySig <= 7) || (prevKeySig <= 7 && nextKeySig > 7) ) {
		this->valid = false;
		return;
	}
	
	prevMidi = tempoDilation(prevMidi, findFirstTempo(nextMidi));
	prevMidi = transpose(prevMidi, -1);
	
	vector<int> endNoteKeys = getEndNoteKeys(prevMidi);
	for(int i=0; i< endNoteKeys.size(); i++){
		std::cout << "endNoteKeys[" << i << "] = " << endNoteKeys[i] << "\n";
	}
	
	vector<int> beginningNoteKeys = getBeginningNoteKeys(nextMidi);
	for(int i=0; i< beginningNoteKeys.size(); i++){
		std::cout << "beginningNoteKeys[" << i << "] = " << beginningNoteKeys[i] << "\n";
	}
	
	vector<MidiFile> catList;
	catList.push_back(prevMidi);
	catList.push_back(nextMidi);
	newMidi = midiCat.run(catList, 0.0);
	
	this->bridgeMidi = newMidi;
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

MidiFile Bridge::getFirstBarsFromSegment(MusicSegment inputSegment, int bars) {
	MidiFile output;
	MidiCat midiCat;
	vector<MidiFile> catList;
	MidiExcerptByBar midiExcerptByBar;
	
	if(inputSegment.prep) {
		int prepBars = inputSegment.prep->getTotalBars();
		if(prepBars <= bars) {
			catList.push_back(*(inputSegment.prep));
			bars -= prepBars;
		}
		else {
			catList.push_back(midiExcerptByBar.run(1, bars, *(inputSegment.prep)));
			bars = 0;
		}
	}
	
	while(bars > 0) {
		int mainLoopBars = inputSegment.mainLoop->getTotalBars();
		if(mainLoopBars <= bars) {
			catList.push_back(*(inputSegment.mainLoop));
			bars -= mainLoopBars;
			if(bars > 0 && inputSegment.mainLoopEnd) {
				int mainLoopEndBars = inputSegment.mainLoopEnd->getTotalBars();
				if(mainLoopEndBars <= bars) {
					catList.push_back(*(inputSegment.mainLoopEnd));
					bars -= mainLoopEndBars;
				}
				else {
					catList.push_back(midiExcerptByBar.run(1, bars, *(inputSegment.mainLoopEnd)));
					bars = 0;
				}
			}
		}
		else {
			catList.push_back(midiExcerptByBar.run(1, bars, *(inputSegment.mainLoop)));
			bars = 0;
		}
	}
	
	output = midiCat.run(catList, 0.0);
	return output;
	
}

MidiFile Bridge::getLastBarsFromSegment(MusicSegment inputSegment, int bars) {
	MidiFile output;
	MidiCat midiCat;
	vector<MidiFile> catList;
	MidiExcerptByBar midiExcerptByBar;
	
	if(inputSegment.finalEnd) {
		int finalEndBars = inputSegment.finalEnd->getTotalBars();
		if(finalEndBars <= bars) {
			catList.push_back(*(inputSegment.finalEnd));
			bars -= finalEndBars;
		}
		else {
			catList.push_back(midiExcerptByBar.run(finalEndBars - bars + 1, finalEndBars, *(inputSegment.finalEnd)));
			bars = 0;
			
		}
	}
	
	if(bars > 0) {
		int mainLoopBars = inputSegment.mainLoop->getTotalBars();
		if(mainLoopBars <= bars) {
			catList.insert(catList.begin(), *(inputSegment.mainLoop));
			bars -= mainLoopBars;
			
		}
		else {
			catList.insert(catList.begin(), midiExcerptByBar.run(mainLoopBars - bars + 1, mainLoopBars, *(inputSegment.mainLoop)));
			bars = 0;
		}
	}
	
	while(bars > 0) {
		if(inputSegment.mainLoopEnd) {
			int mainLoopEndBars = inputSegment.mainLoopEnd->getTotalBars();
			if(mainLoopEndBars <= bars) {
				catList.insert(catList.begin(), *(inputSegment.mainLoopEnd));
				bars -= mainLoopEndBars;
			}
			else {
				catList.insert(catList.begin(), midiExcerptByBar.run(mainLoopEndBars - bars + 1, mainLoopEndBars, *(inputSegment.mainLoopEnd)));
				bars = 0;
			}
		}
		
		if(bars <= 0) break;
		
		int mainLoopBars = inputSegment.mainLoop->getTotalBars();
		if(mainLoopBars <= bars) {
			catList.insert(catList.begin(), *(inputSegment.mainLoop));
			bars -= mainLoopBars;
		}
		else {
			catList.insert(catList.begin(), midiExcerptByBar.run(mainLoopBars - bars + 1, mainLoopBars, *(inputSegment.mainLoop)));
			bars = 0;
		}
	}

	output = midiCat.run(catList, 0.0);
	return output;
	
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


MidiEvent Bridge::getLastKeySignature(MidiFile inputFile) {
	inputFile.joinTracks();
	int eventCount = inputFile.getEventCount(0);
	MidiEvent keySignature;
	
	for(int i=0; i<eventCount; i++) {
		if(inputFile.getEvent(0, i).isKeySignature()){
			keySignature = inputFile.getEvent(0, i);
		}
	}
	
	return keySignature;
}

MidiEvent Bridge::getFirstKeySignature(MidiFile inputFile) {
	inputFile.joinTracks();
	int eventCount = inputFile.getEventCount(0);

	for(int i=0; i<eventCount; i++) {
		if(inputFile.getEvent(0, i).isKeySignature()){
			return inputFile.getEvent(0, i);
		}
	}
	
	return MidiEvent();
}

MidiFile Bridge::transpose(MidiFile inputFile, int keyChange) {
	inputFile.joinTracks();
	int eventCount = inputFile.getEventCount(0);

	for(int i=0; i<eventCount; i++) {
		if(inputFile.getEvent(0, i).isNoteOn() || inputFile.getEvent(0, i).isNoteOff()){
			int originalKey = inputFile.getEvent(0, i).getKeyNumber();
			int newKey = originalKey + keyChange;
			inputFile.getEvent(0, i).setKeyNumber(newKey);
		}
	}
	inputFile.linkNotePairs();
	
	return inputFile;
}

vector<int> Bridge::getEndNoteKeys(MidiFile inputFile) {
	inputFile.joinTracks();
	inputFile.makeAbsoluteTicks();
	int eventCount = inputFile.getEventCount(0);
	vector<int> endNoteKeys;
	int currentTick = 0;

	for(int i=0; i<eventCount; i++) {
		if(inputFile.getEvent(0, i).isNoteOn() || inputFile.getEvent(0, i).isNoteOff()){
			if(inputFile.getEvent(0, i).tick > currentTick) {
				endNoteKeys.clear();
				endNoteKeys.push_back(inputFile.getEvent(0, i).getKeyNumber());
				currentTick = inputFile.getEvent(0, i).tick;
			}
			else if(inputFile.getEvent(0, i).tick == currentTick) {
				endNoteKeys.push_back(inputFile.getEvent(0, i).getKeyNumber());
			}
		}
	}

	return endNoteKeys;
}

vector<int> Bridge::getBeginningNoteKeys(MidiFile inputFile) {
	inputFile.joinTracks();
	inputFile.makeAbsoluteTicks();
	int eventCount = inputFile.getEventCount(0);
	vector<int> beginningNoteKeys;
	int currentTick = 0;

	for(int i=0; i<eventCount; i++) {
		if(inputFile.getEvent(0, i).isNoteOn() || inputFile.getEvent(0, i).isNoteOff()){
			if(inputFile.getEvent(0, i).tick > currentTick && !beginningNoteKeys.empty()) {
				return beginningNoteKeys;
			}
			beginningNoteKeys.push_back(inputFile.getEvent(0, i).getKeyNumber());
			currentTick = inputFile.getEvent(0, i).tick;
		}
	}

	return beginningNoteKeys;
}