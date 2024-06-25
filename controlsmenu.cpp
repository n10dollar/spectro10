#include "controlsmenu.h"

#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

#include "streammanager.h"

ControlsMenu::ControlsMenu(AudioProcessor* audioProcessor, QWidget *parent)
    : QWidget{parent}, audioProcessor(audioProcessor)
{
    // Spectrogram controls
    QPushButton* openButton = new QPushButton(tr("Open"));
    QPushButton* closeButton = new QPushButton(tr("Close"));
    QPushButton* startButton = new QPushButton(tr("Start"));
    QPushButton* stopButton = new QPushButton(tr("Stop"));

    // Connect QPushButton signal to StreamManager slots
    connect(openButton, &QPushButton::clicked, &audioProcessor->streamManager, &StreamManager::openStream);
    connect(closeButton, &QPushButton::clicked, &audioProcessor->streamManager, &StreamManager::closeStream);
    connect(startButton, &QPushButton::clicked, &audioProcessor->streamManager, &StreamManager::startStream);
    connect(stopButton, &QPushButton::clicked, &audioProcessor->streamManager, &StreamManager::stopStream);

    // Create QList
    QList<QAbstractButton*>* buttonList = new QList<QAbstractButton*>;
    buttonList->append(openButton);
    buttonList->append(closeButton);
    buttonList->append(startButton);
    buttonList->append(stopButton);

    QButtonGroup* buttonGroup = new QButtonGroup;
    QVBoxLayout* buttonBox = new QVBoxLayout;

    // Add buttons to groups for management
    for (QAbstractButton* button : *buttonList)
    {
        // Logical QButtonGroup
        buttonGroup->addButton(button);

        // Visual QVBoxLayout
        buttonBox->addWidget(button);
    }

    // QLayout for controls
    setLayout(buttonBox);
}
