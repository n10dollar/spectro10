#include "spectropage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>

SpectroPage::SpectroPage(AudioProcessor* audioProcessor, QWidget *parent)
    : QWidget{parent},
    audioProcessor(audioProcessor),
    controlsMenu(audioProcessor),
    preOscilliscope(&audioProcessor->streamManager.streamData.iVecBuffer),
    postOscilliscope(&audioProcessor->streamManager.streamData.oVecBuffer),
    preSpectrogram(&audioProcessor->fftManager.fftData.vecFFT),
    postSpectrogram(&audioProcessor->fftManager.fftData.vecFFT)
{
    // Highest level box
    QHBoxLayout* row = new QHBoxLayout;
    row->addWidget(&controlsMenu);

    // Osc and Spec for pre-processing buffer
    QVBoxLayout* preCol = new QVBoxLayout;
    preCol->addWidget(&preOscilliscope);
    preCol->addWidget(&preSpectrogram);
    row->addLayout(preCol);

    // Osc and Spec for post-processing buffer
    QVBoxLayout* postCol = new QVBoxLayout;
    postCol->addWidget(&postOscilliscope);
    postCol->addWidget(&postSpectrogram);
    row->addLayout(postCol);

    setLayout(row);

    // Timer to update GUI components
    QTimer* timer = new QTimer;
    connect(timer, &QTimer::timeout, &audioProcessor->fftManager, &FFTManager::update);
    connect(timer, &QTimer::timeout, &preOscilliscope, &Oscilloscope::update);
    connect(timer, &QTimer::timeout, &postOscilliscope, &Oscilloscope::update);
    connect(timer, &QTimer::timeout, &preSpectrogram, &Spectrogram::update);
    connect(timer, &QTimer::timeout, &postSpectrogram, &Spectrogram::update);
    timer->start(UPDATE_INTERVAL);
}
