#include "settingspage.h"

SettingsPage::SettingsPage(AudioProcessor* audioProcessor, QWidget *parent)
    : QWidget{parent}, audioProcessor(audioProcessor)
{}
