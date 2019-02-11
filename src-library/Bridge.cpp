#include "Bridge.h"

using namespace bridge;

Bridge::Bridge(string ID, MidiFile bridgeMidi) {
	this->ID = ID;
	this-> bridgeMidi = bridgeMidi;
	this->valid = true;
}

bool Bridge::isInvalid() {
	return !valid;
}