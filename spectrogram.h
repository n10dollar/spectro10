#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <QWidget>
#include <QPainter>
#include <vector>

class Spectrogram : public QWidget
{
    Q_OBJECT
public:
    explicit Spectrogram(std::vector<float>* dataStream, QWidget *parent = nullptr);

public slots:
    void update();

protected:
    // Paint method invoked on QWidget update()
    void paintEvent(QPaintEvent *event) override;

private:
    std::vector<float>* dataStream;

    // Core painting/drawing functionality
    void paint(QPainter* painter, QPaintEvent* event);

    // QPainter helper QObjects
    QPainter painter;
    QBrush background;
};

#endif // SPECTROGRAM_H
