#include <iostream>
#include <string>
#include <map>
//#include <unistd.h>
//#include <vector>
#include <regex>
//#include <fstream>
#include <boost/filesystem.hpp>
#include "MidiFile.h"
#include "MusicSegment.h"
#include "Bridge.h"

using namespace smf;
using namespace bridge;
using namespace boost::filesystem;   
using namespace music_segment;
using std::string;
using std::regex;
using std::map;

namespace bridge_manager {
	class BridgeManager {
		public:
			BridgeManager() = default;
			BridgeManager(string bridgeFolderPath);
			~BridgeManager() = default;
			MidiFile getBridge(MusicSegment prevSegment, MusicSegment nextSegment);
		
		private:
			map<string, Bridge> bridgeMap;
		
	};
}