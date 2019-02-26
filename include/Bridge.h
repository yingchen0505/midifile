#ifndef _BRIDGE_H_INCLUDED
#define _BRIDGE_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include "MidiFile.h"
#include "MusicSegment.h"
#include "MidiCat.h"
#include "MidiExcerptByBar.h"

using namespace smf;
using namespace music_segment;
using namespace midicat;
using namespace midi_excerpt_by_bar;
using std::string;
using std::vector;

namespace bridge {
	class Bridge {
		public:
			Bridge() {
				valid = false;
			}
			Bridge(MusicSegment prevSegment, MusicSegment nextSegment);
			Bridge(string ID, MidiFile bridgeMidi, int barErosionIntoPrevSeg, int barErosionIntoNextSeg);
			
			bool valid;
			string ID;	// is the same as file name without last "_digit" that represents barErosion
			int barErosionIntoPrevSeg; // number of end bars that the previous segment has to drop when connected to this bridge
			int barErosionIntoNextSeg; // number of beginning bars that the next segment has to drop when connected to this bridge
			MidiFile bridgeMidi;
			string prevSegmentID;
			string nextSegmentID;
			
			bool isInvalid();
		
		private:
			MidiFile tempoDilation(MidiFile inputFile, double finalTempo);
			double findFirstTempo(MidiFile inputFile); // Returns the value of first tempo marking (BPM)
	};
}

#endif /* _BRIDGE_H_INCLUDED */