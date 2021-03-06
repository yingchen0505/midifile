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
		
	if(prevSegment.currTransposition != 0) {
		prevMidi = transpose(prevMidi, prevSegment.currTransposition);
	}

	vector<int> endNoteKeys = getEndNoteKeys(prevMidi);
	
	vector<int> beginningNoteKeys = getBeginningNoteKeys(nextMidi);
	
	int maxBeginningKey = 0;
	int maxEndKey = 0;
	int keyChange = *max_element(begin(beginningNoteKeys), end(beginningNoteKeys)) - *max_element(begin(endNoteKeys), end(endNoteKeys));
	keyChange = keyChange%12;

	double tempoOfNext = findFirstTempo(nextMidi);

	vector<MidiFile> catList;
	vector<MidiFile> keyChangeCatList;

	MidiFile keyChangeBar = midiExcerptByBar.run(1, max(1, prevMidi.getTotalBars() - 1), prevMidi);
	keyChangeCatList.push_back(keyChangeBar);
	
	vector<int> begNotesOfKeyChangeBar = getBeginningNoteKeys(keyChangeBar);
	
	vector<int> endNotesOfKeyChangeBar = getEndNoteKeys(keyChangeBar);
	
	MidiFile finalBarOfPrev = midiExcerptByBar.run(prevMidi.getTotalBars(), prevMidi.getTotalBars(), prevMidi);
	
	vector<int> begNotesOfFinalBarOfPrev = getBeginningNoteKeys(finalBarOfPrev);
	int magicNumber = *max_element(begin(begNotesOfFinalBarOfPrev), end(begNotesOfFinalBarOfPrev)) - 
						*max_element(begin(begNotesOfKeyChangeBar), end(begNotesOfKeyChangeBar));
	
	/////////// Magic set please save me
	///----------------------------------------------------------------------------
	vector<int> magicSet;
	vector<int> stepSet;
	int startPoint = *max_element(begin(begNotesOfKeyChangeBar), end(begNotesOfKeyChangeBar));
	for(int begNote : begNotesOfFinalBarOfPrev) {
		int step = begNote - startPoint;
		stepSet.push_back( abs(step) < 12 ? step : 0);

	}
	
	///// Remove duplicates
	sort( stepSet.begin(), stepSet.end() );
	stepSet.erase( unique( stepSet.begin(), stepSet.end() ), stepSet.end() );

	int* array = &stepSet[0];
	vector<vector<int>> solutions = countSolutions(array, stepSet.size(), keyChange);
	int nextTransposition = 0;
	bool nextTranspositionFailed = false;
	while(solutions.empty() && nextTransposition != -keyChange) {
		// explore in the sequence of -1, 1, -2, 2, -3, 3...
		nextTransposition = nextTransposition >= 0 ? (nextTransposition + 1) * (-1) : nextTransposition * (-1);
		solutions = countSolutions(array, stepSet.size(), keyChange + nextTransposition);
		if(abs(nextTransposition) > 11) {
			nextTranspositionFailed = true;
			break;
		}
	}
	
	keyChange += !nextTranspositionFailed ? nextTransposition : 0;
	if(!solutions.empty()) {
		int shortestSolutionLength = INT8_MAX;
		for(vector<int> solution : solutions) {
			if(solution.size() < shortestSolutionLength) {
				shortestSolutionLength = solution.size();
				magicSet = solution;
			}
		}
	}
	
	///----------------------------------------------------------------------------

	int currentKeyChange = 0;
	
	if(!magicSet.empty()) {
		int remainingKeyChange = keyChange;
		for(int magic : magicSet) {
			currentKeyChange += magic;
			remainingKeyChange -= magic;
			if(remainingKeyChange == 0) {
				keyChangeCatList.push_back(transpose(prevMidi, currentKeyChange));
			}
			else {
				keyChangeCatList.push_back(transpose(keyChangeBar, currentKeyChange));
			}
		}
	}
	else if(!nextTranspositionFailed) {
		// cout << "MAGIC FAILED BUT WE GOT TRANSPOSE! \n";
		keyChangeCatList.push_back(finalBarOfPrev);
	}
	else {
		cout << "MAGIC FAILED! CRAIS \n";
	}
	
	MidiFile prevMidiAfterKeyChange = midiCat.run(keyChangeCatList, 0.0);
	double prevTempo = findLastTempo(prevMidi);
	double nextTempo = findFirstTempo(nextMidi);
	if(prevTempo < nextTempo) {
		double intermediateTempo = prevTempo * 0.6;
		prevMidiAfterKeyChange = tempoDilation(prevMidiAfterKeyChange, intermediateTempo);
		nextMidi = reverseTempoDilation(nextMidi, intermediateTempo);
	}
	else {
		prevMidiAfterKeyChange = tempoDilation(prevMidiAfterKeyChange, nextTempo);
	}
	
	if(!nextTranspositionFailed && nextTransposition != 0) {
		nextMidi = transpose(nextMidi, nextTransposition);
	}
	
	////-------------------------------
	//// Volume
	
	int lastVolume = getLastVolume(nextMidi);
	int firstVolume = getFirstVolume(prevMidiAfterKeyChange);
	int intermediateVolume = min(firstVolume, lastVolume) * 0.5;
	
	nextMidi = reverseVolumeInterpolation(nextMidi, intermediateVolume);
	prevMidiAfterKeyChange = volumeInterpolation(prevMidiAfterKeyChange, intermediateVolume);


	////-------------------------------
	//// Volume done
		
	catList.push_back(prevMidiAfterKeyChange);
	
	this->prevMidi = prevMidiAfterKeyChange;
	this->nextMidi = nextMidi;
	this->valid = true;
	this->nextTransposition = !nextTranspositionFailed ? nextTransposition : 0;
	this->prevMidiDuration = prevMidiAfterKeyChange.getFileDurationInSeconds();
	this->nextMidiDuration = nextMidi.getFileDurationInSeconds();
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

MidiFile Bridge::volumeInterpolation(MidiFile inputFile, int endVolume) {
	inputFile.joinTracks();
	inputFile.absoluteTicks();
	int totalTicks = inputFile.getFileDurationInTicks();
	int eventCount = inputFile.getEventCount(0);
	double progression = 0.0;
	int tickOfLastNoteOff;
	
	/// Get the tick of last note-off event
	for(int i=0; i < eventCount; i++) {
		if(inputFile.getEvent(0, i).isNoteOff()) {
			tickOfLastNoteOff = inputFile.getEvent(0, i).tick;
		}
	}

	
	// This loop adjusts the velocity values of the note events in the midi file
	// so that the new velocity values are results of linear interpolation
	// between the original velocity and endVolume
	for(int i=0; i < eventCount; i++) {
		progression = (double) inputFile.getEvent(0, i).tick / (double) tickOfLastNoteOff;
		
		if(inputFile.getEvent(0, i).isNoteOn()) {
			int originalVolume = inputFile.getEvent(0, i).getVelocity();
			int newVolume = endVolume + (double)(originalVolume - endVolume) * (1.0 - progression);
			inputFile.getEvent(0, i).setVelocity(newVolume);
		}
	}
	
	inputFile.sortTracks();
	return inputFile;
}


MidiFile Bridge::reverseVolumeInterpolation(MidiFile inputFile, int initialVolume) {
	inputFile.joinTracks();
	inputFile.absoluteTicks();
	int totalTicks = inputFile.getFileDurationInTicks();
	int eventCount = inputFile.getEventCount(0);
	double progression = 0.0;
	int beginningTick = 0;
	
	/// Get the tick of first note-on event
	for(int i=0; i < eventCount; i++) {
		if(inputFile.getEvent(0, i).isNoteOn()) {
			beginningTick = inputFile.getEvent(0, i).tick;
			break;
		}
	}

	// This loop adjusts the velocity values of the note events in the midi file
	// so that the new velocity values are results of linear interpolation
	// between the initialVolume and the original velocity
	for(int i=0; i < eventCount; i++) {
		progression = (double) (inputFile.getEvent(0, i).tick - beginningTick) / (double) (totalTicks - beginningTick);
		
		if(inputFile.getEvent(0, i).isNoteOn()) {
			int originalVolume = inputFile.getEvent(0, i).getVelocity();
			int newVolume = initialVolume + (double)(originalVolume - initialVolume) * progression;
			inputFile.getEvent(0, i).setVelocity(newVolume);
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

double Bridge::findLastTempo(MidiFile inputFile) {
	inputFile.joinTracks();
	int eventCount = inputFile.getEventCount(0);
	double lastTempo = 120.0; // default tempo
	for(int i=0; i<eventCount; i++) {
		if(inputFile.getEvent(0, i).isTempo()) {
			lastTempo = inputFile.getEvent(0, i).getTempoBPM();
		}
	}
	return lastTempo; 
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

int Bridge::getLastVolume(MidiFile inputFile) {
	inputFile.joinTracks();
	int eventCount = inputFile.getEventCount(0);
	int lastVolume = -1;
	
	for(int i=0; i<eventCount; i++) {
		if(inputFile.getEvent(0, i).isNoteOn()){
			lastVolume = inputFile.getEvent(0, i).getVelocity();
		}
	}
	
	return lastVolume;
}

int Bridge::getFirstVolume(MidiFile inputFile) {
	inputFile.joinTracks();
	int eventCount = inputFile.getEventCount(0);

	for(int i=0; i<eventCount; i++) {
		if(inputFile.getEvent(0, i).isNoteOn()){
			return inputFile.getEvent(0, i).getVelocity();
		}
	}
	
	return -1;
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
	
	// Gaps smaller than a 32nd note are assumed to be 
	// performance variations of the notes that are supposed
	// to fall on the same beat.
	int tickAmbiguity = inputFile.getTPQ() / 8; 

	for(int i=0; i<eventCount; i++) {
		if(inputFile.getEvent(0, i).isNoteOn() || inputFile.getEvent(0, i).isNoteOff()){
			int diff = inputFile.getEvent(0, i).tick - currentTick;
			if(diff > tickAmbiguity)  {
				endNoteKeys.clear();
				endNoteKeys.push_back(inputFile.getEvent(0, i).getKeyNumber());
				currentTick = inputFile.getEvent(0, i).tick;
			}
			else if(diff >= 0) {
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
	
	// Gaps smaller than a 32nd note are assumed to be 
	// performance variations of the notes that are supposed
	// to fall on the same beat.
	int tickAmbiguity = inputFile.getTPQ() / 8; 

	for(int i=0; i<eventCount; i++) {
		if(inputFile.getEvent(0, i).isNoteOn() || inputFile.getEvent(0, i).isNoteOff()){
			int diff = inputFile.getEvent(0, i).tick - currentTick;
			if(diff > tickAmbiguity) {
				currentTick = inputFile.getEvent(0, i).tick;
				if(!beginningNoteKeys.empty()) {
					return beginningNoteKeys;
				}
			}
			beginningNoteKeys.push_back(inputFile.getEvent(0, i).getKeyNumber());
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

// Returns a vector of vector of integer where 
// first vector of integer (of size 1) is the count of ways we can  
// sum S[0...m-1] coins to get sum n;
// and the rest are the sets of solutions coins
// that each set sums up to n
vector<vector<int>> Bridge::countSolutions( int S[], int m, int n ) 
{ 

	if(n == 0) {
		return vector<vector<int>>();
	}
	// If there is only 1 coin, 
	// and the coin does not add up to sum
	// then no solution exist
	if (m == 1) {
		vector<vector<int>> result;
		if(S[0] == n) {
			vector<int> coin{S[0]};
			result.push_back(coin);
		}
        return result;
	}
	
    // count is sum of solutions (i)  
    // including S[m-1] (ii) excluding S[m-1] (iii) including S[m-1] twice
	vector<vector<int>> excludeLast = countSolutions( S, m - 1, n );
	vector<vector<int>> result;

	if(!excludeLast.empty()) {
		for(vector<int> solution : excludeLast) {
			result.push_back(solution);
		}
	}
	excludeLast.clear();

	// If S[m-1] is zero or has different sign from n,
	// don't even think about including it
	if(S[m-1] * n <=0 ) {
		return result;
	}
	
	// If including the last coin would make the sum
	// there's no need to explore further
	// cuz the last coin is the solution
	if(n-S[m-1] == 0) {
		result.push_back(vector<int>{S[m-1]});
		return result;
	}
	
	vector<vector<int>> includeLast = countSolutions( S, m-1, n-S[m-1] );

	if(!includeLast.empty()) {
		for(vector<int> solution : includeLast) {
			solution.push_back(S[m-1]);
			result.push_back(solution);
		}
	}
	includeLast.clear();
	
	// If including the last coin twice would make the sum
	// there's no need to explore further
	// cuz twice the last coin is the solution
	if(n-S[m-1]*2 == 0) {
		result.push_back(vector<int>{S[m-1], S[m-1]});
		return result;
	}
	
	vector<vector<int>> includeLastTwice = countSolutions( S, m-1, n-S[m-1] * 2 );

	if(!includeLastTwice.empty()) {
		
		for(vector<int> solution : includeLastTwice) {
			solution.push_back(S[m-1]);
			solution.push_back(S[m-1]);
			result.push_back(solution);
		}
	}
	includeLastTwice.clear();

    return result; 
} 