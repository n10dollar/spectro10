#include "oscilloscope.h"

Oscilloscope::Oscilloscope(AudioProcessor* audioProcessor, QWidget *parent)
    : QWidget{parent}, audioProcessor(audioProcessor)
{}
