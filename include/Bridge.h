#include <iostream>
#include <string>
#include "MidiFile.h"
#include "MusicSegment.h"

using namespace smf;
using namespace music_segment;
using std::string;

namespace bridge {
	class Bridge {
		public:
			Bridge() {
				valid = false;
			}
			Bridge(MusicSegment prevSegment, MusicSegment nextSegment, MidiFile bridgeMidi);
			Bridge(string ID, MidiFile bridgeMidi, int barErosion);
			
			bool valid;
			string ID;	// is the same as file name without last "_digit" that represents barErosion
			int barErosion; // number of end bars that the previous segment has to drop when connected to this bridge
			MidiFile bridgeMidi;
			string prevSegmentID;
			string nextSegmentID;
			
			bool isInvalid();
		
		private:
			
	};
}