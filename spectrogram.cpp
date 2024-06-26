#include "spectrogram.h"

Spectrogram::Spectrogram(AudioProcessor* audioProcessor, QWidget *parent)
    : QWidget{parent}, audioProcessor(audioProcessor)
{}
