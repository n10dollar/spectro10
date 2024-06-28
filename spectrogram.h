#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <QWidget>
#include <QPainter>
#include <vector>

class Spectrogram : public QWidget
{
    Q_OBJECT
public:
    explicit Spectrogram
    (
        std::vector<float>* dataStream,
        int dimWidth,
        int dimHeight,
        QColor background,
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

    int dimWidth;
    int dimHeight;
};

#endif // SPECTROGRAM_H
