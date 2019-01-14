#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include "MidiCat.h"

using namespace midicat;
using namespace smf;

// user interface variables


int main(int argc, char* argv[]) {
	
	Options options;
	options.process(argc, argv);
	const char* filename = options.getArg(1).c_str();
	MidiFile infile(filename);
	infile.joinTracks();
	int eventCount = infile.getEventCount(0);

	for (int i=0; i<eventCount; i++) {
		if(infile.getEvent(0,i).bar != 0){
			std::cout << infile.getEvent(0,i).bar << "\n";
		}
	}
	
	return 0;
}