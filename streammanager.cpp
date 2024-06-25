#include "streammanager.h"

StreamManager::StreamManager(QObject *parent)
    : QObject{parent}
{}


// ========= control audio stream =========
void StreamManager::openStream()
{
    rtAudio.openStream
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

void StreamManager::closeStream()
{
    rtAudio.closeStream();
}

void StreamManager::startStream()
{
    rtAudio.startStream();
}

void StreamManager::stopStream()
{
    rtAudio.stopStream();
}

void StreamManager::tickStream()
{
}

void StreamManager::abortStream()
{
    rtAudio.abortStream();
}
