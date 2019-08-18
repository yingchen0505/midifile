This repo is the code base for the Final Year Project **"Conversion from Pictures to Music: Generating Music from Emotions"**

Developed based on Craig Sapp's [Midifile library](https://github.com/craigsapp/midifile).


The following changes have been made to Craig's libary:

#### 1. Bar information is added to MidiFile class to keep track of which bar it is throughout a midi file
The length of a bar is calculated from the time signature and "ticks per quarternote" of the midi file.   

Changes in time signature in the middle of a midi file are accommodated as well.  

Bar number starts from 1.  

##### The following functions are added:

* int getBarByTick (int tickvalue)  
Returns the bar number for a given tick number.  

* void updateBarNumber (void)  
Calculates the bar numbers for the entire midi file.  
This function is automatically called when the midi file is read from a "\*.mid" file, and when any of the bar-related functions are called if the bar information is outdated.  

* vector<int> getBeginningAndEndTicksByBar (int bar)  
Returns the beginning and ending tick numbers for a given bar.

* int getTotalBars (void)  
Returns the total number of bars in a midi file.  

##### The following properties are added:  

* bool m_barnumbervalid  
Indicates whether the bar numbers are up to date.  

* map<int, vector<int>> m_tickbarmap   
Maps the tick number to the bar number (and the beginning and ending tick numbers of that bar).

* map<int, vector<int>> m_bartickmap   
Maps the bar number to the beginning and ending tick numbers.
	
* int m_totalbars;  
Indicates the total number of bars for a midi file.

#### 2. Added MidiCat (a tool for concatenating Midi files) as part of the library

MidiCat was modified based on the original sample client program MidiCat in Craig's repo.  

The original MidiCat was buggy when it comes to concatenating midifiles with different ticks per quarternote (TPQ) since TPQ of a midifile is immutable.   

The new MidiCat performs the following conversion to ensure the integrity of the tempo after concatenation.   

𝑁𝑒𝑤 𝑇𝑒𝑚𝑝𝑜 = 𝑂𝑟𝑖𝑔𝑖𝑛𝑎𝑙 𝑇𝑒𝑚𝑝𝑜 × 𝑁𝑒𝑤 𝑇𝑖𝑐𝑘𝑠 𝑃𝑒𝑟 𝑄𝑢𝑎𝑟𝑡𝑒𝑟𝑛𝑜𝑡𝑒 ÷ 𝑂𝑟𝑖𝑔𝑖𝑛𝑎𝑙 𝑇𝑖𝑐𝑘𝑠 𝑃𝑒𝑟 𝑄𝑢𝑎𝑟𝑡𝑒𝑟𝑛𝑜𝑡𝑒  

The resultant midifile follows the TPQ of the first input midifile in the input list. Subsequent midifiles have their tempos converted according to the above formula to fit the TPQ of the first midifile.   

#### 3. Added MidiExcerptByBar as part of the library 

A tool to make an excerpt of a midi file given beginning and ending bar numbers (inclusive).  

This tool takes care of the following issues:  

* MIDI events that take place before the selected region (e.g. tempo, channel settings etc.) may nonetheless have an impact on the selected region. Therefore, the excerpting module analyzes the effects of events before the start bar, and integrate them into the selected region when necessary.  

* Some MIDI note-on events might have their corresponding note-off events occurring only after the end of the selected region, but note-on events not turned off would give errors. Therefore, the excerpting module detects such note-on events and turn them off at the end of the selected region.  

* Some MIDI files do not have MIDI events that last through the entire end bar of the selected region. This situation might cause an incomplete end bar in the output. Therefore, the excerpting module inserts a non-disruptive filler event to fill the output MIDI file to the total length of the intended end bar.  

#### 4. Classes used by the client program fyp.cpp, including:

* Bridge  
The bridging segment between two music segments.   
It contains information about the previous and next music segments, such as their identity, how many bars are to be cut away when the bridging happens, and transposition (if required).  

* BridgeManager  
The wrapper to generate a bridge between two music segments, and keep track of a map of the bridges already generated.   

* EmotionState  
This class contains the start time, end time, valence and arousal values of the emotion.   

* MusicSegment  
The music segment is a unit used in the generation of music.   
Each music segment can be repeated infinitely to suit the time span required.   
To achieve the repetition, each music segment consists of four midifiles - pick-up bars, start phrase, end phrase, and conclusion.
See FYP report for the details of valid repetitions based on the four midifiles in a music segment.   

Each music segment also contains the valence and arousal values corresponding to the emotion conveyed by this music segment, as well as the transposition value.   

* MusicSegmentManager  
The manager generates an output midifile that corresponds to a given list of emotion states.  

Downloading
-----------

You can download as a ZIP file from the Github page,
or if you use git, then download with this command:

``` bash
git clone https://github.com/yingchen0505/midifile.git
```

This will create the `midifile` directory with the source code for the library.


Compiling with GCC
------------------

In Linux, the library can be built with the command:
``` bash
cmake .
```

Build files will be written to the folder `CMakeFiles`.

The specific client program fyp.cpp can be compiled with the following command:
``` bash
make fyp
```
  
This will generate an executable program fyp, which be run with the following command:
``` bash
./fyp [options] [arguments]
```

Run with Sample Data
------------------

In the `input` folder, there are a set of sample music segments, a set of (manually composed) bridges, and a file `emotion_sequence.txt`.  

#### Sample Music Segments
Each subfolder under the `music_segments` folder has its name in the format `[valence]_[arousal]` that represents its emotion value.  
Each valence/arousal value is on the scale of -10 to 10.  
For each midifile in the subfolder, the file name is in the format `[music segment number]_[part number]`.
For example, the pick-up bars (i.e. part 1) of the 1st music segment in emotion X has the file name `1_1`.
The start phrase (i.e. part 2) of the 3rd music segment in emotion X has the file name `3_2`.

#### Sample Bridges
The sample bridges file names are in the format `[v1]_[a1]_[s1]_[v2]_[a2]_[s2]_[e1]_[e2]`, where:  
* `v1` is the valence value of the previous music segment  
* `a1` is the arousal value of the previous music segment  
* `s1` is the segment number of the previous music segment in its emotion subfolder  
* `v2` is the valence value of the next music segment  
* `a2` is the arousal value of the next music segment  
* `s2` is the segment number of the next music segment in its emotion subfolder  
* `e1` is the number of end bars to be eroded from the previous music segment when the bridging is applied
* `e2` is the number of beginning bars to be eroded from the next music segment when the bridging is applied

Note that the 4 sample bridges are manually composed (a relic from earlier stages of the project).   
When the actual project is run, it generates automatically composed bridges that are used in the output music, but not stored in this folder.   

#### Sample Emotion Sequence
Each row in the file `emotion_sequence.txt` represents an emotion state.   
The first integer is the minute.   
Second integer is the seconds.   
Third integer is the valence value.   
Fourth integer is the arousal value.   

The sample emotion sequence is for the video [*La Luna*](https://youtu.be/vbuq7w3ZDUQ), a short film by Pixar.


To generate output music from sample data:
``` bash
./fyp
```

In root folder, you would find two midifiles generated.   
`output.mid` is the output midifile.   
`no_transition.mid` is the "raw" output without executing the transition algorithm for the purpose of comparison.   

