#include "MusicSegmentManager.h"

using namespace music_segment_manager;

MusicSegmentManager::MusicSegmentManager(string inputFolderPath) {
	// Store current directory so as to return to this before exiting constructor
	char homeDirectory[1024];
	getcwd(homeDirectory, sizeof(homeDirectory));
	
	char currDirectory[1024];
	
	// Change to image input directory
	chdir(inputFolderPath.c_str());
	
	getcwd(currDirectory, sizeof(currDirectory));
	std::cout << currDirectory << "\n";
	
	chdir(homeDirectory);
	
	getcwd(currDirectory, sizeof(currDirectory));
	std::cout << currDirectory << "\n";
}