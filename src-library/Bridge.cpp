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
	
	MidiFile prevMidi = getLastBarsFromSegment(prevSegment, 1);
	MidiFile nextMidi = getFirstBarsFromSegment(nextSegment, 1);
	
	this->barErosionIntoPrevSeg = getPhraseLengthInBars(prevMidi);
	this->barErosionIntoNextSeg = getPhraseLengthInBars(nextMidi);
	
	prevMidi = getLastBarsFromSegment(prevSegment, barErosionIntoPrevSeg);
	nextMidi = getFirstBarsFromSegment(nextSegment, barErosionIntoNextSeg);
	/*
	std::ofstream outfile; // without std::, reference would be ambiguous because of Boost
	outfile.open(to_string(prevSegment.valence) + to_string(prevSegment.arousal) + to_string(prevSegment.ID) + "helplah.mid");
	prevMidi.write(outfile);
	outfile.close();
	std::ofstream outfiletxt; // without std::, reference would be ambiguous because of Boost
	outfiletxt.open(to_string(prevSegment.valence) + to_string(prevSegment.arousal) + to_string(prevSegment.ID) + "helplah.txt");
	outfiletxt << prevMidi;
	outfiletxt.close();*/
	
	if(!(getLastKeySignature(prevMidi).isEmpty()) && !(getFirstKeySignature(nextMidi).isEmpty())){
		int prevKeySig = getLastKeySignature(prevMidi)[3];
		int nextKeySig = getFirstKeySignature(nextMidi)[3];

		if((prevKeySig > 7 && nextKeySig <= 7) || (prevKeySig <= 7 && nextKeySig > 7) ) {
			this->valid = false;
			return;
		}
	}

	vector<int> endNoteKeys = getEndNoteKeys(prevMidi);
	for(int i=0; i< endNoteKeys.size(); i++){
		std::cout << "endNoteKeys[" << i << "] = " << endNoteKeys[i] << "\n";
	}
	
	vector<int> beginningNoteKeys = getBeginningNoteKeys(nextMidi);
	for(int i=0; i< beginningNoteKeys.size(); i++){
		std::cout << "beginningNoteKeys[" << i << "] = " << beginningNoteKeys[i] << "\n";
	}
	
	int maxBeginningKey = 0;
	int maxEndKey = 0;
	int keyChange = *max_element(begin(beginningNoteKeys), end(beginningNoteKeys)) - *max_element(begin(endNoteKeys), end(endNoteKeys));
	keyChange = keyChange%12;

	std::cout << "keyChange = " << keyChange << "\n";
	double tempoOfNext = findFirstTempo(nextMidi);
	//prevMidi = tempoDilation(prevMidi, findFirstTempo(nextMidi));

	vector<MidiFile> catList;
	vector<MidiFile> keyChangeCatList;

	//catList.push_back(prevMidi);
	
	MidiFile keyChangeBar = midiExcerptByBar.run(1, 1, prevMidi);
	keyChangeCatList.push_back(keyChangeBar);
	int currentKeyChange = 0;
	int greatestPrime = 2;
	/*
	if(abs(keyChange) >= 7) {
		greatestPrime = 7;
	}*/
	if (abs(keyChange) >= 5) {
		greatestPrime = 5;
	}
	else if (abs(keyChange) >= 3) {
		greatestPrime = 3;
	}
	std::cout << "greatestPrime = " << greatestPrime << "\n";
	
	while (keyChange != 0) {
		int keyChangeStep = keyChange > 0 ? min(greatestPrime, keyChange) : max((-1)*greatestPrime, keyChange);
		std::cout << "keyChangeStep = " << keyChangeStep << "\n";

		currentKeyChange += keyChangeStep;
		keyChange -= keyChangeStep;
		if(keyChange == 0) {
			keyChangeCatList.push_back(transpose(prevMidi, currentKeyChange));
		}
		else {
			keyChangeCatList.push_back(transpose(keyChangeBar, currentKeyChange));
		}
	}
	
	MidiFile prevMidiAfterKeyChange = midiCat.run(keyChangeCatList, 0.0);
	//prevMidiAfterKeyChange = tempoDilation(prevMidiAfterKeyChange, findFirstTempo(nextMidi));
	prevMidiAfterKeyChange = tempoDilation(prevMidiAfterKeyChange, 30);
	catList.push_back(prevMidiAfterKeyChange);
	
	//MidiFile frontOfNextMidi = midiExcerptByBar(1, )
	nextMidi = reverseTempoDilation(nextMidi, 30);
	
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

MidiFile Bridge::reverseTempoDilation(MidiFile inputFile, double initialTempo) {
	inputFile.joinTracks();
	inputFile.absoluteTicks();
	int totalTicks = inputFile.getFileDurationInTicks();
	int eventCount = inputFile.getEventCount(0);
	double progression = 0.0;
	vector<MidiEvent> tempoList;
	MidiEvent lastOriginalTempo;
	
	// This loop adjusts the BPM values of the tempo events in the midi file
	// so that the new BPM values are results of linear interpolation
	// between the initialTempo and the original tempos
	for(int i=0; i < eventCount; i++) {
		progression = (double) inputFile.getEvent(0, i).tick / (double) totalTicks;
		
		if(inputFile.getEvent(0, i).isTempo()) {
			double originalTempo = inputFile.getEvent(0, i).getTempoBPM();
			double newTempo = initialTempo + (originalTempo - initialTempo) * progression;
			lastOriginalTempo = inputFile.getEvent(0, i);
			inputFile.getEvent(0, i).setTempo(newTempo);
			tempoList.push_back(inputFile.getEvent(0, i));
		}
	}
	
	lastOriginalTempo.tick = totalTicks;
	tempoList.push_back(lastOriginalTempo);
	
	int currentTick = 0;
	int lastTick = 0;
	int nextTick = 0;
	const double stepsize = 1.0 / 10.0;
	double stepCounter = 0.0;
	progression = 0.0;
	double lastTempo = initialTempo;
	double nextTempo = 120.0; // this is the midi default tempo if no tempo marking is found
	
	// This loop performs linear interpolation for events in-between the tempo events
	for(int i=0; i < eventCount; i++) {
		currentTick = inputFile.getEvent(0, i).tick;
		double oldProgression = progression;
		progression = (double) currentTick / (double) totalTicks;
		stepCounter += progression - oldProgression;
		
		if(!tempoList.empty()) {
			if(currentTick > tempoList.front().tick) {
				lastTick = tempoList.front().tick;
				lastTempo = tempoList.front().getTempoBPM();
				tempoList.erase(tempoList.begin());
			}
		}
		
		if(!(inputFile.getEvent(0, i).isTempo()) && stepCounter >= stepsize) {

			if(!tempoList.empty()) {
				nextTempo = tempoList.front().getTempoBPM();
				nextTick = tempoList.front().tick;
			}
			else {
				nextTick = totalTicks;
			}
			
			double progressionBetweenTempos = (double) (currentTick - lastTick) / (double) (nextTick - lastTick);
			double newTempo = lastTempo + (nextTempo - lastTempo) * progressionBetweenTempos;
			MidiEvent tempoEvent;
			tempoEvent.makeTempo(newTempo);
			tempoEvent.tick = inputFile.getEvent(0, i).tick;
			inputFile.addEvent(tempoEvent);
			while(stepCounter >= stepsize) stepCounter -= stepsize;
		}
		
	}
	inputFile.sortTracks();
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

int Bridge::getPhraseLengthInBars(MidiFile inputFile) {
	inputFile.joinTracks();
	int eventCount = inputFile.getEventCount(0);
	int phraseLength;

	for(int i=0; i<eventCount; i++) {
		if(inputFile.getEvent(0, i).isTimeSignature()){
			int numerator = inputFile.getEvent(0, i)[3];
			int denom = inputFile.getEvent(0, i)[4];
			denom = pow(2, denom);
			if(denom%2 == 0) {
				phraseLength = 2;
			}
			else if(denom%3 == 0) {
				phraseLength = 3;
			}
		}
	}
	return phraseLength;
}