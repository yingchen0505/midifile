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
		regex bridgeIDRegex("(\\-)?[[:d:]]+_(\\-)?[[:d:]]+_(\\-)?[[:d:]]+_(\\-)?[[:d:]]+_(\\-)?[[:d:]]+_(\\-)?[[:d:]]+");
		smatch substringFound;  
		regex_search(bridgeFileName, substringFound, bridgeIDRegex);
		string bridgeID = substringFound[0];
		cout << "bridgeID = " << bridgeID << "\n";
		//searchString = numberFound.suffix();
/*
		// Read previous music segment valence
		regex_search(searchString, numberFound, signedIntRegex);
		int prevValence = stoi(numberFound[0]);
		searchString = numberFound.suffix();
		
		// Read previous music segment arousal
		regex_search(searchString, numberFound, signedIntRegex);
		int prevArousal = stoi(numberFound[0]);
		*/
		MidiFile bridgeMidi(itr->path().string());
		Bridge bridge(bridgeID, bridgeMidi);
		bridgeMap[bridgeID] = bridge;

	}
}

MidiFile BridgeManager::getBridge(MusicSegment prevSegment, MusicSegment nextSegment) {
	string bridgeID = to_string(prevSegment.valence) + "_" + to_string(prevSegment.arousal) + "_" + to_string(prevSegment.ID) + "_" +
						to_string(nextSegment.valence) + "_" + to_string(nextSegment.arousal) + "_" + to_string(nextSegment.ID);
	if(bridgeMap[bridgeID].isInvalid()) {
		cout << bridgeID << "NOT FOUND \n";
	}
	else {
		cout << bridgeID << "FOUND \n";
	}
	return MidiFile();
}
		