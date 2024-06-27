#include "audioprocessor.h"

AudioProcessor::AudioProcessor(QObject *parent)
    : QObject{parent}, fftManager(&streamManager.streamData.iVecBuffers)
{}


void AudioProcessor::update()
{
    fftManager.update();
}
