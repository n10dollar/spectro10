#include "audioprocessor.h"

#include "utils.h"

AudioProcessor::AudioProcessor(QObject *parent)
    : QObject{parent}, fftManager(&streamManager.streamData.iVecBuffers)
{}


void AudioProcessor::update()
{
    createAverages();

    fftManager.update();
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
