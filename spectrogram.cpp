#include "spectrogram.h"

#include <QPaintEvent>
#include <QDebug>
#include <cmath>

Spectrogram::Spectrogram
(
    std::vector<float>* dataStream,
    int dimWidth,
    int dimHeight,
    QColor background,
    QWidget *parent
)
    :
    dataStream(dataStream),
    dimWidth(dimWidth),
    dimHeight(dimHeight),
    background(background),
    QWidget{parent}
{
    setFixedSize(dimWidth, dimHeight);
}


// =========== Slots ===========
void Spectrogram::update()
{
    QWidget::update();
    // qDebug() << "Spectrogram updated!";
};


// =========== Overriden ===========
void Spectrogram::paintEvent(QPaintEvent *event)
{
    painter.begin(this);
    paintLogarithmic(&painter, event);
    painter.end();
}


// =========== Class methods ===========
void Spectrogram::paintLinear(QPainter* painter, QPaintEvent* event)
{
    painter->fillRect(event->rect(), background);

    for (int s = 0; s < dataStream->size(); s++)
    {
        int x = (((float) s) / ((float) dataStream->size())) * dimWidth;
        int y = (*dataStream)[x] * dimHeight;
        painter->drawLine(x, dimHeight, x, dimHeight - y);
    }
}


void Spectrogram::paintLogarithmic(QPainter* painter, QPaintEvent* event)
{
    painter->fillRect(event->rect(), background);

    for (int x = 1; x < dataStream->size(); x++)
    {
        int logY = LOG_ZERO * ((float) dimHeight) + std::log10((*dataStream)[x]) * ((1 - LOG_ZERO) * ((float) dimHeight));
        int logXstart =
            (((float) std::log10(x)) /
            ((float) std::log10(dataStream->size()))) *
            dimWidth;
        int logXend =
            (((float) std::log10(x + 1)) /
            ((float) std::log10(dataStream->size()))) *
            dimWidth;
        painter->drawRect(logXstart, dimHeight - logY, logXend - logXstart, dimHeight);
    }
}
