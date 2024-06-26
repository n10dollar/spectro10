#include "oscilloscope.h"

#include <QPaintEvent>
#include <QDebug>

Oscilloscope::Oscilloscope(std::vector<float>* dataStream, QWidget *parent)
    : QWidget{parent}, dataStream(dataStream), background(QColor(100, 100, 0))
{
    setFixedSize(dataStream->size(), 50);
}


// =========== Slots ===========
void Oscilloscope::update()
{
    QWidget::update();
    // qDebug() << "Oscilliscope updating!";
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

    // qDebug() << "Oscilliscope pt " << dataStream->size() / 2 << ": " << (*dataStream)[dataStream->size() / 2];
    for (int s = 0; s < dataStream->size(); s++)
    {
        unsigned int point = (((*dataStream)[s] + 1.0f) / 2.0f) * height();
        // qDebug() << "Oscilliscope pt " << dataStream->size() / 2 << ": " << point;
        painter->drawLine(s, height() / 2, s, height() - point);
    }
}
