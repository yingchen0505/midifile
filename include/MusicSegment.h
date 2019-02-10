#include <cmath>
#include "MidiFile.h"
#include "MidiCat.h"

using namespace smf;
using namespace midicat;

namespace music_segment {
	class MusicSegment {
		public:
			int valence;
			int arousal;
			MidiFile* prep;
			MidiFile* mainLoop;
			MidiFile* mainLoopEnd;
			MidiFile* finalEnd;
			
			// Constructors and desctructors:
			MusicSegment() = default;
			MusicSegment(int valence, int arousal, MidiFile* prep, MidiFile* mainLoop, MidiFile* mainLoopEnd, MidiFile* finalEnd);
			~MusicSegment() = default;
			MusicSegment(const MusicSegment &src) = default;
			MusicSegment& operator=(const MusicSegment &rhs) = default;
			
			// Returns a midifile that repeats for the closest possible value to given time
			// If this segment is the absolute beginning of the entire midi output, prep (if exists) will be used
			// If this segment is the absolute end of the entire midi output, finalEnd (if exists) will be used
			// Else, only the mainLoop will be used since new prep and end bar will be provided by the bridges
			MidiFile repeat(double timeInSeconds, bool isAbsoluteStart, bool isAbsoluteEnd);

			// Returns true if main loop is null
			bool isInvalid();

		private:
	};
}