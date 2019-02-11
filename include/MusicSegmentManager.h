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

using namespace boost::filesystem;    
using namespace music_segment;
using namespace smf;
using namespace bridge_manager;

namespace music_segment_manager {
	class MusicSegmentManager {
		public:
			MusicSegmentManager(string inputFolderPath);

		private:
			std::vector<MusicSegment> musicSegmentList;
			BridgeManager bridgeManager;
		
	};
}