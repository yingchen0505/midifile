#include "MidiFile.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <list>
#include <unistd.h>
#include "EmotionState.h"
#include "MusicSegmentManager.h"

#define INPUT_PATH	"../input"
#define OUTPUT_PATH	"../output"
#define EMOTION_SEQUENCE_FILE_NAME "emotion_sequence.txt"

using namespace smf;
using namespace music_segment_manager;
using std::string;

vector<EmotionState> readEmotionSequence(string emotionSequencePath);

int main(int argc, char* argv[]) {
	MusicSegmentManager musicSegmentManager(INPUT_PATH);
	std::string inputPath = INPUT_PATH;	
	std::string emotionSequenceFileName = EMOTION_SEQUENCE_FILE_NAME;
	std::string emotionSequenceFilePath = inputPath + "/" + emotionSequenceFileName;
	vector<EmotionState> emotionSequence = readEmotionSequence(emotionSequenceFilePath);
	musicSegmentManager.generateMusicFromEmotion(emotionSequence);
	musicSegmentManager.generateMusicWithoutTransitionForComparison(emotionSequence);
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
