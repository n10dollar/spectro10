#include "streammanager.h"

#include <QDebug>
#include <climits>

#include "utils.h"

StreamManager::StreamManager
(
    RtAudioFormat audioFormat,
    unsigned int bufferSize,
    QObject *parent
)
    : audioFormat(audioFormat), bufferSize(bufferSize), QObject{parent}
{
    std::vector<unsigned int> deviceIds = rtAudio.getDeviceIds();
    if (deviceIds.empty())
        qDebug() << "No audio devices found!";

    for (unsigned int id : deviceIds)
        qDebug() << id << ": " << rtAudio.getDeviceInfo(id).name;

    // Set input device
    streamParams.inputParameters.deviceId = rtAudio.getDefaultInputDevice();

    // Set output device
    streamParams.outputParameters.deviceId = rtAudio.getDefaultOutputDevice();

    // Get input/output device info
    RtAudio::DeviceInfo infoInput = rtAudio.getDeviceInfo(streamParams.inputParameters.deviceId);
    RtAudio::DeviceInfo infoOutput = rtAudio.getDeviceInfo(streamParams.outputParameters.deviceId);

    // Log device info
    qDebug() << "Default Input Device: " << infoInput.name;
    qDebug() << "Default Output Device: " << infoOutput.name;

    qDebug() << "Num Input Channels: " << infoInput.inputChannels;
    qDebug() << "Num Output Channels: " << infoOutput.outputChannels;

    // Set data fields
    sampleRate = closestMatchingSampleRate(infoInput.sampleRates, infoOutput.sampleRates);

    // Set input/output channels
    numInputChannels = infoInput.inputChannels;
    numOutputChannels = infoOutput.outputChannels;

    streamParams.inputParameters.nChannels = numInputChannels;
    streamParams.outputParameters.nChannels = numOutputChannels;

    // Set frame params
    streamParams.audioFormat = audioFormat;
    streamParams.sampleRate = sampleRate;
    streamParams.bufferFrames = bufferSize;

    // Set callback function
    streamParams.audioCallback = &nullCallback;

    // Set additional data params
    streamParams.userData = &callbackData;
    // streamParams.options = ?;

    // Configure vector buffers
    callbackData.iVecBuffer.resize(bufferSize);
    callbackData.oVecBuffer.resize(bufferSize);
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
