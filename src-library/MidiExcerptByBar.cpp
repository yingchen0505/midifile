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
	
	// Initialize output file
	MidiFile outfile;
	outfile.joinTracks();
	outfile.deltaTicks();
    outfile.setTicksPerQuarterNote(infile.getTicksPerQuarterNote());
	
	MidiEvent tempoBeforeStart;
	bool tempoBeforeStartIsAdded = false;

	// Loop through all events except the end-of-file event
	for (int i=0; i<eventCount; i++) {
		int currentBar = infile.getEvent(0,i).bar;
		
		// Store the latest tempo setting before start bar
		if(currentBar < startBar && infile.getEvent(0,i).isTempo()) {
			tempoBeforeStart = infile.getEvent(0,i);
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
			
			// Add current event to output file
			MidiEvent event = infile.getEvent(0,i);
			// Make track number 0 so that it does not exceed the number of tracks in output file
			event.track = 0;
			outfile.addEvent(event);
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