#include "spectrogram.h"

#include <QPaintEvent>
#include <QDebug>
#include <cmath>

Spectrogram::Spectrogram(std::vector<float>* dataStream, QWidget *parent)
    : QWidget{parent}, dataStream(dataStream), background(QColor(100, 100, 0))
{
    setFixedSize(dataStream->size(), 200);
}


// =========== Slots ===========
void Spectrogram::update()
{
    QWidget::update();
    // qDebug() << "Spectrogram updating!";
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

    for (int s = 0; s < dataStream->size() / 2; s++)
    {
        unsigned int point = (*dataStream)[s] * 3;
        // qDebug() << "Spectrogram pt " << dataStream->size() / 2 << ": " << point;
        painter->drawLine(2 * s, height(), 2 * s, height() - point);
        painter->drawLine(2 * s + 1, height(), 2 * s + 1, height() - point);
    }
}


void Spectrogram::paintLogarithmic(QPainter* painter, QPaintEvent* event)
{
    painter->fillRect(event->rect(), background);

    for (int s = 0; s < dataStream->size() / 2; s++)
    {
        unsigned int y = (*dataStream)[s] * 3;
        // qDebug() << "Spectrogram pt " << dataStream->size() / 2 << ": " << point;
        int logXs =
            (((float) std::log10(2 * s)) /
            ((float) std::log10(dataStream->size()))) *
            dataStream->size();
        int logXe =
            (((float) std::log10(2 * (s + 1))) /
             ((float) std::log10(dataStream->size()))) *
            dataStream->size();
        painter->drawRect(logXs, height()- y, logXe - logXs, height());
        // painter->drawLine(2 * s + 1, height(), 2 * s + 1, height() - point);
    }
}
