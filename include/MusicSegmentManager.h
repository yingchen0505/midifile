#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <regex>
#include <boost/filesystem.hpp>
#include "MusicSegment.h"
#include "MidiFile.h"
#include "BridgeManager.h"
#include "Bridge.h"
#include "MidiCat.h"

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
			void generateMusicFromEmotion();

		private:
			std::vector<MusicSegment> musicSegmentList;
			BridgeManager bridgeManager;
		
	};
}