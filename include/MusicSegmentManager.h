#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <boost/filesystem.hpp>
#include "MusicSegment.h"

using namespace boost::filesystem;    
using namespace music_segment;

namespace music_segment_manager {
	class MusicSegmentManager {
		public:
			MusicSegmentManager(string inputFolderPath);
			
			
		private:
			std::vector<MusicSegment> musicSegmentList;
		
	};
}