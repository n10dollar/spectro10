#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include <QWidget>
#include <QPainter>
#include <vector>

#define OSCILLOSCOPE_WIDTH 400
#define OSCILLOSCOPE_HEIGHT 50
#define OSCILLOSCOPE_BACKGROUND QColor(100, 100, 100)

class Oscilloscope : public QWidget
{
    Q_OBJECT
public:
    explicit Oscilloscope
    (
        std::vector<float>* dataStream,
        int dimWidth = OSCILLOSCOPE_WIDTH,
        int dimHeight = OSCILLOSCOPE_HEIGHT,
        QColor background = OSCILLOSCOPE_BACKGROUND,
        QWidget *parent = nullptr
    );

public slots:
    void update();

protected:
    // Paint method invoked on QWidget update()
    void paintEvent(QPaintEvent *event) override;

private:
    // Range: [0, 1]
    std::vector<float>* dataStream;

    // Core painting/drawing functionality
    void paint(QPainter* painter, QPaintEvent* event);

    // QPainter helper QObjects
    QPainter painter;
    QBrush background;

    int dimWidth;
    int dimHeight;
};

#endif // OSCILLOSCOPE_H
