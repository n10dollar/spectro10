#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include <QWidget>
#include <QPainter>

#include "audioprocessor.h"

class Oscilloscope : public QWidget
{
    Q_OBJECT
public:
    explicit Oscilloscope(AudioProcessor* audioProcessor, QWidget *parent = nullptr);

public slots:
    void update();

protected:
    // Paint method invoked on QWidget update()
    void paintEvent(QPaintEvent *event) override;

private:
    AudioProcessor* audioProcessor;

    // Core painting/drawing functionality
    void paint(QPainter* painter, QPaintEvent* event);

    // QPainter helper QObjects
    QPainter painter;
    QBrush background;
};

#endif // OSCILLOSCOPE_H
