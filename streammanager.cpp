#include "streammanager.h"

#include <QDebug>
#include <climits>>

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
    streamParams.userData = &callbackData;
    // streamParams.options = ?;

    // Configure vector buffers
    callbackData.iVecBuffer.resize(BUFFER_SIZE);
    callbackData.oVecBuffer.resize(BUFFER_SIZE);
}


// ========= control audio stream =========
void StreamManager::openStream()
{
    rtAudio.openStream
    (
        &streamParams.outputParameters,
        &streamParams.inputParameters,
        streamParams.audioFormat,
        streamParams.sampleRate,
        &streamParams.bufferFrames,
        streamParams.audioCallback,
        streamParams.userData,
        &streamParams.options
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
    // for (int s = 0; s < BUFFER_SIZE; s++)
    //     qDebug() << "Stream pt " << s << ": " << callbackData.iVecBuffer[s];
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
    auto callbackData = (CallbackData *) data;

    // qDebug() << "Stream pt (int) " << nBufferFrames / 2 << ": " << iBuffer[nBufferFrames / 2];
    for (int i = 0; i < nBufferFrames; i++)
        callbackData->iVecBuffer[i] = ((float) iBuffer[i]) / ((float) INT32_MAX);
    // qDebug() << "Stream pt (float) " << nBufferFrames / 2 << ": " << callbackData->iVecBuffer[nBufferFrames / 2];
    \
    return 0;
}
