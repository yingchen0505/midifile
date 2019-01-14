#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include "MidiCat.h"

using namespace midicat;
using namespace smf;

int startBar;
int endBar;

void checkOptions (Options& opts, int argc, char** argv);

int main(int argc, char* argv[]) {
	Options options;
	checkOptions(options, argc, argv);
	const char* filename = options.getArg(1).c_str();
	MidiFile infile(filename);
	infile.joinTracks();
	infile.deltaTicks();
	//infile.absoluteTicks();
	int eventCount = infile.getEventCount(0);
	
	MidiFile outfile;
	outfile.joinTracks();
	outfile.deltaTicks();
    outfile.setTicksPerQuarterNote(infile.getTicksPerQuarterNote());
	
	for (int i=0; i<eventCount; i++) {
		//std::cout << "i = " << i << "\n";
		int currentBar = infile.getEvent(0,i).bar;
		if(currentBar >= startBar && currentBar <= endBar){
			MidiEvent event = infile.getEvent(0,i);
			event.track = 0;
			outfile.addEvent(event);
			//outfile.addEvent(infile.getEvent(0,i));
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
	outfile.write(cout);
	
	return 0;
}

//////////////////////////////
//
// checkOptions --
//

void checkOptions(Options& opts, int argc, char* argv[]) {
   opts.define("s|start=i:1",  "Starting bar (inclusive)");
   opts.define("e|end=i:-1",  "Ending bar (inclusive)");
   opts.process(argc, argv);

   startBar     =  opts.getInt("start");
   endBar     = opts.getInt("end");
}