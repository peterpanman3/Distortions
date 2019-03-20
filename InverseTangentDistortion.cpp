#include <Bela.h>
#include <algorithm>
#include <cmath>
#include <Scope.h>

int gAudioChannelNum; // number of audio channels to iterate over
int gAnalogChannelNum; // number of analog channels to iterate over
int gainKnob = 0;
int rangeKnob = 1;
int blendKnob = 2;
int volumeKnob = 3;
int gAudioFramesPerAnalogFrame = 0;
float gInverseSampleRate = 0.0;
float gPhase = 0.0;
float gain = 1.0;
float range = 1.0;
float blend = 1.0;
float volume = 1.0;
float output = 0.0;
float distSig = 0.0;
float cleanSig = 0.0;
float x1 = 0.0;
float x = 0.0;
float y = 0.0;

Scope scope;


bool setup(BelaContext *context, void *userData)
{

	gAudioFramesPerAnalogFrame = context->audioFrames / context->analogFrames;
	gInverseSampleRate = 1.0 / context->audioSampleRate;
	gPhase = 0.0;
	scope.setup(2,context->audioSampleRate);

	return true;
}

void render(BelaContext *context, void *userData)
{
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		if(gAudioFramesPerAnalogFrame && !(n % gAudioFramesPerAnalogFrame)) {
			range = map(analogRead(context, n/gAudioFramesPerAnalogFrame, rangeKnob), 0, 1, 0, 20);
			gain = map(analogRead(context, n/gAudioFramesPerAnalogFrame, gainKnob), 0, 1, .1, 10);
			blend = analogRead(context, n/gAudioFramesPerAnalogFrame, blendKnob);
			volume = map(analogRead(context, n/gAudioFramesPerAnalogFrame, volumeKnob), 0, 1, 0, 1);
		}
			x = range*audioRead(context, n, 0)+.01;
			x1 = range*audioRead(context, n, 0);
			if (gain*x<=-1)
				y = -2/3;
			else if (gain*x>-1 && gain*x<1)
				y = (gain*x) - ((1/3) * (gain*x*gain*x*gain*x));
			else if (gain*x>=1)
				y = 2/3;
			output = y/gain;
			scope.log(x1,output);
			audioWrite(context, n, 0, output);
	}
}

void cleanup(BelaContext *context, void *userData)
{

}