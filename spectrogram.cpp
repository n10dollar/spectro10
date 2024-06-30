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
    QColor bins,
    QWidget *parent
)
    :
    dataStream(dataStream),
    dimWidth(dimWidth),
    dimHeight(dimHeight),
    background(background),
    bins(bins),
    QWidget{parent}
{
    setFixedSize(dimWidth, dimHeight);
}


// ========= Slots =========
void Spectrogram::update()
{
    QWidget::update();
    // qDebug() << "Spectrogram updated!";
};


// ========= Overriden =========
void Spectrogram::paintEvent(QPaintEvent *event)
{
    painter.begin(this);
    paintLogarithmic(&painter, event);
    painter.end();
}


// ========= Drawing =========
void Spectrogram::paintLinear(QPainter* painter, QPaintEvent* event)
{
    painter->fillRect(event->rect(), background);

    for (int s = 0; s < dataStream->size(); s++)
    {
        int x = (((float) s) / ((float) dataStream->size())) * dimWidth;
        int y = (*dataStream)[s] * dimHeight;
        painter->drawLine(x, dimHeight, x, dimHeight - y);
    }
}


void Spectrogram::paintLogarithmic(QPainter* painter, QPaintEvent* event)
{
    painter->fillRect(event->rect(), background);

    for (int s = 1; s < dataStream->size(); s++)
    {
        int logXstart =
            (((float) std::log10(s)) /
             ((float) std::log10(dataStream->size()))) *
            dimWidth;
        int logXend =
            (((float) std::log10(s + 1)) /
             ((float) std::log10(dataStream->size()))) *
            dimWidth;
        int logY = LOG_ZERO * ((float) dimHeight) + std::log10((*dataStream)[s]) * ((1 - LOG_ZERO) * ((float) dimHeight));
        painter->drawRect(logXstart, dimHeight - logY, logXend - logXstart, dimHeight);
        // painter->fillRect(logXstart, dimHeight - logY, logXend - logXstart, dimHeight, bins);
    }
}
