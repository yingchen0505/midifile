#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>

using namespace smf;

namespace midi_excerpt_by_bar {
	
	class MidiExcerptByBar {
		public:
			int startBar = 1;	// Use with -s option (inclusive)
			int endBar = -1;	// Use with -e option (inclusive)
 			
			// Function declarations:
			MidiExcerptByBar() = default;
			~MidiExcerptByBar() = default;
			void checkOptions (Options& opts, int argc, char** argv);
			void run(int argc, char* argv[]);
		
		private:
		
	};

}