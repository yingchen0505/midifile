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
					
					// Read music segments within this emotion
					for (directory_iterator musicSegmentItr( emotionFolderItr->path() ); musicSegmentItr != end_itr; ++musicSegmentItr) {
						MidiFile infile(musicSegmentItr->path().string().c_str());
						std::ofstream outfile; // without std::, reference would be ambiguous because of Boost
						string infileName = musicSegmentItr->path().leaf().string();
						string outfileName = emotionFolderName + "_" + infileName;
						outfile.open(outfileName.c_str());
						infile.write(outfile);
						outfile.close();
					}
					/*
					cout << "musicSegmentItr->path().string() = " << musicSegmentItr->path().string() << "\n";
					chdir(musicSegmentItr->path().string().c_str());
					char currDirectory[1024];
					getcwd(currDirectory, sizeof(currDirectory));
					cout << currDirectory << "\n";*/
				}
			}
		}
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