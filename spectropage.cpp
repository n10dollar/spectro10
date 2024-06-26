#include "spectropage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

SpectroPage::SpectroPage(AudioProcessor* audioProcessor, QWidget *parent)
    : QWidget{parent},
    audioProcessor(audioProcessor),
    controlsMenu(audioProcessor),
    preOscilliscope(&audioProcessor->streamManager.callbackData.iVecBuffer),
    postOscilliscope(&audioProcessor->streamManager.callbackData.oVecBuffer),
    preSpectrogram(&audioProcessor->streamManager.callbackData.iVecBuffer),
    postSpectrogram(&audioProcessor->streamManager.callbackData.oVecBuffer)
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
}
