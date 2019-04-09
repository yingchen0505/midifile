#ifndef _MIDIEXCERPTBYBAR_H_INCLUDED
#define _MIDIEXCERPTBYBAR_H_INCLUDED

#include "MidiFile.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <algorithm>
#include "Options.h"

using namespace smf;
using std::string;
using std::to_string;
using std::max;

namespace midi_excerpt_by_bar {
	
	class MidiExcerptByBar {
		public:
			MidiExcerptByBar() = default;
			~MidiExcerptByBar() = default;

			MidiFile run(int startBar, int endBar, MidiFile infile);
		private:
		
	};

}

#endif /* _MIDIEXCERPTBYBAR_H_INCLUDED */