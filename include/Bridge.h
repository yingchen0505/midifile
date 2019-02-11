#include <iostream>
#include <string>
#include "MidiFile.h"

using namespace smf;
using std::string;

namespace bridge {
	class Bridge {
		public:
			Bridge() {
				valid = false;
			}
			Bridge(string ID, MidiFile bridgeMidi);
			
			bool valid;
			string ID;
			MidiFile bridgeMidi;
			
			bool isInvalid();
		
		private:
			
	};
}