# AudioVisualizer
C++ audio visualizer made with OpenGL and FMOD Ex. 

This application is made by Kenze Kua as a student, and is non-commercial. 
If there are any queries or questions, you may contact the owner of this repository or via kenzekua+github@gmail.com

Main music used: supapao - HEAVYLIGHT
Please ignore the other mp3 files as they are only used for educational purposes.

== Instructions == 
• Connect the linkers to the SDKs: gles2, glfw 3.3, and FmodEx  
• Main programming will be in main.cpp 
• triangle_demo.h handles most of the shapes being displayed and the FMOD calculations

== Division of Sound Frequency == 
1 array = 43.06640625 Hz (22 050 Hz / 512 sample size) 
Note of B in each octave in brackets 
Octave 0 (30.86771) = [0] 
Octave 1 (61.73541) = [1] 
Octave 2 (123.4708) = [2] 
Octave 3 (246.9417) = [3] - [5] 
Octave 4 (493.8833) = [6] - [13] 
Octave 5 (987.7666) = [14] - [24] 
Octave 6 (1975.533) = [25] - [47] 
Octave 7 (3951.066) = [48] - [91] 
Octave 8 (7902.133) = [92] - [184] 
 
== Notes on the sound frequency == 
Not all frequencies were used for this audio visualizer. I based it around 1 base note on the maximum piano frequency range. 
How I calculated this audio visualizer is by using a spectrum array size of 512. 
This can be higher but requires adjustment to the bulk of the programmingg since it was customize for size 512. 
After using FMOD to collect the frequencies, I later divided it into 8 octaves of sounds. 
The octaves ranges between 30 Hz to 7900 Hz. The 8 octaves are then divided between 18 bars and averaged out for certain bars. 
This creates a smooth transition between the bars allowing it to give a smoother shape. 
