#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <QWidget>
#include <QPainter>

#include "audioprocessor.h"

class Spectrogram : public QWidget
{
    Q_OBJECT
public:
    explicit Spectrogram(AudioProcessor* audioProcessor, QWidget *parent = nullptr);

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

#endif // SPECTROGRAM_H
