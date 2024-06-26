#include "oscilloscope.h"

#include <QPaintEvent>

Oscilloscope::Oscilloscope(AudioProcessor* audioProcessor, QWidget *parent)
    : QWidget{parent}, audioProcessor(audioProcessor), background(QColor(100, 100, 0))
{
    setFixedSize(NYQUIST_SIZE, 50);
}


// =========== Slots ===========
void Oscilloscope::update()
{
    QWidget::update();
};


// =========== Overriden ===========
void Oscilloscope::paintEvent(QPaintEvent *event)
{
    painter.begin(this);
    paint(&painter, event);
    painter.end();
}


// =========== Class methods ===========
void Oscilloscope::paint(QPainter* painter, QPaintEvent* event)
{
    painter->fillRect(event->rect(), background);

    // for (int i = 0; i < NYQUIST_SIZE; i++)
    // {
    //     int freqMagLeft = fftData->fftMagLeft[i] / fftData->fftSize * height();
    //     int freqMagRight = fftData->fftMagRight[i] / fftData->fftSize * height();

    //     int averageMag = (freqMagLeft + freqMagRight) / 2;

    //     painter->drawLine(i, height(), i, height() - averageMag * 3);
    // }
}
