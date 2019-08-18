#include "MidiFile.h"
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

#define INPUT_PATH	"./input"
#define OUTPUT_PATH	"./output"

using namespace midi_excerpt_by_bar;
using namespace smf;
using namespace music_segment_manager;
using std::string;

vector<EmotionState> readEmotionSequence();

int main(int argc, char* argv[]) {

	MusicSegmentManager musicSegmentManager(INPUT_PATH);
	
	vector<EmotionState> emotionSequence = readEmotionSequence();
	
	musicSegmentManager.generateMusicFromEmotion(emotionSequence);
	musicSegmentManager.generateMusicWithoutTransitionForComparison(emotionSequence);
	
}

vector<EmotionState> readEmotionSequence() {
	
	std::ifstream in((string)(INPUT_PATH) + "/" + "emotion_sequence.txt");
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
