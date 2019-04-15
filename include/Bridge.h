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
			int prevTransposition;
			int nextTransposition;
			
			bool isInvalid();
		
		private:
			MidiFile tempoDilation(MidiFile inputFile, double finalTempo);
			MidiFile reverseTempoDilation(MidiFile inputFile, double initialTempo);

			double findFirstTempo(MidiFile inputFile); // Returns the value of first tempo marking (BPM)
			double findLastTempo(MidiFile inputFile); // Returns the value of last tempo marking (BPM)
			MidiFile getFirstBarsFromSegment(MusicSegment inputSegment, int bars); // Returns the first few bars from a segment
			MidiFile getLastBarsFromSegment(MusicSegment inputSegment, int bars);
			MidiEvent getLastKeySignature(MidiFile inputFile);
			MidiEvent getFirstKeySignature(MidiFile inputFile);
			MidiFile transpose(MidiFile inputFile, int keyChange);
			vector<int> getEndNoteKeys(MidiFile inputFile);		
			vector<int> getBeginningNoteKeys(MidiFile inputFile);
			int getPhraseLengthInBars(MidiFile inputFile);
			vector<vector<int>> countSolutions( int S[], int m, int n );
			MidiFile reverseVolumeInterpolation(MidiFile inputFile, int initialVolume);
			MidiFile volumeInterpolation(MidiFile inputFile, int endVolume);


	};
}

#endif /* _BRIDGE_H_INCLUDED */