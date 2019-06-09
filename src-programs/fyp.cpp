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
#include "MusicSegmentManager.h"

#define INPUT_PATH	"../input"
#define OUTPUT_PATH	"../output"
#define EMOTION_SEQUENCE_FILE_NAME "emotion_sequence.txt"

using namespace midi_excerpt_by_bar;
using namespace smf;
using namespace music_segment_manager;
using std::string;

// pause (in seconds) between concatenated midi files.
// used with -p option
double pauseBetweenMidi = 0.0;  

int startBar = 1;	// Use with -s option (inclusive)
int endBar = -1;	// Use with -e option (inclusive)
double volumeFactor = 1.0; // Use with -v option

void checkOptions (Options& opts, int argc, char** argv);
vector<EmotionState> readEmotionSequence(string emotionSequencePath);

int main(int argc, char* argv[]) {
	
	Options options;
	checkOptions(options, argc, argv);
	
	MusicSegmentManager musicSegmentManager(INPUT_PATH);
	std::string inputPath = INPUT_PATH;	
	std::string emotionSequenceFileName = EMOTION_SEQUENCE_FILE_NAME;
	std::string emotionSequenceFilePath = inputPath + "/" + emotionSequenceFileName;
	vector<EmotionState> emotionSequence = readEmotionSequence(emotionSequenceFilePath);
	musicSegmentManager.generateMusicFromEmotion(emotionSequence);
	musicSegmentManager.generateMusicWithoutTransitionForComparison(emotionSequence);
	
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


vector<EmotionState> readEmotionSequence(string emotionSequenceFilePath) {
	std::ifstream in(emotionSequenceFilePath);
	int min;
	int sec;
	int valence;
	int arousal;
	vector<EmotionState> emotionSequence;
	int endOfFileTime;
	
	while(in >> min) {
		in >> sec;
		if(in >> valence) {
			in >> arousal;
			emotionSequence.push_back({min * 60 + sec, 0, valence, arousal}); 
		}
		else {
			endOfFileTime = min * 60 + sec;
		}
	}
	
	for (int i=0; i<emotionSequence.size() - 1; i++) {
		emotionSequence.at(i).endTime = emotionSequence.at(i+1).startTime;
	}
	
	emotionSequence.back().endTime = endOfFileTime;
	
	return emotionSequence;
}
