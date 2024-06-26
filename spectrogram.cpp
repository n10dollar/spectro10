#include "spectrogram.h"

#include <QPaintEvent>
#include <QDebug>

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
    paint(&painter, event);
    painter.end();
}


// =========== Class methods ===========
void Spectrogram::paint(QPainter* painter, QPaintEvent* event)
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
