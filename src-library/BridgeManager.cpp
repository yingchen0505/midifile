#include "BridgeManager.h"

using namespace bridge_manager;

BridgeManager::BridgeManager(string bridgeFolderPath) {
	if ( !exists( bridgeFolderPath ) ) {
		return;
	}

	directory_iterator end_itr; // default construction yields past-the-end
	
	// Loop through the bridges folder to add bridges to the map
	for ( directory_iterator itr( bridgeFolderPath ); itr != end_itr; ++itr ) {
		string bridgeFileName = itr->path().leaf().string();

		string searchString = bridgeFileName;
		
		/// Read bridge ID:
		
		// The format is basically six integers separated by "_"
		regex bridgeIDRegex("(\\-)?[[:d:]]+_(\\-)?[[:d:]]+_(\\-)?[[:d:]]+_(\\-)?[[:d:]]+_(\\-)?[[:d:]]+_(\\-)?[[:d:]]+");
		
		smatch substringFound;  
		regex_search(searchString, substringFound, bridgeIDRegex);
		string bridgeID = substringFound[0];
		searchString = substringFound.suffix();
		
		/// Read bar erosion number:
		regex intRegex("[[:d:]]+");
		regex_search(searchString, substringFound, intRegex);
		int barErosion = stoi(substringFound[0]);
		
		// Constructing bridge and adding it to map
		MidiFile bridgeMidi(itr->path().string());
		Bridge bridge(bridgeID, bridgeMidi, barErosion);
		bridgeMap[bridgeID] = bridge;
	}
}

Bridge BridgeManager::getBridge(MusicSegment prevSegment, MusicSegment nextSegment) {
	string bridgeID = to_string(prevSegment.valence) + "_" + to_string(prevSegment.arousal) + "_" + to_string(prevSegment.ID) + "_" +
						to_string(nextSegment.valence) + "_" + to_string(nextSegment.arousal) + "_" + to_string(nextSegment.ID);
	if(bridgeMap[bridgeID].isInvalid()) {
		// TODO: construct new bridge
	}
	else {
		return bridgeMap[bridgeID];
	}
	return Bridge();
}
		