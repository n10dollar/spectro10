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
    : QObject{parent}
{
    // Get device
        std::vector<unsigned int> deviceIds = rtAudio.getDeviceIds();
        if (deviceIds.empty())
            qDebug() << "No audio devices found!";

        for (unsigned int id : deviceIds)
            qDebug() << id << ": " << rtAudio.getDeviceInfo(id).name;

        // Get input device
        int deviceInput = rtAudio.getDefaultInputDevice();

        // Get output device
        int deviceOutput = rtAudio.getDefaultOutputDevice();

        // Get input/output device info
        RtAudio::DeviceInfo infoInput = rtAudio.getDeviceInfo(deviceInput);
        RtAudio::DeviceInfo infoOutput = rtAudio.getDeviceInfo(deviceOutput);

        // Log device info
        qDebug() << "Default Input Device: " << infoInput.name;
        qDebug() << "Default Output Device: " << infoOutput.name;

        qDebug() << "Num Input Channels: " << infoInput.inputChannels;
        qDebug() << "Num Output Channels: " << infoOutput.outputChannels;

        qDebug() << "Input Sample Rates:";
        for (auto& sampleRate : infoInput.sampleRates)
            qDebug() << sampleRate;
        qDebug() << "Output Sample Rates:";
        for (auto& sampleRate : infoOutput.sampleRates)
            qDebug() << sampleRate;

    // Configure input/output params
        streamParams.inputParameters.deviceId = deviceInput;
        streamParams.outputParameters.deviceId = deviceOutput;

        streamData.numInputChannels = infoInput.inputChannels;
        streamParams.inputParameters.nChannels = infoInput.inputChannels;

        streamData.numOutputChannels = infoOutput.outputChannels;
        streamParams.outputParameters.nChannels = infoOutput.outputChannels;

    // Configure data params
        streamData.audioFormat = audioFormat;
        streamParams.audioFormat = audioFormat;

        int sampleRate = closestMatchingSampleRate(infoInput.sampleRates, infoOutput.sampleRates);
        streamData.sampleRate = sampleRate;
        streamParams.sampleRate = sampleRate;

        streamData.bufferSize = bufferSize;
        streamParams.bufferFrames = bufferSize;

    // Configure vector buffer channels
        streamData.iVecBuffers.resize(streamData.numInputChannels);
        streamData.oVecBuffers.resize(streamData.numOutputChannels);

        // Set vector buffers size
        for (auto& iVecBuffer : streamData.iVecBuffers)
            iVecBuffer.resize(bufferSize);
        for (auto& oVecBuffer : streamData.oVecBuffers)
            oVecBuffer.resize(bufferSize);

    // Set additional StreamParams
        streamParams.audioCallback = &nullCallback;
        streamParams.userData = &streamData;
        // streamParams.options = ?;
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
    auto streamData = (StreamData *) data;

    // qDebug() << "Stream pt (int) " << nBufferFrames / 2 << ": " << iBuffer[nBufferFrames / 2];
    for (int s = 0; s < nBufferFrames; s++)
        for (int c = 0; c < streamData->numInputChannels; c++)
        {
            int sample = iBuffer[s * streamData->numInputChannels + c];
            streamData->iVecBuffers[c][s] = sample;
        }
    // qDebug() << "Stream pt (float) " << nBufferFrames / 2 << ": " << callbackData->iVecBuffer[nBufferFrames / 2];
    \
    return 0;
}
