#include "oscilloscope.h"

#include <QPaintEvent>
#include <QDebug>

Oscilloscope::Oscilloscope
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
void Oscilloscope::update()
{
    QWidget::update();
    // qDebug() << "Oscilliscope updated!";
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

    for (int s = 0; s < dataStream->size(); s++)
    {
        int x = (((float) s) / ((float) dataStream->size())) * dimWidth;
        int y = (*dataStream)[s] * dimHeight;
        painter->drawLine(x, dimHeight / 2, x, dimHeight - y);
    }
}
