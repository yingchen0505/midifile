#include "MidiFile.h"
#include <list>

using namespace std;
using namespace smf;

namespace midicat {
	
	class MidiCat {
		public:
			
			// function declarations:
			MidiCat() = default;
			~MidiCat() = default;
			void      example           (void);
			void      usage             (const char* command);
			void      appendMidi        (MidiFile& outfile, const char* filename,
										 double pause, int initQ);
			int 	  correctTempo		(int oldTempo, int oldTpq, int newTpq);
			MidiFile 	  run			(list<const char*> inputFileNames, double pause);
			
		private:
		
	};
	
}