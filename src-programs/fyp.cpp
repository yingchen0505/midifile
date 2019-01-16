#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
//#include "MidiCat.h"
#include "MidiExcerptByBar.h"
#include "MusicSegment.h"

#define INPUT_PATH	"./input"
#define OUTPUT_PATH	"./output"

using namespace midi_excerpt_by_bar;
using namespace smf;

//void checkOptions (Options& opts, int argc, char** argv);

int main(int argc, char* argv[]) {
	MidiExcerptByBar midiExcerptByBar;
	midiExcerptByBar.run(argc, argv);
}


//////////////////////////////
//
// checkOptions --
//

/*
void checkOptions(Options& opts, int argc, char* argv[]) {
   opts.define("s|start=i:1",  "Starting bar (inclusive)");
   opts.define("e|end=i:-1",  "Ending bar (inclusive)");
   opts.process(argc, argv);

   startBar     =  opts.getInt("start");
   endBar     = opts.getInt("end");
}*/