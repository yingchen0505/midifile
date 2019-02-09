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
				// Loop through all emotion folders in music_segments folder
				for (directory_iterator musicSegmentItr( itr->path() ); musicSegmentItr != end_itr; ++musicSegmentItr) {
					std::cout << musicSegmentItr->path().leaf().string() << "\n";
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