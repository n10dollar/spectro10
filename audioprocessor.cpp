#include "audioprocessor.h"

#include "utils.h"

AudioProcessor::AudioProcessor(QObject *parent)
    : QObject{parent}, fftManager(&streamManager.streamData.iVecBuffers)
{
    audioData.streamData = &streamManager.streamData;
    audioData.fftData = &fftManager.fftData;

    audioData.iVecBufferAvg.resize(audioData.streamData->bufferSize);
    audioData.oVecBufferAvg.resize(audioData.streamData->bufferSize);

    audioData.iVecFFTAvg.resize(audioData.fftData->fftSize);
    audioData.oVecFFTAvg.resize(audioData.fftData->fftSize);
}


void AudioProcessor::update()
{
    fftManager.update();
    createAverages();
}


void AudioProcessor::createAverages()
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
