#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <QWidget>
#include <QPainter>
#include <vector>

#define WIDTH 400
#define HEIGHT 200
#define BACKGROUND QColor(100, 100, 100)
#define BINS QColor(255, 255, 255)

#define LOG_ZERO .8f

class Spectrogram : public QWidget
{
    Q_OBJECT
public:
    explicit Spectrogram
    (
        std::vector<float>* dataStream,
        int dimWidth = WIDTH,
        int dimHeight = HEIGHT,
        QColor background = BACKGROUND,
        QColor bins = BINS,
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
    void paintLinear(QPainter* painter, QPaintEvent* event);
    void paintLogarithmic(QPainter* painter, QPaintEvent* event);

    // QPainter helper QObjects
    QPainter painter;
    QBrush background;
    QBrush bins;

    int dimWidth;
    int dimHeight;
};

#endif // SPECTROGRAM_H
