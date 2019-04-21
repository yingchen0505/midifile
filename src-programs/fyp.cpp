#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <list>
#include <unistd.h>
#include "EmotionState.h"
#include "MidiExcerptByBar.h"
//#include "MusicSegment.h"
#include "MusicSegmentManager.h"

#define INPUT_PATH	"./input"
#define OUTPUT_PATH	"./output"

using namespace midi_excerpt_by_bar;
using namespace smf;
//using namespace music_segment;
using namespace music_segment_manager;
using std::string;



// pause (in seconds) between concatenated midi files.
// used with -p option
double pauseBetweenMidi = 0.0;  

int startBar = 1;	// Use with -s option (inclusive)
int endBar = -1;	// Use with -e option (inclusive)
double volumeFactor = 1.0; // Use with -v option

void checkOptions (Options& opts, int argc, char** argv);

int main(int argc, char* argv[]) {
	
	Options options;
	checkOptions(options, argc, argv);
	
	// Read and print text:
	//MidiFile infile(options.getArg(1).c_str());
	//cout << infile;

	
	 MusicSegmentManager musicSegmentManager(INPUT_PATH);
	
	std::ifstream in("emotion_sequence.txt");
	int min;
	int sec;
	int valence;
	int arousal;
	vector<EmotionState> emotionSequence;
	while(in >> min) {
		in >> sec;
		in >> valence;
		in >> arousal;
		emotionSequence.push_back({min * 60 + sec, valence, arousal}); 
	}
	
	musicSegmentManager.generateMusicFromEmotion(emotionSequence);

	
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
	// MidiExcerptByBar midiExcerptByBar;
	// midiExcerptByBar.run(startBar, endBar, options.getArg(1)).write(cout);
	
	//// remove parts
/* 	MidiFile infile(options.getArg(1));
	for(int i=0; i< infile.getTrackCount(); i++) {
		
	} */
	
	////--------------------------------------------
	//// Volume adjustment tool
	// MidiFile infile(options.getArg(1));
	// infile.joinTracks();
	// for(int i=0; i< infile.getEventCount(0); i++) {
		// if(infile.getEvent(0, i).isNoteOn()) {
			// int velocity = infile.getEvent(0, i).getVelocity() * volumeFactor;
			// infile.getEvent(0, i).setVelocity(velocity);
		// }
	// }
	// infile.splitTracks();
	// infile.write(cout);
	///----------------------------------------
	
	/*
	MidiFile infile(options.getArg(1));
	
	std::ofstream orig_inMidi; // without std::, reference would be ambiguous because of Boost
	orig_inMidi.open("infile.mid");
	infile.write(orig_inMidi);
	orig_inMidi.close();
	
	midiExcerptByBar.run(startBar, endBar, infile);*/
	
}


//////////////////////////////
//
// checkOptions --
//

void checkOptions(Options& opts, int argc, char* argv[]) {
	opts.define("p|pause=i:1",  "pause (in seconds) between concatenated midi files.");
	opts.define("s|start=i:1",  "Starting bar (inclusive)");
	opts.define("e|end=i:-1",  "Ending bar (inclusive)");
	opts.define("v|volumeFactor=d:1.0",  "volume factor");
	opts.process(argc, argv);
	
	pauseBetweenMidi = opts.getDouble("pause");
	startBar     =  opts.getInt("start");
	endBar     = opts.getInt("end");
	volumeFactor = opts.getDouble("volumeFactor");
}