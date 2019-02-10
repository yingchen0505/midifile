#include "MusicSegmentManager.h"

using namespace music_segment_manager;

MusicSegmentManager::MusicSegmentManager(string inputFolderPath) {
	// Store current directory so as to return to this before exiting constructor
	char homeDirectory[1024];
	getcwd(homeDirectory, sizeof(homeDirectory));

	if ( !exists( inputFolderPath ) ) {
		return;
	}

	directory_iterator end_itr; // default construction yields past-the-end
	
	for ( directory_iterator itr( inputFolderPath ); itr != end_itr; ++itr ) {
		if ( is_directory(itr->status()) ) {
			if(itr->path().leaf().string() == "music_segments") {
				// Loop through all emotion folders in music_segments folder
				for (directory_iterator emotionFolderItr( itr->path() ); emotionFolderItr != end_itr; ++emotionFolderItr) {
					string emotionFolderName = emotionFolderItr->path().leaf().string();
					cout << "emotionFolderName = " << emotionFolderName << "\n";
					
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

					cout << "valence = " << valence << "\n";
					cout << "arousal = " << arousal << "\n";
					
					vector<MusicSegment> tempList;
					
					// Loop through music segments within this emotion
					for (directory_iterator musicSegmentItr( emotionFolderItr->path() ); musicSegmentItr != end_itr; ++musicSegmentItr) {
						MidiFile infile(musicSegmentItr->path().string().c_str());
						//std::ofstream outfile; // without std::, reference would be ambiguous because of Boost
						string infileName = musicSegmentItr->path().leaf().string();
						//string outfileName = emotionFolderName + "_" + infileName;
						//outfile.open(outfileName.c_str());
						//infile.write(outfile);
						//outfile.close();
						
						searchString = infileName;
						regex intRegex("[[:d:]]+");
						
						// Read file number
						regex_search(searchString, numberFound, intRegex);
						int fileNumber = stoi(numberFound[0]) - 1; // vector starts from 0 instead of 1, therefore minus 1
						searchString = numberFound.suffix();
												
						// Read part number
						regex_search(searchString, numberFound, intRegex);
						int partNumber = stoi(numberFound[0]);
						
						if(fileNumber >= tempList.size()) {
							for(int i=tempList.size() - 1; i<fileNumber; i++) {
								MusicSegment musicSegment;
								musicSegment.valence = valence;
								musicSegment.arousal = arousal;
								tempList.push_back(musicSegment);
							}
						}

						switch(partNumber) {
							case 1:
								tempList[fileNumber].prep = &infile;
								break;
							
							case 2:
								tempList[fileNumber].mainLoop = &infile;
								break;
							
							case 3:
								tempList[fileNumber].mainLoopEnd = &infile;
								break;
							
							case 4:
								tempList[fileNumber].finalEnd = &infile;
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
	
	for(int i=0; i<musicSegmentList.size(); i++){
		//musicSegmentList[i].repeat(30, true, true);
		//MidiFile midiFile = musicSegmentList[i].repeat(30, true, true);
		cout << musicSegmentList[i].isInvalid() << "\n";
		/*
		std::ofstream outfile; // without std::, reference would be ambiguous because of Boost
		outfile.open(to_string(i).c_str());
		midiFile.write(outfile);
		outfile.close();*/
	}
	
	// Change to input directory
	//chdir(inputFolderPath.c_str());

	//MusicSegment musicSegment();
	//musicSegmentList.push_back(musicSegment);

	chdir(homeDirectory);
	
	char currDirectory[1024];
	getcwd(currDirectory, sizeof(currDirectory));
	std::cout << currDirectory << "\n";
}