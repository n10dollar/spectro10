#include "controlsmenu.h"

ControlsMenu::ControlsMenu(AudioProcessor* audioProcessor, QWidget *parent)
    : QWidget{parent}
{
    this->audioProcessor = audioProcessor;
}
