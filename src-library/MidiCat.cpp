//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>, Modified by Yingchen Lai <lai.yingchen@u.nus.edu>
// Creation Date: Tue Oct 16 07:34:30 PDT 2012
// Last Modified: 
// Filename:      ...sig/examples/all/midicat.cpp
// Web Address:   http://museinfo.sapp.org/examples/museinfo/midi/midicat.cpp
// Syntax:        C++; museinfo
//
// Description:   Concatenate multiple MIDI files into single type-0 MIDI file.
//

#include "MidiCat.h"
using namespace midicat;

MidiFile MidiCat::run(vector<MidiFile> inputFiles, double pause) {
   MidiFile outfile;
   outfile.joinTracks();
   outfile.deltaTicks();

   int i;
   int initQ = 0;
   
   for(int i=0; i<inputFiles.size(); i++){
	   appendMidi(outfile, inputFiles[i], pause, initQ++);
   }

   // insert an end-of track Meta Event
   int tpq = outfile.getTicksPerQuarterNote();
   MidiEvent mfevent;
   mfevent.tick = 0;
   mfevent.track = 0;
   mfevent.resize(3);
   mfevent[0] = 0xff;
   mfevent[1] = 0x2f;
   mfevent[2] = 0;
   outfile.addEvent(mfevent);
	
	return outfile;
}


//////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// correctTempo -- Corrects the problem of tempo distortion due to difference in tpq of the two midi files being concatenated. 
// 				   Returns the new tempo which = oldTempo * newTpq / oldTpq
//

int MidiCat::correctTempo(int oldTempo, int oldTpq, int newTpq) {
	double factor = (double)newTpq / (double)oldTpq;
	double newTempo = (double) oldTempo * factor;
	return newTempo;
}


//////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// appendMidi --
//

void MidiCat::appendMidi(MidiFile& outfile, MidiFile infile,
      double pause, int initQ) {
   infile.joinTracks();
   infile.deltaTicks();
   int i;
   int tpq;
   int count;

   MidiEvent anevent;
   if (initQ == 0) {
      outfile.joinTracks();
      count = infile.getEventCount(0);
      // don't include end-of-track meta event
      count--;
      tpq = infile.getTicksPerQuarterNote();
      outfile.setTicksPerQuarterNote(tpq);
      for (i=0; i<count; i++) {
         anevent = infile.getEvent(0,i);
         // storing as a type-0 file, so remove track information
         anevent.track = 0;
         outfile.addEvent(anevent);
      }
	  return;
   }

   // presuming constant tpq for different files.
   tpq = outfile.getTicksPerQuarterNote();

   if (pause > 0.0) {
      // insert a tempo marking of 120, and then a pause related to how
      // long in second to wait until next MIDI file contents.
      // micro-seconds per quarter note is last three bytes of meta message
      // If quarter note is 120 bpm, that is 0.5 seconds or 500000 usec.
      // In hex 500000 is 07 a1 20
      // Tempo meta event:  ff 51 03 07 a1 20
      vector<uchar> bpm120;
      bpm120.resize(3);
      bpm120[0] = 0x07;
      bpm120[1] = 0xa1;
      bpm120[2] = 0x20;

      outfile.addMetaEvent(0, 0, 0x51, bpm120);
      infile.getEvent(0,0).tick = int(pause * 2 * tpq + 0.5);
   }

   int tpqInfile = infile.getTicksPerQuarterNote();
   count = infile.getEventCount(0);
   // don't include end-of-track meta event
   count--;
   for (i=0; i<count; i++) {
      anevent = infile.getEvent(0,i);
      anevent.track = 0;
	  if (anevent.isTempo()) {
		  int oldTempo = anevent.getTempoMicroseconds();
		  int newTempo = correctTempo(oldTempo, tpqInfile, tpq);
		  anevent.setTempoMicroseconds(newTempo);
	  }
      outfile.addEvent(anevent);
   }
}

//////////////////////////////
//
// example --
//

void MidiCat::example(void) {

}



//////////////////////////////
//
// usage --
//

void MidiCat::usage(const char* command) {

}



