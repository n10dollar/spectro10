#include "oscilloscope.h"

#include <QPaintEvent>

Oscilloscope::Oscilloscope(std::vector<float>* dataStream, QWidget *parent)
    : QWidget{parent}, dataStream(dataStream), background(QColor(100, 100, 0))
{
    setFixedSize(dataStream->size(), 50);
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

    for (int s; s < dataStream->size(); s++)
    {
        unsigned int point = (((*dataStream)[2 * s] + 1.0f) / 2.0f) * height();
        painter->drawLine(s, height() / 2, s, height() - point);
    }
}
