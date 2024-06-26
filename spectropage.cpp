#include "spectropage.h"

SpectroPage::SpectroPage(AudioProcessor* audioProcessor, QWidget *parent)
    : QWidget{parent}, audioProcessor(audioProcessor), controlsMenu(audioProcessor)
{}
