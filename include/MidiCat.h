#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace smf;

namespace midicat {
	
	class MidiCat {
		public:
			
			// user interface variables
			Options options;
			double seconds         = 2.0;  // used with -p option
			int    binaryQ         = 1;    // used with -a option

			// function declarations:
			MidiCat() = default;
			~MidiCat() = default;
			void      checkOptions      (Options& opts, int argc, char** argv);
			void      example           (void);
			void      usage             (const char* command);
			void      appendMidi        (MidiFile& outfile, const char* filename,
										 double seconds, int initQ);
			int 	  correctTempo		(int oldTempo, int oldTpq, int newTpq);
			void 	  run				(int argc, char* argv[]);
			
		private:
		
	};
	
}