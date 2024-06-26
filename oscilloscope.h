#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include <QWidget>

#include "audioprocessor.h"

class Oscilloscope : public QWidget
{
    Q_OBJECT
public:
    explicit Oscilloscope(AudioProcessor* audioProcessor, QWidget *parent = nullptr);

private:
    AudioProcessor* audioProcessor;
};

#endif // OSCILLOSCOPE_H
