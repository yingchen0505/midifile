#include "MidiExcerptByBar.h"

using namespace midi_excerpt_by_bar;
MidiFile MidiExcerptByBar::run(int startBar, int endBar, MidiFile infile) {
	infile.joinTracks();
	infile.deltaTicks();
	
	int eventCount = infile.getEventCount(0) - 1; // exclude end of file event
	
	infile.linkNotePairs();
	
	// Initialize output file
	MidiFile outfile;
	outfile.joinTracks();
	outfile.deltaTicks();
    outfile.setTicksPerQuarterNote(infile.getTicksPerQuarterNote());
	
	MidiEvent tempoBeforeStart;
	bool hasTempoBeforeStart = false;
	bool tempoBeforeStartIsAdded = false;
	
	MidiEvent timeSignature;
	bool hasTimeSignature = false;
	bool timeSignatureIsAdded = false;
	
	// Notes that are turned on during the selected bars but only turned off afterwards
	// Need to add the note-off events at the end of the file
	std::vector<MidiEvent> noteOffAfterEndBar; 
	
	int currentTick = 0;
	
	std::vector<int> startBarTicks = infile.getBeginningAndEndTicksByBar(startBar);
	if(startBarTicks.empty()) {
		std::cout << "Error: start bar out of bound. \n";
		return outfile; 
	}
	const int beginningTicksOfStartBar = startBarTicks[0];
	std::cout << "beginningTicksOfStartBar = " << beginningTicksOfStartBar << "\n";
	
	std::vector<int> endBarTicks = infile.getBeginningAndEndTicksByBar(endBar);
	if(endBarTicks.empty()) {
		std::cout << "Error: end bar out of bound. \n";
		return outfile; 
	}
	const int endTicksOfEndBar = endBarTicks[1];
	bool lostTempo = infile.getEvent(0,0).isTempo();
	
	// Loop through all events except the end-of-file event
	for (int i=0; i<eventCount; i++) {
		int currentBar = infile.getEvent(0,i).bar;
		currentTick += infile.getEvent(0,i).tick;
		std::cout << "infile.getEvent(0,i).bar = " << infile.getEvent(0,i).bar << "\n";

		// Store the latest tempo setting before start bar
		if(currentBar < startBar && infile.getEvent(0,i).isTempo()) {
			hasTempoBeforeStart = true;
			tempoBeforeStart = infile.getEvent(0,i);
		}
		
		// Store the latest time signature before start bar
		if(currentBar < startBar && infile.getEvent(0,i).isTimeSignature()) {
			hasTimeSignature = true;
			timeSignature = infile.getEvent(0,i);
		}
		
		// Add timber setting before start bar
		if(currentBar < startBar && infile.getEvent(0,i).isTimbre()) {
			MidiEvent timbreEvent = infile.getEvent(0,i);
			timbreEvent.clearVariables();
			outfile.addEvent(timbreEvent);
		}
		
		// Find events between targeted bars:
		if(lostTempo && i==0) {
			std::cout << "hello tempo lost. currentBar >= startBar && currentBar <= endBar = " << (currentBar >= startBar && currentBar <= endBar) <<"\n";
			std::cout << "currentBar = " << currentBar << "\n";
			std::cout << "startBar = " << startBar << "\n";
			std::cout << "endBar = " << endBar << "\n";
			
		}
		if(currentBar >= startBar && currentBar <= endBar){
			
			int ticksSinceBeginningOfStartBar = currentTick - beginningTicksOfStartBar;
			if(ticksSinceBeginningOfStartBar < 0) {
				std::cout << "Error: currentTick < beginningTicksOfStartBar. Bar tick map is wrong! \n";
				return outfile;
			}
			
			// Add tempo setting before start bar if it hasn't been added
			if(!tempoBeforeStartIsAdded && hasTempoBeforeStart) {
				tempoBeforeStart.clearVariables();
				outfile.addEvent(tempoBeforeStart);
				tempoBeforeStartIsAdded = true;
			}
			
			// Add time signature before start bar if it hasn't been added
			if(!timeSignatureIsAdded && hasTimeSignature) {
				timeSignature.clearVariables();
				outfile.addEvent(timeSignature);
				timeSignatureIsAdded = true;
			}

			if(infile.getEvent(0,i).isNoteOff()) {
				MidiEvent* linkedEvent = infile.getEvent(0,i).getLinkedEvent();
				// Ignore this note-off if it is for a note before the targeted bar range
				if(linkedEvent && (*linkedEvent).bar < startBar) continue;
			}
			
			if(infile.getEvent(0,i).isNoteOn()) {
				MidiEvent* linkedEvent = infile.getEvent(0,i).getLinkedEvent();
				// If this note is only turned off after selected bar range, 
				// need to add the note-off to end of file
				if(linkedEvent && (*linkedEvent).bar > endBar) {
					noteOffAfterEndBar.push_back(*linkedEvent);
				}
			}
			
			// Make track number 0 so that it does not exceed the number of tracks in output file
			infile.getEvent(0,i).track = 0;
			if(infile.getEvent(0,i).tick > ticksSinceBeginningOfStartBar) {
				infile.getEvent(0,i).tick = ticksSinceBeginningOfStartBar;
			}
			if(lostTempo) {
				if(i==0) std::cout << "idk waddu say alr \n";
			}
			// Add current event to output file
			outfile.addEvent(infile.getEvent(0,i));
		}
	}
	
	int currentOutputLengthInTicks = outfile.getFileDurationInTicks();
	// We don't want a gap between the end of file and the actual end of the last bar
	// This gap will be filled by either a rest, or the note-off events yet to be added.
	int gap = endTicksOfEndBar - beginningTicksOfStartBar - currentOutputLengthInTicks - 1; // minus 1 to prevent it from overflowing to next bar
	
	// If there isn't any notes still on, then add a rest to fill the gap
	if(noteOffAfterEndBar.empty() && gap > 0){
		std::string text = "This is a rest to complete the last bar";
		MidiEvent rest;
		rest.makeText(text);
		rest.tick = gap;
		outfile.addEvent(rest);
	}
	// There are notes still on, therefore turn them off, which fills the gap at the same time
	else {
		for(int i = 0; i < noteOffAfterEndBar.size(); i++) {
			if(i==0) {
				noteOffAfterEndBar[i].tick = gap;
			}
			else {
				noteOffAfterEndBar[i].tick = 0;
			}
			outfile.addEvent(noteOffAfterEndBar[i]);
		}
	}

	 // insert an end-of track Meta Event
   int tpq = outfile.getTicksPerQuarterNote();
   MidiEvent mfevent;
   mfevent.tick = tpq;
   mfevent.track = 0;
   mfevent.resize(3);
   mfevent[0] = 0xff;
   mfevent[1] = 0x2f;
   mfevent[2] = 0;
   outfile.addEvent(mfevent);

   	outfile.absoluteTicks();
	outfile.sortTracks();

	outfile.updateBarNumber();
	
	return outfile;
}