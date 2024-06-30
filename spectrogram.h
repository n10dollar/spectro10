#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <vector>

#define SPECTROGRAM_WIDTH 400
#define SPECTROGRAM_HEIGHT 200
#define SPECTROGRAM_NUM_BINS 512

#define SPECTROGRAM_BG_COLOR QColor(100, 100, 100)
#define SPECTROGRAM_BAR_COLOR QColor(255, 255, 255)

#define LOG_ZERO .8f

typedef void (*PaintSpectrogram)(QPainter* painter, QPaintEvent* event);
// typedef std::function<void (QPainter* painter, QPaintEvent* event)> PaintSpectrogram;


typedef struct
{
    // Range: [0, 1]
    std::vector<float> dataStream;
}
SpectrogramData;


typedef struct
{
    QPainter painter;

    int width;
    int height;

    int numBins;

    QBrush bgBrush;
    QBrush barBrush;
}
SpectrogramParams;


class Spectrogram : public QWidget
{
    Q_OBJECT
public:
    explicit Spectrogram
    (
        int width = SPECTROGRAM_WIDTH,
        int height = SPECTROGRAM_HEIGHT,
        int numBins = SPECTROGRAM_NUM_BINS,
        QColor bgColor = SPECTROGRAM_BG_COLOR,
        QColor barColor = SPECTROGRAM_BAR_COLOR,
        QWidget *parent = nullptr
    );

    // Setters
        void setPaintSpectrogram(PaintSpectrogram paintSpectrogram);

        void setDimensions(int width, int height);
        void setNumBins(int numBins);

        void setBgColor(QColor bgColor);
        void setBarColor(QColor barColor);

    // Getters
        PaintSpectrogram getPaintSpectrogram();

        // dimensions: [width, height]
        std::pair<int, int> getDimensions();
        int getNumBins();

        QColor getBgColor();
        QColor getBarColor();

    SpectrogramData spectrogramData;

public slots:
    void update();

protected:
    // Paint method invoked on QWidget update()
    void paintEvent(QPaintEvent *event) override;

private:
    SpectrogramParams spectrogramParams;
    PaintSpectrogram paintSpectrogram;

    // Core painting/drawing functionality
    void paintLinear(QPainter* painter, QPaintEvent* event);
    void paintLogarithmic(QPainter* painter, QPaintEvent* event);
};

#endif // SPECTROGRAM_H
