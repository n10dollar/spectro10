#include "audioprocessor.h"

AudioProcessor::AudioProcessor(QObject *parent)
    : QObject{parent}, fftManager(&streamManager.callbackData.iVecBuffer, FFT_SIZE)
{}


void AudioProcessor::update()
{
    for (int i)
}
