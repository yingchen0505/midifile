#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <list>
#include <unistd.h>
#include "MidiExcerptByBar.h"
#include "MusicSegment.h"

#define INPUT_PATH	"./input"
#define OUTPUT_PATH	"./output"

using namespace midi_excerpt_by_bar;
using namespace smf;
using namespace music_segment;


// pause (in seconds) between concatenated midi files.
// used with -p option
double pauseBetweenMidi = 0.0;  

int startBar = 1;	// Use with -s option (inclusive)
int endBar = -1;	// Use with -e option (inclusive)

void checkOptions (Options& opts, int argc, char** argv);

int main(int argc, char* argv[]) {
	/*
	// Store current directory so as to return to this before exiting constructor
	char homeDirectory[1024];
	getcwd(homeDirectory, sizeof(homeDirectory));
	
	char currDirectory[1024];
	
	// Change to image input directory
	chdir(INPUT_PATH);
	
	getcwd(currDirectory, sizeof(currDirectory));
	cout << currDirectory << "\n";*/
	
	Options options;
	checkOptions(options, argc, argv);
	MidiFile infile(options.getArg(1).c_str());
	
	infile.joinTracks();
	/*
	for (int i=0; i<infile.getEventCount(0); i++){
		cout << "bar = " << infile.getEvent(0, i).bar << "\n";
		cout << "ticks since beginning of bar = " << infile.getEvent(0, i).ticksSinceBeginningOfBar << "\n";
		cout << "ticks till end of bar = " << infile.getEvent(0, i).ticksTillEndOfBar << "\n";
	}*/
	
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
	}
	
	//infile.write(cout);
	/*
	MidiFile prep(options.getArg(1).c_str());
	MidiFile mainLoop(options.getArg(2).c_str());
	MidiFile mainLoopEnd(options.getArg(3).c_str());
	MidiFile finalEnd(options.getArg(4).c_str());
	
	//MusicSegment musicSegment(NULL, &mainLoop, &mainLoopEnd, NULL);
	
	MusicSegment musicSegment(&prep, &mainLoop, &mainLoopEnd, &finalEnd);
	MidiFile outfile = musicSegment.repeat(150, true, true);
	cout << outfile;
	outfile.write(cout);*/

	//MidiExcerptByBar midiExcerptByBar;
	//midiExcerptByBar.run(argc, argv);
	/*
	chdir(homeDirectory);
	
	getcwd(currDirectory, sizeof(currDirectory));
	cout << currDirectory << "\n";*/
	
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