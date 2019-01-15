#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include "MidiCat.h"
#include "MidiExcerptByBar.h"

using namespace midi_excerpt_by_bar;
using namespace smf;

int main(int argc, char* argv[]) {
	MidiExcerptByBar midiExcerptByBar;
	midiExcerptByBar.run(argc, argv);
}
