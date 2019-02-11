#include "BridgeManager.h"

using namespace bridge_manager;

BridgeManager::BridgeManager(string bridgeFolderPath) {
	if ( !exists( bridgeFolderPath ) ) {
		return;
	}

	directory_iterator end_itr; // default construction yields past-the-end
	
	for ( directory_iterator itr( bridgeFolderPath ); itr != end_itr; ++itr ) {
		string bridgeFileName = itr->path().leaf().string();

		string searchString = bridgeFileName;
		regex signedIntRegex("(\\-)?[[:d:]]+");
		smatch numberFound;  

		// Read previous music segment valence
		regex_search(searchString, numberFound, signedIntRegex);
		int prevValence = stoi(numberFound[0]);
		searchString = numberFound.suffix();
		
		// Read previous music segment arousal
		regex_search(searchString, numberFound, signedIntRegex);
		int prevArousal = stoi(numberFound[0]);
		
		MidiFile bridgeMidi(itr->path().string());
		Bridge bridge(bridgeFileName, bridgeMidi);

	}
}

MidiFile BridgeManager::getBridge(MusicSegment prevSegment, MusicSegment nextSegment) {
	string bridgeID = to_string(prevSegment.valence) + to_string(prevSegment.arousal) + to_string(prevSegment.ID) +
						to_string(nextSegment.valence) + to_string(nextSegment.arousal) + to_string(nextSegment.ID);
	if(bridgeMap[bridgeID].isInvalid()) {
		cout << "HERE \n";
	}
	return MidiFile();
}
		