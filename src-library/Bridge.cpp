#include "Bridge.h"

using namespace bridge;

Bridge::Bridge(string ID, MidiFile bridgeMidi, int barErosion) {
	this->ID = ID;
	this-> bridgeMidi = bridgeMidi;
	this->valid = true;
	this->barErosion = barErosion;
}

bool Bridge::isInvalid() {
	return !valid;
}