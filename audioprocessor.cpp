#include "audioprocessor.h"

#include "utils.h"

AudioProcessor::AudioProcessor(QObject *parent)
    : QObject{parent}, fftManager(&streamManager.streamData.iVecBuffers)
{
    audioData.streamData = &streamManager.streamData;
    audioData.fftData = &fftManager.fftData;

    audioData.iVecBufferAvg.resize(audioData.streamData->bufferSize);
    audioData.oVecBufferAvg.resize(audioData.streamData->bufferSize);

    audioData.oVecFFTAvg.resize(audioData.fftData->fftSize);

    audioData.vecOscilliscope.resize(audioData.streamData->bufferSize);
    // Cut off at Nyquist bin
    audioData.vecSpectrogram.resize(audioData.fftData->fftSize / 2);
}


void AudioProcessor::update()
{
    fftManager.update();

    calculateAverages();

    processOscilliscope();
    processSpectrogram();
}


void AudioProcessor::calculateAverages()
{
    averageVectors<float>
    (
        audioData.streamData->iVecBuffers,
        audioData.iVecBufferAvg,
        audioData.streamData->numInputChannels,
        audioData.streamData->bufferSize
    );

    averageVectors<float>
    (
        audioData.streamData->oVecBuffers,
        audioData.oVecBufferAvg,
        audioData.streamData->numOutputChannels,
        audioData.streamData->bufferSize
    );

    averageVectors<float>
    (
        audioData.fftData->oVecFFTs,
        audioData.oVecFFTAvg,
        audioData.fftData->numFFTBuffers,
        audioData.fftData->fftSize
    );
}


void AudioProcessor::processOscilliscope()
{
    for(int i = 0; i < audioData.vecOscilliscope.size(); i++)
        audioData.vecOscilliscope[i] = (audioData.iVecBufferAvg[i] + 1.0f) / 2.0f;
}


void AudioProcessor::processSpectrogram()
{
    for(int i = 0; i < audioData.vecSpectrogram.size(); i++)
        audioData.vecSpectrogram[i] = audioData.oVecFFTAvg[i] / ((float) audioData.fftData->fftSize);
}
