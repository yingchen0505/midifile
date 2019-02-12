#ifndef _MIDIEXCERPTBYBAR_H_INCLUDED
#define _MIDIEXCERPTBYBAR_H_INCLUDED

#include "MidiFile.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>

using namespace smf;
using std::string;

namespace midi_excerpt_by_bar {
	
	class MidiExcerptByBar {
		public:
			MidiExcerptByBar() = default;
			~MidiExcerptByBar() = default;

			void run(int startBar, int endBar, string filePath);
		
		private:
		
	};

}

#endif /* _MIDIEXCERPTBYBAR_H_INCLUDED */