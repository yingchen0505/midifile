#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <list>
#include "MidiCat.h"
#include "MidiExcerptByBar.h"
#include "MusicSegment.h"

#define INPUT_PATH	"./input"
#define OUTPUT_PATH	"./output"

using namespace midi_excerpt_by_bar;
using namespace smf;


// Pause (in seconds) between concatenated midi files.
// used with -p option
double pause = 0.0;  

int startBar = 1;	// Use with -s option (inclusive)
int endBar = -1;	// Use with -e option (inclusive)

void checkOptions (Options& opts, int argc, char** argv);

int main(int argc, char* argv[]) {
	Options options;
	checkOptions(options, argc, argv);
	MidiCat midiCat;
	list<const char *> inputFileNames;

	for (int i=1; i<=options.getArgCount(); i++) {
		inputFileNames.push_back(options.getArg(i).c_str());
	}
	
	vector<MidiFile> inputFiles;
	while(!inputFileNames.empty()){
		inputFiles.push_back(MidiFile(inputFileNames.front()));
		inputFileNames.pop_front();
	}

	MidiFile output = midiCat.run(inputFiles, pause);
	output.write(cout);
	
	
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