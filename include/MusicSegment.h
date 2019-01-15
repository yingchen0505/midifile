#include "MidiFile.h"

using namespace smf;

namespace music_segment {
	class MusicSegment {
		public:
			MidiFile* prepBar;
			MidiFile* mainLoop;
			MidiFile* mainLoopEndBar;
			MidiFile* finalEndBar;
			
			// Constructors and desctructors:
			MusicSegment() = default;
			MusicSegment(MidiFile* prepBar, MidiFile* mainLoop, MidiFile* mainLoopEndBar, MidiFile* finalEndBar);
			~MusicSegment() = default;
			
			// Returns a midifile that repeats for the closest possible value to given time
			// If this segment is the absolute beginning of the entire midi output, prepBar (if exists) will be used
			// If this segment is the absolute end of the entire midi output, finalEndBar (if exists) will be used
			// Else, only the mainLoop will be used since new prepBar and end bar will be provided by the bridges
			MidiFile repeat(double timeInSeconds, bool isAbsoluteStart, bool isAbsoluteEnd);

		private:
	}
}