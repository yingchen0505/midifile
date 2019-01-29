#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include "MusicSegment.h"

using namespace music_segment;

namespace music_segment_manager {
	class MusicSegmentManager {
		public:
			MusicSegmentManager(string inputFolderPath);
			
			
		private:
			std::vector<MusicSegment> musicSegmentList;
		
	};
}