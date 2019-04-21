#include "MusicSegmentManager.h"

using namespace music_segment_manager;

MusicSegmentManager::MusicSegmentManager(string inputFolderPath) {
	if ( !exists( inputFolderPath ) ) {
		return;
	}

	directory_iterator end_itr; // default construction yields past-the-end
	
	for ( directory_iterator itr( inputFolderPath ); itr != end_itr; ++itr ) {
		if ( is_directory(itr->status()) ) {
			
			if(itr->path().leaf().string() == "bridges") {
				bridgeManager = BridgeManager(itr->path().string());
			}
			
			else if(itr->path().leaf().string() == "music_segments") {
				// Loop through all emotion folders in music_segments folder
				for (directory_iterator emotionFolderItr( itr->path() ); emotionFolderItr != end_itr; ++emotionFolderItr) {
					
					string emotionFolderName = emotionFolderItr->path().leaf().string();

					string searchString = emotionFolderName;
					regex signedIntRegex("(\\-)?[[:d:]]+");
					smatch numberFound;  

					// Read valence
					regex_search(searchString, numberFound, signedIntRegex);
					int valence = stoi(numberFound[0]);
					searchString = numberFound.suffix();
					
					// Read arousal
					regex_search(searchString, numberFound, signedIntRegex);
					int arousal = stoi(numberFound[0]);

					vector<MusicSegment> tempList;
					
					// Loop through music segments within this emotion
					for (directory_iterator musicSegmentItr( emotionFolderItr->path() ); musicSegmentItr != end_itr; ++musicSegmentItr) {
						MidiFile infile(musicSegmentItr->path().string().c_str());
						string infileName = musicSegmentItr->path().leaf().string();
						
						searchString = infileName;
						regex intRegex("[[:d:]]+");
						
						// Read file number
						regex_search(searchString, numberFound, intRegex);
						int fileNumber = stoi(numberFound[0]) - 1; // vector starts from 0 instead of 1, therefore minus 1
						searchString = numberFound.suffix();
												
						// Read part number
						regex_search(searchString, numberFound, intRegex);
						int partNumber = stoi(numberFound[0]);
						
						// Construct new MusicSegment if needed (i.e. if fileNumber is out of bound)
						if(fileNumber >= tempList.size()) {
							for(int i=tempList.size() - 1; i<fileNumber; i++) {
								MusicSegment musicSegment;
								musicSegment.valence = valence;
								musicSegment.arousal = arousal;
								musicSegment.ID = fileNumber + 1; // ID starts from 1, while fileNumber starts from 0
								tempList.push_back(musicSegment);
							}
						}
						switch(partNumber) {
							case 1:
								tempList[fileNumber].prep = new MidiFile(infile);
								break;
							
							case 2:
								tempList[fileNumber].mainLoop = new MidiFile(infile);
								break;
							
							case 3:
								tempList[fileNumber].mainLoopEnd = new MidiFile(infile);
								break;
							
							case 4:
								tempList[fileNumber].finalEnd = new MidiFile(infile);
								break;
						}
					
					}
					
					// Add all valid music segments from this emotion to the overall list
					for (int i=0; i<tempList.size(); i++) {
						if(!(tempList[i].isInvalid())) {
							musicSegmentList.push_back(tempList[i]);
						}
					}
				}
			}
		}
	}	
}

void MusicSegmentManager::generateMusicFromEmotion(vector<EmotionState> emotionSequence) {
	
	MidiCat midiCat;
	EmotionState currEmotion = emotionSequence.at(0);	
	MusicSegment currMusic = getMusicSegmentByEmotion(currEmotion.valence, currEmotion.arousal);
	int currTransposition = 0;

	for(int i=0; i<emotionSequence.size() - 1; i++) {
		EmotionState nextEmotion = emotionSequence.at(i+1);		
		int currDuration = nextEmotion.sec - currEmotion.sec;
		
		MusicSegment nextMusic = getMusicSegmentByEmotion(nextEmotion.valence, nextEmotion.arousal);

		Bridge bridge = bridgeManager.getBridge(currMusic, nextMusic);
		if(!bridge.isInvalid()) {

			vector<MidiFile> catList;
			catList.push_back(musicSegmentList[i].repeat(30, 0, bridge.barErosionIntoPrevSeg, bridge.prevTransposition));
			catList.push_back(bridge.bridgeMidi);
			catList.push_back(musicSegmentList[j].repeat(30, bridge.barErosionIntoNextSeg, 0, bridge.nextTransposition));
			
			MidiFile midiFile = midiCat.run(catList, 0.0);
			std::ofstream outfile; // without std::, reference would be ambiguous because of Boost
			outfile.open((to_string(i) + to_string(j) + ".mid").c_str());
			midiFile.write(outfile);
			outfile.close();
		}

	}
	
	
	
	
	
	// Testing repeat function for all segments
	// for(int i=0; i<musicSegmentList.size(); i++){
		
		// for(int j=0; j<musicSegmentList.size(); j++){
			// if(i==j) continue;
			// std::cout << "i = " << i << " j = " << j << "\n";
			// Bridge bridge = bridgeManager.getBridge(musicSegmentList[i], musicSegmentList[j]);
			// if(!bridge.isInvalid()) {

				// vector<MidiFile> catList;
				// catList.push_back(musicSegmentList[i].repeat(30, 0, bridge.barErosionIntoPrevSeg, bridge.prevTransposition));
				// catList.push_back(bridge.bridgeMidi);
				// catList.push_back(musicSegmentList[j].repeat(30, bridge.barErosionIntoNextSeg, 0, bridge.nextTransposition));
				
				// MidiFile midiFile = midiCat.run(catList, 0.0);
				// std::ofstream outfile; // without std::, reference would be ambiguous because of Boost
				// outfile.open((to_string(i) + to_string(j) + ".mid").c_str());
				// midiFile.write(outfile);
				// outfile.close();
			// }
		// }
	// }	
}

MusicSegment MusicSegmentManager::getMusicSegmentByEmotion(int valence, int arousal) {
	double currDistance = INT_MAX;
	MusicSegment result;
	for(int i=0; i<musicSegmentList.size(); i++){
		MusicSegment musicSegment = musicSegmentList.at(i);
		double distance = sqrt(pow(musicSegment.valence - valence, 2) + pow(musicSegment.arousal - arousal, 2));
		if(currDistance > distance) {
			currDistance = distance;
			result = musicSegment;
		}
	}
	return result;
}