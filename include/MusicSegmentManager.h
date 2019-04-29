#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <regex>
#include <boost/filesystem.hpp>
#include <climits>
#include "MusicSegment.h"
#include "MidiFile.h"
#include "BridgeManager.h"
#include "Bridge.h"
#include "MidiCat.h"
#include "EmotionState.h"

using namespace boost::filesystem;    
using namespace music_segment;
using namespace smf;
using namespace bridge_manager;
using namespace bridge;
using namespace midicat;

namespace music_segment_manager {

	class MusicSegmentManager {
		public:
			MusicSegmentManager(string inputFolderPath);
			void generateMusicFromEmotion(vector<EmotionState> emotionSequence);
			void generateMusicWithoutTransitionForComparison(vector<EmotionState> emotionSequence);

		private:
			std::vector<MusicSegment> musicSegmentList;
			BridgeManager bridgeManager;
			MusicSegment getMusicSegmentByEmotion(int valence, int arousal);	
			MidiFile shrinkOrExpand(MidiFile infile, double factor);
			
	};
}