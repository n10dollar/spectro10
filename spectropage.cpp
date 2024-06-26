#include "spectropage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

SpectroPage::SpectroPage(AudioProcessor* audioProcessor, QWidget *parent)
    : QWidget{parent},
    audioProcessor(audioProcessor),
    controlsMenu(audioProcessor),
    preOscilliscope(audioProcessor),
    postOscilliscope(audioProcessor),
    preSpectrogram(audioProcessor),
    postSpectrogram(audioProcessor)
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
