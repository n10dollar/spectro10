#include "spectrogram.h"

#include <QDebug>

Spectrogram::Spectrogram
(
    int width,
    int height,
    int numBins,
    QColor bgColor,
    QColor barColor,
    QWidget *parent
)
    :
    spectrogramParams
    {
        {},
        width,
        height,
        numBins,
        {bgColor},
        {barColor},
    },
    QWidget{parent}
{
    setDimensions(width, height);
    setNumBins(numBins);
    setPaintSpectrogram(&paintLinear);
}


// ========= Slots =========
void Spectrogram::update()
{
    QWidget::update();
};


// ========= Setters =========
void Spectrogram::setPaintSpectrogram(PaintSpectrogram paintSpectrogram)
{
    qDebug() << "Setting spectrogram paint scaling";

    this->paintSpectrogram = paintSpectrogram;
}


void Spectrogram::setDimensions(int width, int height)
{
    qDebug() << "Setting spectrogram dimensions (width, height)";

    spectrogramParams.width = width;
    spectrogramParams.height = height;

    setFixedSize(width, height);
}


void Spectrogram::setNumBins(int numBins)
{
    qDebug() << "Setting spectrogram number of bins";

    spectrogramParams.numBins = numBins;

    spectrogramData.dataStream.resize(numBins);
}


void Spectrogram::setBgColor(QColor bgColor)
{
    spectrogramParams.bgBrush.setColor(bgColor);
}


void Spectrogram::setBarColor(QColor barColor)
{
    spectrogramParams.barBrush.setColor(barColor);
}


// ========= Getters =========
PaintSpectrogram Spectrogram::getPaintSpectrogram()
{
    return paintSpectrogram;
}


std::pair<int, int> Spectrogram::getDimensions()
{
    return std::pair<int, int>(spectrogramParams.width, spectrogramParams.height);
}


int Spectrogram::getNumBins()
{
    return spectrogramParams.numBins;
}


QColor Spectrogram::getBgColor()
{
    return spectrogramParams.bgBrush.color();
}


QColor Spectrogram::getBarColor()
{
    return spectrogramParams.barBrush.color();
}


// ========= Overriden =========
void Spectrogram::paintEvent(QPaintEvent *event)
{
    spectrogramParams.painter.begin(this);
    paintSpectrogram(&spectrogramParams.painter, event);
    spectrogramParams.painter.end();
}


// ========= Painting =========
void Spectrogram::paintLinear(QPainter* painter, QPaintEvent* event)
{
    painter->fillRect(event->rect(), spectrogramParams.bgBrush);

    for (int s = 1; s < spectrogramParams.numBins; s++)
    {
        int x = (((float) s) / ((float) spectrogramParams.numBins)) * getDimensions().first;
        int y = spectrogramData.dataStream[s] * getDimensions().second;
        painter->drawLine(x, getDimensions().second, x, getDimensions().second - y);
    }
}


void Spectrogram::paintLogarithmic(QPainter* painter, QPaintEvent* event)
{
    painter->fillRect(event->rect(), spectrogramParams.bgBrush);

    for (int s = 1; s < spectrogramParams.numBins; s++)
    {
        int logXstart =
            (((float) std::log10(s)) /
            ((float) std::log10(spectrogramParams.numBins))) *
            getDimensions().first;
        int logXend =
            (((float) std::log10(s + 1)) /
            ((float) std::log10(spectrogramParams.numBins))) *
            getDimensions().first;

        int logY = LOG_ZERO *
            ((float) getDimensions().second) + std::log10(spectrogramData.dataStream[s]) *
            ((1 - LOG_ZERO) * ((float) getDimensions().second));

        painter->drawRect(logXstart, getDimensions().second - logY, logXend - logXstart, getDimensions().second);
    }
}
