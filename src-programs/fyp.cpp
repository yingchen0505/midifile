#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <list>
//#include "MidiCat.h"
#include "MidiExcerptByBar.h"
#include "MusicSegment.h"

#define INPUT_PATH	"./input"
#define OUTPUT_PATH	"./output"

using namespace midi_excerpt_by_bar;
using namespace smf;
using namespace music_segment;


// Pause (in seconds) between concatenated midi files.
// used with -p option
double pause = 0.0;  

int startBar = 1;	// Use with -s option (inclusive)
int endBar = -1;	// Use with -e option (inclusive)

void checkOptions (Options& opts, int argc, char** argv);

int main(int argc, char* argv[]) {
	Options options;
	checkOptions(options, argc, argv);
	
	MidiFile prep(options.getArg(1).c_str());
	MidiFile mainLoop(options.getArg(2).c_str());
	MidiFile finalEnd(options.getArg(3).c_str());
	
	MusicSegment musicSegment(&prep, &mainLoop, NULL, &finalEnd);
	MidiFile outfile = musicSegment.repeat(150, true, true);
	outfile.write(cout);

	//MidiExcerptByBar midiExcerptByBar;
	//midiExcerptByBar.run(argc, argv);
}


//////////////////////////////
//
// checkOptions --
//

void checkOptions(Options& opts, int argc, char* argv[]) {
	opts.define("p|pause=i:1",  "Pause (in seconds) between concatenated midi files.");
	opts.define("s|start=i:1",  "Starting bar (inclusive)");
	opts.define("e|end=i:-1",  "Ending bar (inclusive)");
	opts.process(argc, argv);
	
	pause = opts.getInt("pause");
	startBar     =  opts.getInt("start");
	endBar     = opts.getInt("end");
}