#include "audioprocessor.h"

#include "utils.h"

AudioProcessor::AudioProcessor(QObject *parent)
    : QObject{parent}, fftManager(&audioData.iVecBuffersNormalized)
{
    audioData.streamData = &streamManager.streamData;
    audioData.fftData = &fftManager.fftData;

    // Init iVecBuffersNormalized
    audioData.iVecBuffersNormalized.resize(audioData.streamData->numInputChannels);
    for (auto& iBuffer : audioData.iVecBuffersNormalized)
        iBuffer.resize(audioData.streamData->bufferSize);

    // Init oVecBuffersNormalized
    audioData.oVecBuffersNormalized.resize(audioData.streamData->numOutputChannels);
    for (auto& oBuffer : audioData.oVecBuffersNormalized)
        oBuffer.resize(audioData.streamData->bufferSize);

    audioData.iVecBufferAvg.resize(audioData.streamData->bufferSize);
    audioData.oVecBufferAvg.resize(audioData.streamData->bufferSize);

    audioData.oVecFFTAvg.resize(audioData.fftData->fftSize);

    audioData.vecOscilliscope.resize(audioData.streamData->bufferSize);
    // Cut off at Nyquist bin
    audioData.vecSpectrogram.resize(audioData.fftData->fftSize / 2);
}


void AudioProcessor::update()
{
    streamBuffersNormalize();

    fftManager.update();

    calculateAverages();

    processOscilliscope();
    processSpectrogram();
}


void AudioProcessor::streamBuffersNormalize()
{
    for (int c = 0; c < audioData.streamData->numInputChannels; c++)
        for (int s = 0; s < audioData.streamData->bufferSize; s++)
            audioData.iVecBuffersNormalized[c][s] = (audioData.streamData->iVecBuffers[c][s] + 1.0f) / 2.0f;

    // for (int c = 0; c < audioData.streamData->numOutputChannels; c++)
    //     for (int s = 0; s < audioData.streamData->bufferSize; s++)
    //         audioData.oVecBuffersNormalized[c][s] = (audioData.streamData->iVecBuffers[c][s] + 1.0f) / 2.0f;
}


void AudioProcessor::calculateAverages()
{
    averageVectors<float>
    (
        audioData.iVecBuffersNormalized,
        audioData.iVecBufferAvg,
        audioData.streamData->numInputChannels,
        audioData.streamData->bufferSize
    );

    averageVectors<float>
    (
        audioData.oVecBuffersNormalized,
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
    for (int i = 0; i < audioData.vecOscilliscope.size(); i++)
        audioData.vecOscilliscope[i] = audioData.iVecBufferAvg[i];
}


void AudioProcessor::processSpectrogram()
{
    for (int i = 0; i < audioData.vecSpectrogram.size(); i++)
        audioData.vecSpectrogram[i] = audioData.oVecFFTAvg[i] / ((float) audioData.fftData->fftSize);
}
