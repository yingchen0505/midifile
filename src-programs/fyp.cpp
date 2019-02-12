#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <list>
#include <unistd.h>
#include "MidiExcerptByBar.h"
//#include "MusicSegment.h"
#include "MusicSegmentManager.h"

#define INPUT_PATH	"./input"
#define OUTPUT_PATH	"./output"

using namespace midi_excerpt_by_bar;
using namespace smf;
//using namespace music_segment;
using namespace music_segment_manager;

// pause (in seconds) between concatenated midi files.
// used with -p option
double pauseBetweenMidi = 0.0;  

int startBar = 1;	// Use with -s option (inclusive)
int endBar = -1;	// Use with -e option (inclusive)

void checkOptions (Options& opts, int argc, char** argv);

int main(int argc, char* argv[]) {
	
	Options options;
	checkOptions(options, argc, argv);
	
	MusicSegmentManager musicSegmentManager(INPUT_PATH);
	musicSegmentManager.generateMusicFromEmotion();
	
	//// Debugger by event
	/*
	MidiFile infile(options.getArg(1).c_str());
	infile.joinTracks();
	
	for (int i=0; i<infile.getEventCount(0); i++){
		cout << "bar = " << infile.getEvent(0, i).bar << "\n";
		cout << "ticks since beginning of bar = " << infile.getEvent(0, i).ticksSinceBeginningOfBar << "\n";
		cout << "ticks till end of bar = " << infile.getEvent(0, i).ticksTillEndOfBar << "\n";
	}*/
	
	
	//// Debugger by bar
	/*
	int currBar = 1;
	
	while(1) {
		std::vector<int> ticks = infile.getBeginningAndEndTicksByBar(currBar);
		if(ticks.empty()){
			break;
		}
		cout << "bar = " << currBar << "\n";
		cout << "beg ticks = " << ticks[0] << "\n";
		cout << "end ticks = " << ticks[1] << "\n";
		currBar++;
	}*/
	
	///// MidiCat Tester
	/*
	MidiFile prep(options.getArg(1).c_str());
	MidiFile mainLoop(options.getArg(2).c_str());
	MidiFile mainLoopEnd(options.getArg(3).c_str());
	MidiFile finalEnd(options.getArg(4).c_str());
	MusicSegment musicSegment(0, 0, &prep, &mainLoop, &mainLoopEnd, &finalEnd);
	MidiFile outfile = musicSegment.repeat(150, true, true);
	//cout << outfile;
	outfile.write(cout);*/

	///// Midi Excerpt Tool
	
	//MidiExcerptByBar midiExcerptByBar;
	//midiExcerptByBar.run(argc, argv);
	
}


//////////////////////////////
//
// checkOptions --
//

void checkOptions(Options& opts, int argc, char* argv[]) {
	opts.define("p|pause=i:1",  "pause (in seconds) between concatenated midi files.");
	opts.define("s|start=i:1",  "Starting bar (inclusive)");
	opts.define("e|end=i:-1",  "Ending bar (inclusive)");
	opts.process(argc, argv);
	
	pauseBetweenMidi = opts.getDouble("pause");
	startBar     =  opts.getInt("start");
	endBar     = opts.getInt("end");
}