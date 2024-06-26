#include "audioprocessor.h"

AudioProcessor::AudioProcessor(QObject *parent)
    : QObject{parent}, fftManager(FFT_SIZE)
{}
