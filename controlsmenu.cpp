#include "controlsmenu.h"

ControlsMenu::ControlsMenu(QWidget *parent)
    : QWidget{parent}
{}


// ========= control audio stream =========
void ControlsMenu::openStream()
{
    rtAudio->openStream
    (
        streamParams.inputParameters,
        streamParams.outputParameters,
        streamParams.format,
        streamParams.sampleRate,
        streamParams.bufferFrames,
        streamParams.callback,
        streamParams.userData,
        streamParams.options
    );
}

void ControlsMenu::closeStream()
{
    rtAudio->closeStream();
}

void ControlsMenu::startStream()
{
    rtAudio->startStream();
}

void ControlsMenu::stopStream()
{
    rtAudio->stopStream();
}

void ControlsMenu::tickStream()
{
}

void ControlsMenu::abortStream()
{
    rtAudio->abortStream();
}
