#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <iostream>

#include <sys/types.h>
#include <numeric>
#include <napi.h>
#include "Gist.h"

using namespace std;

class GistNative : public Napi::ObjectWrap<GistNative> {
	public:
		static Napi::Object Init(Napi::Env env, Napi::Object exports);
		GistNative(const Napi::CallbackInfo& info);
		~GistNative();
        Napi::Value setAudioFrameSize(const Napi::CallbackInfo& info);
        Napi::Value setSamplingFrequency(const Napi::CallbackInfo& info);
        Napi::Value getAudioFrameSize(const Napi::CallbackInfo& info);
        Napi::Value getSamplingFrequency(const Napi::CallbackInfo& info);
        Napi::Value processAudioFrame(const Napi::CallbackInfo& info);
        Napi::Value peakEnergy(const Napi::CallbackInfo& info);
        Napi::Value rootMeanSquare(const Napi::CallbackInfo& info);
        Napi::Value zeroCrossingRate(const Napi::CallbackInfo& info);
        Napi::Value spectralCentroid(const Napi::CallbackInfo& info);
        Napi::Value spectralCrest(const Napi::CallbackInfo& info);
        Napi::Value spectralFlatness(const Napi::CallbackInfo& info);
        Napi::Value spectralRolloff(const Napi::CallbackInfo& info);
        Napi::Value spectralKurtosis(const Napi::CallbackInfo& info);
        Napi::Value energyDifference(const Napi::CallbackInfo& info);
        Napi::Value spectralDifference(const Napi::CallbackInfo& info);
        Napi::Value spectralDifferenceHWR(const Napi::CallbackInfo& info);
        Napi::Value complexSpectralDifference(const Napi::CallbackInfo& info);
        Napi::Value highFrequencyContent(const Napi::CallbackInfo& info);
        Napi::Value getMagnitudeSpectrum(const Napi::CallbackInfo& info);
        Napi::Value pitch(const Napi::CallbackInfo& info);
        Napi::Value getMelFrequencySpectrum(const Napi::CallbackInfo& info);
        Napi::Value getMelFrequencyCepstralCoefficients(const Napi::CallbackInfo& info);

	private:
		static Napi::FunctionReference constructor;
        Gist<float> *gist;
};


/* ============================================
 Native module implementation
============================================ */

Napi::FunctionReference GistNative::constructor;

Napi::Object GistNative::Init(Napi::Env env, Napi::Object exports) {

	Napi::Function func = DefineClass(env, "Gist", {
        InstanceMethod("setAudioFrameSize", &GistNative::setAudioFrameSize),
        InstanceMethod("setSamplingFrequency", &GistNative::setSamplingFrequency),
        InstanceMethod("getAudioFrameSize", &GistNative::getAudioFrameSize),
        InstanceMethod("getSamplingFrequency", &GistNative::getSamplingFrequency),
        InstanceMethod("processAudioFrame", &GistNative::processAudioFrame),
        InstanceMethod("peakEnergy", &GistNative::peakEnergy),
        InstanceMethod("rootMeanSquare", &GistNative::rootMeanSquare),
        InstanceMethod("zeroCrossingRate", &GistNative::zeroCrossingRate),
        InstanceMethod("spectralCentroid", &GistNative::spectralCentroid),
        InstanceMethod("spectralCrest", &GistNative::spectralCrest),
        InstanceMethod("spectralFlatness", &GistNative::spectralFlatness),
        InstanceMethod("spectralRolloff", &GistNative::spectralRolloff),
        InstanceMethod("spectralKurtosis", &GistNative::spectralKurtosis),
        InstanceMethod("energyDifference", &GistNative::energyDifference),
        InstanceMethod("spectralDifference", &GistNative::spectralDifference),
        InstanceMethod("spectralDifferenceHWR", &GistNative::spectralDifferenceHWR),
        InstanceMethod("complexSpectralDifference", &GistNative::complexSpectralDifference),
        InstanceMethod("highFrequencyContent", &GistNative::highFrequencyContent),
        InstanceMethod("getMagnitudeSpectrum", &GistNative::getMagnitudeSpectrum),
        InstanceMethod("pitch", &GistNative::pitch),
        InstanceMethod("getMelFrequencySpectrum", &GistNative::getMelFrequencySpectrum),
        InstanceMethod("getMelFrequencyCepstralCoefficients", &GistNative::getMelFrequencyCepstralCoefficients)
	});

	constructor = Napi::Persistent(func);
	constructor.SuppressDestruct();

	exports.Set("Gist", func);

	return exports;
}

GistNative::GistNative(const Napi::CallbackInfo& info) : Napi::ObjectWrap<GistNative>(info) {
	Napi::Env env = info.Env();
	Napi::HandleScope scope(env);

    int frameSize  = info[0].As<Napi::Number>().Uint32Value();
    int sampleRate = info[1].As<Napi::Number>().Uint32Value();

    gist = new Gist<float>(frameSize, sampleRate, WindowType::HammingWindow);
}

GistNative::~GistNative() {
    delete gist;
    gist = NULL;
}

Napi::Value GistNative::setAudioFrameSize(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int frameSize = info[0].As<Napi::Number>().Uint32Value();

    gist->setAudioFrameSize(frameSize);

    return env.Undefined();
}

Napi::Value GistNative::setSamplingFrequency(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int sampleRate = info[0].As<Napi::Number>().Uint32Value();

    gist->setSamplingFrequency(sampleRate);

    return env.Undefined();
}

Napi::Value GistNative::getAudioFrameSize(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int sampleRate = gist->getAudioFrameSize();

    return Napi::Number::New(env, sampleRate);
}

Napi::Value GistNative::getSamplingFrequency(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int sampleRate = gist->getSamplingFrequency();

    return Napi::Number::New(env, sampleRate);
}

Napi::Value GistNative::processAudioFrame(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    const Napi::Float32Array frames = info[0].As<Napi::Float32Array>();
    const int numElements           = frames.ElementLength();

    if ( numElements != gist->getAudioFrameSize() ) {
        Napi::Error::New(env, "You are passing an audio frame with a different size to the frame size set").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    std::vector<float> audioFrame;

    for (int i = 0; i < numElements; i++) {
        audioFrame.push_back(frames[i]);
    }

    gist->processAudioFrame(audioFrame);

    return Napi::Boolean::New(env, true);
}

Napi::Value GistNative::peakEnergy(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    float peakEnergy = gist->peakEnergy();

    return Napi::Number::New(env, peakEnergy);
}


Napi::Value GistNative::rootMeanSquare(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    float result = gist->rootMeanSquare();
    return Napi::Number::New(env, result);
}

Napi::Value GistNative::zeroCrossingRate(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    float result = gist->zeroCrossingRate();
    return Napi::Number::New(env, result);
}

Napi::Value GistNative::spectralCentroid(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    float result = gist->spectralCentroid();
    return Napi::Number::New(env, result);
}

Napi::Value GistNative::spectralCrest(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    float result = gist->spectralCrest();
    return Napi::Number::New(env, result);
}

Napi::Value GistNative::spectralFlatness(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    float result = gist->spectralFlatness();
    return Napi::Number::New(env, result);
}

Napi::Value GistNative::spectralRolloff(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    float result = gist->spectralRolloff();
    return Napi::Number::New(env, result);
}

Napi::Value GistNative::spectralKurtosis(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    float result = gist->spectralKurtosis();
    return Napi::Number::New(env, result);
}

Napi::Value GistNative::energyDifference(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    float result = gist->energyDifference();
    return Napi::Number::New(env, result);
}

Napi::Value GistNative::spectralDifference(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    float result = gist->spectralDifference();
    return Napi::Number::New(env, result);
}

Napi::Value GistNative::spectralDifferenceHWR(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    float result = gist->spectralDifferenceHWR();
    return Napi::Number::New(env, result);
}

Napi::Value GistNative::complexSpectralDifference(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    float result = gist->complexSpectralDifference();
    return Napi::Number::New(env, result);
}

Napi::Value GistNative::highFrequencyContent(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    float result = gist->highFrequencyContent();
    return Napi::Number::New(env, result);
}

Napi::Value GistNative::getMagnitudeSpectrum(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    const std::vector<float>& magSpec = gist->getMagnitudeSpectrum();
    const int numSpecs = magSpec.size();

    Napi::Array values = Napi::Array::New(env);
    for (int i = 0; i < numSpecs; i++) {
        values[i] = magSpec[i];
    }

    return values;
}

Napi::Value GistNative::pitch(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    float pitch = gist->pitch();
    return Napi::Number::New(env, pitch);
}

Napi::Value GistNative::getMelFrequencySpectrum(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    const std::vector<float>& melSpec = gist->getMelFrequencySpectrum();
    const int numSpecs = melSpec.size();

    Napi::Array values = Napi::Array::New(env);
    for (int i = 0; i < numSpecs; i++) {
        values[i] = melSpec[i];
    }

    return values;
}

Napi::Value GistNative::getMelFrequencyCepstralCoefficients(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    const std::vector<float>& mfcc = gist->getMelFrequencyCepstralCoefficients();
    const int numMFCCs = mfcc.size();

    Napi::Array values = Napi::Array::New(env);
    for (int i = 0; i < numMFCCs; i++) {
        values[i] = mfcc[i];
    }

    return values;
}

/* ============================================
 Native module initialization
============================================ */

Napi::Object Init(Napi::Env env, Napi::Object exports) {
	// Register the engine
	GistNative::Init(env, exports);

	return exports;
};

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)