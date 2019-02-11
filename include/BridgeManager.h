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

namespace bridge_manager {
	class BridgeManager {
		public:
			BridgeManager() = default;
			BridgeManager(std::string bridgeFolderPath);
			~BridgeManager() = default;
			MidiFile getBridge(MusicSegment prevSegment, MusicSegment nextSegment);
		
		private:
			std::map<std::string, Bridge> bridgeMap;
		
	};
}