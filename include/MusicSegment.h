#ifndef _MUSICSEGMENT_H_INCLUDED
#define _MUSICSEGMENT_H_INCLUDED

#include <cmath>
#include <iostream>
#include "MidiFile.h"
#include "MidiCat.h"
#include "MidiExcerptByBar.h"

using namespace smf;
using namespace midicat;
using namespace midi_excerpt_by_bar;

namespace music_segment {
	class MusicSegment {
		public:
			int valence;
			int arousal;
			int ID;		// unique identification number among music segments with exact same emotion
			MidiFile* prep;
			MidiFile* mainLoop;
			MidiFile* mainLoopEnd;
			MidiFile* finalEnd;
			
			// Constructors and desctructors:
			MusicSegment();
			MusicSegment(int valence, int arousal, int ID, MidiFile* prep, MidiFile* mainLoop, MidiFile* mainLoopEnd, MidiFile* finalEnd);
			~MusicSegment() = default;
			MusicSegment(const MusicSegment &src) = default;
			MusicSegment& operator=(const MusicSegment &rhs) = default;
			
			// Returns a midifile that repeats for the closest possible value to given time
			MidiFile repeat(double timeInSeconds, int beginningBarErosion, int endBarErosion);
			
			// Returns true if main loop is null
			bool isInvalid();

		private:
	};
}

#endif /* _MUSICSEGMENT_H_INCLUDED */