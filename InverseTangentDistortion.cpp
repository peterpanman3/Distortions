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
float blend = 0.0;
float volume = 1.0;
float cleanSig = 0.0;
float distSig = 0.0;
float output = 0.0;

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
			range = map(analogRead(context, n/gAudioFramesPerAnalogFrame, rangeKnob), 0, 1, 0, 100);
			gain = analogRead(context, n/gAudioFramesPerAnalogFrame, gainKnob);
			blend = analogRead(context, n/gAudioFramesPerAnalogFrame, blendKnob);
			volume = analogRead(context, n/gAudioFramesPerAnalogFrame, volumeKnob);
		}
			cleanSig = audioRead(context, n, 0);
			distSig = cleanSig+.005;
			distSig *= gain*range;
			output = (((((2/3.14159)*std::atan(distSig))*blend)+(cleanSig*(1.f-blend)))/2.f)*volume;
			scope.log(3*cleanSig, 3*output);
			audioWrite(context, n, 0, output);
	}
}

void cleanup(BelaContext *context, void *userData)
{

}