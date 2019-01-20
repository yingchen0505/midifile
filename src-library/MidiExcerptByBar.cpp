#include "MidiExcerptByBar.h"

using namespace midi_excerpt_by_bar;

void MidiExcerptByBar::run(int argc, char* argv[]) {
	Options options;
	checkOptions(options, argc, argv);
	const char* filename = options.getArg(1).c_str();
	
	// Initialize input file
	MidiFile infile(filename);
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
	bool tempoBeforeStartIsAdded = false;
	
	MidiEvent timeSignature;
	bool timeSignatureIsAdded = false;
	
	// Notes that are turned on during the selected bars but only turned off afterwards
	// Need to add the note-off events at the end of the file
	std::vector<MidiEvent> noteOffAfterEndBar; 

	// Loop through all events except the end-of-file event
	for (int i=0; i<eventCount; i++) {
		int currentBar = infile.getEvent(0,i).bar;
		
		// Store the latest tempo setting before start bar
		if(currentBar < startBar && infile.getEvent(0,i).isTempo()) {
			tempoBeforeStart = infile.getEvent(0,i);
		}
		
		// Store the latest time signature before start bar
		if(currentBar < startBar && infile.getEvent(0,i).isTimeSignature()) {
			timeSignature = infile.getEvent(0,i);
		}
		
		// Add timber setting before start bar
		if(currentBar < startBar && infile.getEvent(0,i).isTimbre()) {
			MidiEvent timbreEvent = infile.getEvent(0,i);
			outfile.addEvent(timbreEvent);
		}
		
		// Find events between targeted bars
		if(currentBar >= startBar && currentBar <= endBar){
			// Add tempo setting before start bar if it hasn't been added
			if(!tempoBeforeStartIsAdded) {
				tempoBeforeStart.track = 0;
				outfile.addEvent(tempoBeforeStart);
				tempoBeforeStartIsAdded = true;
			}
			
			// Add time signature before start bar if it hasn't been added
			if(!timeSignatureIsAdded) {
				timeSignature.track = 0;
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
			
			// Add current event to output file
			outfile.addEvent(infile.getEvent(0,i));
		}
	}
	
	// Turn off any note still on
	for(int i = 0; i < noteOffAfterEndBar.size(); i++) {
		outfile.addEvent(noteOffAfterEndBar[i]);
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
	outfile.write(std::cout);
}

//////////////////////////////
//
// checkOptions --
//

void MidiExcerptByBar::checkOptions(Options& opts, int argc, char* argv[]) {
   opts.define("s|start=i:1",  "Starting bar (inclusive)");
   opts.define("e|end=i:-1",  "Ending bar (inclusive)");
   opts.process(argc, argv);

   startBar     =  opts.getInt("start");
   endBar     = opts.getInt("end");
}