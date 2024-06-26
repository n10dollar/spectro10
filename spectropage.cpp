#include "spectropage.h"

#include <QHBoxLayout>

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
    QHBoxLayout* hBox = new QHBoxLayout;
    hBox->addWidget(&controlsMenu);
    setLayout(hBox);
}
