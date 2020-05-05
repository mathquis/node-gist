const File		= require('fs')
const Wav		= require('wavefile').WaveFile
const Gist		= require('./lib/index')
const {framer}	= require('sound-parameters-extractor')

const wavFile = new Wav(File.readFileSync('./test2.wav'))

function convert(n) {
   var v = n < 0 ? n / 32768 : n / 32767;       // convert in range [-32768, 32767]
   return Math.max(-1, Math.min(1, v)); // clamp
}
const WINDOW_SIZE = 16000 * 0.02
const OVERLAP = '50%'
console.log('WINDOW_SIZE: %d', WINDOW_SIZE)

const samples = Array.from(wavFile.getSamples(true, Float32Array).map(convert))
const frames = framer(samples, WINDOW_SIZE, OVERLAP)
const gist = new Gist(WINDOW_SIZE, 8000)
console.log('Audio frame size is %d', gist.getAudioFrameSize())
console.log('Sample rate is %d', gist.getSamplingFrequency())
gist.setSamplingFrequency(16000)
const mfccs = []

for ( let i = 0 ; i < frames.length ; i++ ) {
	process.stdout.write('.')
	const audioFrame = Float32Array.from(frames[i])
	// gist.setAudioFrameSize(audioFrame.length)
	// console.log('Audio frame size is %d', gist.getAudioFrameSize())
	// console.log('Sample rate is %d', gist.getSamplingFrequency())
	gist.processAudioFrame(audioFrame)
	// console.log('Pitch: %d', gist.pitch())
	// console.log('PeakEnergy: %d', gist.peakEnergy())
	// console.log('MelFrequencySpectrum:')
	// console.log(gist.getMelFrequencySpectrum())
	// console.log('MelFrequencyCepstralCoefficients:')
	const mfcc = gist.getMelFrequencyCepstralCoefficients()
	mfccs.push(mfcc)
}
process.stdout.write('\n')
console.log(mfccs.length)
console.log(mfccs[0])
console.log(mfccs[1])
console.log(mfccs[2])