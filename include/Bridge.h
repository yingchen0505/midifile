#include <iostream>
#include <string>
#include "MidiFile.h"

using namespace smf;

namespace bridge {
	class Bridge {
		public:
			Bridge() = default;
			Bridge(std::string ID, MidiFile bridgeMidi);
			
			std::string ID;
			MidiFile bridgeMidi;
		
		private:
			
	};
}