#include "MusicSegmentManager.h"

using namespace music_segment_manager;

MusicSegmentManager::MusicSegmentManager(string inputFolderPath) {
	// Store current directory so as to return to this before exiting constructor
	char homeDirectory[1024];
	getcwd(homeDirectory, sizeof(homeDirectory));
	
	char currDirectory[1024];

	if ( !exists( inputFolderPath ) ) {
		return;
	}

	directory_iterator end_itr; // default construction yields past-the-end
	for ( directory_iterator itr( inputFolderPath ); itr != end_itr; ++itr ) {
		if ( is_directory(itr->status()) ) {
			if(itr->path().leaf().string() == "music_segments") {
				int valence;
				int arousal;
				string emotionFolderName;
				regex signedIntRegex("(\\-)?[[:d:]]+");
				smatch numberFound;  
				// Loop through all emotion folders in music_segments folder
				for (directory_iterator musicSegmentItr( itr->path() ); musicSegmentItr != end_itr; ++musicSegmentItr) {
					emotionFolderName = musicSegmentItr->path().leaf().string();
					cout << "emotionFolderName = " << emotionFolderName << "\n";
					
					// Read valence
					regex_search(emotionFolderName, numberFound, signedIntRegex);
					valence = stoi(numberFound[0]);
					emotionFolderName = numberFound.suffix();
					
					// Read arousal
					regex_search(emotionFolderName, numberFound, signedIntRegex);
					arousal = stoi(numberFound[0]);
					
					
					cout << "valence = " << valence << "\n";
					cout << "arousal = " << arousal << "\n";
				}
			}
		}
	}	
	
	// Change to input directory
	//chdir(inputFolderPath.c_str());

	//MusicSegment musicSegment();
	//musicSegmentList.push_back(musicSegment);
	
	getcwd(currDirectory, sizeof(currDirectory));
	std::cout << currDirectory << "\n";
	
	chdir(homeDirectory);
	
	getcwd(currDirectory, sizeof(currDirectory));
	std::cout << currDirectory << "\n";
}