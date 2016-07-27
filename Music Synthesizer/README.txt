A title of selection : Sparty Wars



Group Members :
    
	Alexandria Marone
    
	Abigael Onchiri
    
	Alex Zajac
    
	Cameron Merrill
    
	Glen Simon

Score file used for our selection : FINALSCORE.score 
Audio file for reference: FINALSCORE.wav
website URL: http://cse.msu.edu/~maroneal/
If you are having issues on generate with piano files, go to project properties->configuration properties->debugging change working directory to $(SolutionDir)


Format of your score files.


Individual component score files:

	Piano : piano.score 
	Additive: AdditiveInstrument.score
	Subtractive: subrtractiveexample.score
	Recorded performace synthesizer: RecordedSynthNoiseGate.score && RecordedSynthCompression.score
	Effects : Effect - Reverberation.score && Effect - Chorus.score && Effect - Flanging.score && Effect - Ring.score
	
       
	Piano Synthesizer :

       	         
		<instrument instrument="Piano">
	       	    		
			<note measure="1" beat="1" duration="1.9"note="C4" dynamics="0.8"/>
       		    
		</instrument>
	     		

	Additive Synthesizer:

		
		<instrument instrument="AdditiveInstrument" >

			<note measure="1" beat="4" duration="0.33"  crossFadeIn=".15" crossFadeOut=".15" amplitudes="1 .8 .6 " note="D4"/>
		</instrument>
	     

	Subtractive Synthesizer:

		<instrument instrument="SubtractiveInstrument">




			<note measure="10" beat="1" duration="1" ADSR=".1 0 1.0 .9" wave = "square" note="C4"/>
		</instrument>

	
	Recorded performance synthesizer:

		<instrument instrument="RecordedSynth">
			<note measure="27" beat="1" filename="audio/something.mp3" effect="NoiseGate" duration="49" ></note>
		
</instrument>

	Effects Component:

		(ADD Effects SCORE FILE FORMAT HERE)
	



How it all Works:

 Under the file menu option, select load score, open the 
score file (FINALSCORE.score). Once the score file is loaded go 
to the
 Generate menu option and select synthesizer. The selection will then 
begin to play.




How each component was implemented:


Piano Synthesizer implementation :

 Abigael Onchiri 


	Polyphony

    	Once the instrument is determined as a Piano, the piano factory class creates a piano instrument object,
        it then calls set note that looks at the note, the duration and the dynamics and loads in the specific note
        file from the CompletePiano notes folder and plays the note for a certain amount of time based on duration. 
        When the measure is complete it moves to a new note. I used the value of the note on the score file to determine
        which note file to open and then used string parsing to determine advanced dynamics ie. whether the piano note 
	should be played loudly or softly. Polyphony was achieved by reading in multiple notes at a time and adding them to
	the pianowave.
	
    
	Envelopes

    	In order to simulate what happens when a key is released I checked the current position and if it had reached the 
        end of the wave stored wave I let it play until the end in order to simulate a dampening rather than just a cutoff.
    
	Pedal Simulation

    	I had a boolean called "pedal_pressed" that is set to true or false depending on the audio file that is open "pedald.wav"
        was the pedal down audio file so I set "pedal_pressed" to true and "pedalu.wav" was the pedal up audio file so I set "pedal-pressed" 
        to false. If the pedal was pressed, I set the current gain to 1 and if the pedal was not pressed I set the current gain to duration 
        subtracted by the current time and divided that value by (duration - 0.05 (which was the value of seconds per beat))
    	
	Dynamics (basic)

    	I simulated basic dynamics by seeing if the note tag had a dynamics value and then set that value to the current amplitude.
    
	Pedal Noise

    	The CompletePiano sample file that was provided had piano notes that simulated pedal noise.

    	Dynamics (advanced)

    	I used string parsing to parse the filename and see if the character before ".wav" was an "l" simulating loud or 
        an "s" simulating soft, if it was loud, I did current sample * 4 plus 1 for frame[0] and current sample * 4 + 2 plus 1 to frame[1].
        If it was soft I just did the current sample * 4 and set that to the frame[0] and current sample * 4 + 2 and set that to frame[1] 
        and did not add an additional sample.

	Grading Elements Supported

	+10 - Playing piano notes
	+20 - Envelope generation
	+30 - Pedal Simulation
	+35 - Basic dynamics
	+40 - Pedal noise
	+50 - Advanced dynamics 


 

Additive Sythesizer Implementation: Cameron Merrill
 

	The synthesizer implementation of Additive Synthesis implements sound generation by adding together sinusoids and includes the 
	following properties:

    	Polyphony
    	
	Read in all the notes in XML, parse through them, and add them in a wavetable to provide for multiple playing of simultaenously notes. This is handled
	in the AdditiveSinewave wavetable class as it reads through the XML.
	
    	Sound Coss-Fading

    	Cross-fading works off the attack and release of the note, as it interlpolates between them - by crossfading between notes, we can fade out of one 
	and into another based on the attack and release of those notes. This is handled in the AdditiveInstrument class, where we work with the current time 
    	
	Envelope Generation

    	Instruments start and sustain a sound in varying ways utilizing volume. Instruments have a volume for the attack, followed by a 
	rapid decay, a sustain at ad ifferent volume level, and a gradual decay at the end. This allows for avoiding an abrupt start and
	stop of a sound. ADSR envelopes employ an envelope generator, which handles Attack Time, Decay Time, Sustain Level, and Release 
	Level for a sound. This is implemented by passing values for ADSR to the instrument and tracking the time as the note is played. 
	We then make adjustments to the audio frame based on what time we are at and what values were passed in. This is handled in the 
	implementation under CAdditiveInstrument::Generate().

    	Vibrato
    		- varying the frequency of the note repeatedly up and down.
    		--This is handled when we generate the wave table for our Additive Sinewave - take the rate of vibrato, and mutiplay by
		2pi. Divide that by the Sample Rate, take the Sin(f) of that. Our new freq should be the original freq of the note, 
		plus the vibrato freq multiplied by the sin of the vibrato calcualted freq.This is all implemented utilizing two classes:
		CAdditiveSinewave, and CAdditiveInstrument. From XML, we load a note's measure, beat it occurs on, duration, crossFadeIn and crossFadeOut value, ADSR values (Attack, Delay, Sustain, Release), amplitude values, vibrato frequency and rate, and finally the note. The AdditiveSinewave handles loading the amplitudes into an array, adding them together, and implementing vibrato (if declared). The AdditiveInstrument then handles the ADSR envelopes, and cross fading.
    
	Grading Elements Supported:

	+10 - Sinusoid playback on demand from the sequencer
	+20 - Definition of all harmonics
	+30 - Envelope generation
	+35 - Polyphony
	+40 - Sound cross-fading
	+50 - Vibrato OR glissando 




Subtractive Synthesizer Implementation: Alex Zajac 

	The synthesizer implementation of Subtractive Synthesis implements sound generation by creating sawtooth, square, and triangle 
	waves of the original note. The square wave is all the harmonics added together, the sawtooth is all the odd harmonics, and the
	triangle wave is a summation of the odd harmonics with alternating signs. It also includes the following properties:

    	Polyphony

    	The property of playing more than one note at a time. This is accomplished through reading in multiple notes at a time and 
	adding them to the wavetable.
    
	Envelope Generation
    	
	Many instruments have a rapid decay after the attack, then sustain at another level, only to gradually decay at the end.
	In all cases for a wavetable synthesis component you will need to be able to fade the sound at the end over several milliseconds
	to avoid a "pop". This is accomplished by reading the sample from the wavetable and multiplying by a calculated factor to 
	determine if it is attacking, sustaining, or decaying.
    
	Reson
    	
	A Reson is a simple recursive filter that emphasizes a specific frequency. It's a simple solution for filtering in this 
	application. Reson was accomplished by reading from past wavetable samples and filtering out a frequency with a certain 
	bandwidth.

    	Filter Envelopes
    	
	Interesting filter effects are often created by using the same envelope design used for amplitude envelopes to filter 
	parameters. Sweeping the filter frequency creates the famous "Moog" sound. This was accomplished by using a constant coefficient
	to vary the frequency that is being filtered. The filtered frequency varies between a minimum and maximum value calculated from
	the reson frequency and the coefficent supplied in the score file.
    
	Grading Elements Supported

    	+10 - Waveform playback from tables
    	+20 - Varying pitch playback from tables
    	+30 - Envelope generation
    	+35 - Polyphony
    	+40 - Reson implementation and the Moog sound
    	+50 - Filter envelopes 



Recorded Preformance Synthesizer Implementation:  Alexandria Marone

	This part of the project involved recording your own sound and apply effects to it while using a mp3 or wave format.

   	NoiseGate

    	This section of the project implements the Noise Gate filter as a class. It is instantiated in the Recorded Performance 
	Synthesizer Object. This fulfills the first effect criteria.
    
	Compression
	
    	This section of the project implements the Compression filter as a class. It is also instantiated in the Recorded Performance 
	Synthesizer Object. This fulfills the second effect criteria.
    
	Recording Synthesizer Class
    	
	This class does all the generating and setting notes. It successfully reads the file, stores the sound arrays, and applies 
	filters when asked for.
    
	Special Recording
    	
	Something.mp3 features a homemade song sung by yours truly. With the XML shown above you can apply your filters and hear 
	the output. This fulfills the recording existence criteria. To have recording play by synthesizer, upload score file with 
	recording filename, and select menu item synthesize. The effects are synchronized with other parts with the beat and measure 
	XML showing you where the recording will be played.
    	
	Effects are modified and controlled

   	The effects have modification points with thresholds where the sound is modified. 

	Grading Elements Supported

	+10 - Recording existence
    	+20 - Recording played by synthesizer
    	+30 - Recording synchronized with other parts
    	+35 - 1 Effect
    	+45 - 2 Effects
    	+50 - At least 30 points in the score where the effects are modified/controlled  
     

Effect Component Implementation: Glen Simon

/** Effects Component **/
The four effects that have been implmented are:
-Chorus
-Flanging
-Reverberation
-Ring Modulation with Sinusoid

To use these effects the commands are:
<effect chorus="1" wet=".1" dry="0" />
<effect flanging="1" wet=".1" dry="0" />
<effect reverberation="1" wet=".7" dry=".3" />
<effect ring="1" wet=".4" dry="0.7" />

Where wet represents the strength of the effect and dry represents the additional strength
of the original audio. Note, when using the effects with addictive instruments it is best to use
small wet and dry values.

The "1" after the effect tells the synthesizer to turn that effect on for all subsequent notes
that are being loaded for that instrument. The effect can be turned off before the instrument
ends by using a "0". 
Example: <effect ring="0" />

When a new instrument is added in the XML file, any previous effects are automatically
cleared.

These commands are used at the same level in the XML as notes.

Example of using an effect:
<?xml version="1.0" encoding="utf-8"?>
<score bpm="120" beatspermeasure="4">
	<instrument instrument="AdditiveInstrument" >
		<effect ring="1" wet=".4" dry="0.7" />
		<note measure="1" beat="4" duration="0.33"  crossFadeIn=".15" crossFadeOut=".15" amplitudes="1 .8 .6 " note="D4"/>
		<note measure="1" beat="4.33" duration="0.33"  crossFadeIn=".15" crossFadeOut=".15" amplitudes="1 .8 .6 " note="D4"/>
		<note measure="1" beat="4.66" duration="0.33"  crossFadeIn=".15" crossFadeOut=".15" amplitudes="1 .8 .6 " note="D4"/>
       </instrument>
</score>


There are four premade scores to test the effects components in the "Scores" directory.

/** End Effects Component **/


	Grading Elements Supported

	+10 - Component passes audio
    	+20 - 1 Effect
    	+30 - 3 Effects
    	+30 - Controllable effects send
    	+50 - 4 Effects  
