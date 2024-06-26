#include "streammanager.h"

#include <QDebug>

StreamManager::StreamManager(QObject *parent)
    : QObject{parent}
{
    std::vector<unsigned int> deviceIds = rtAudio.getDeviceIds();
    if (deviceIds.empty())
        qDebug() << "No audio devices found!";

    for (unsigned int id : deviceIds)
        qDebug() << id << ": " << rtAudio.getDeviceInfo(id).name;

    qDebug() << "Default Input Device: " << rtAudio.getDeviceInfo(rtAudio.getDefaultInputDevice()).name;
    qDebug() << "Default Output Device: " << rtAudio.getDeviceInfo(rtAudio.getDefaultOutputDevice()).name;

    // Set input params
    streamParams.inputParameters.deviceId = rtAudio.getDefaultInputDevice(); // first available device
    streamParams.inputParameters.nChannels = NUM_INPUT_CHANNELS;

    // Set output params
    streamParams.outputParameters.deviceId = rtAudio.getDefaultOutputDevice(); // first available device
    streamParams.outputParameters.nChannels = NUM_OUTPUT_CHANNELS;

    // Set frame params
    streamParams.audioFormat = AUDIO_FORMAT;
    streamParams.sampleRate = SAMPLE_RATE;
    streamParams.bufferFrames = BUFFER_SIZE;

    // Set callback function
    streamParams.audioCallback = &nullCallback;

    // Set additional data params
    streamParams.userData = nullptr;
}


// ========= control audio stream =========
void StreamManager::openStream()
{
    if
    (
        rtAudio.openStream
        (
            &streamParams.inputParameters,
            &streamParams.outputParameters,
            streamParams.audioFormat,
            streamParams.sampleRate,
            &streamParams.bufferFrames,
            streamParams.audioCallback,
            streamParams.userData,
            &streamParams.options
        )
    )
        qDebug() << rtAudio.getErrorText();
    else
        qDebug() << "Opened audio stream!";
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


// ========= callback functions =========
int StreamManager::nullCallback
(
    void *outputBuffer,
    void *inputBuffer,
    unsigned int nBufferFrames,
    double streamTime,
    RtAudioStreamStatus status,
    void *data
)
{
    auto iBuffer = (int *) inputBuffer;
    auto oBuffer = (int *) outputBuffer;

    return 0;
}
