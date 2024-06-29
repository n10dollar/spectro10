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

        qDebug() << "Input Sample Rates: " << infoInput.sampleRates;
        qDebug() << "Output Sample Rates: " << infoOutput.sampleRates;

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
        streamParams.audioCallback = &callback;
        streamParams.userData = &streamData;
        // streamParams.options = ?;
}

// ========= set parameters =========
void StreamManager::setInputDevice(unsigned int deviceID)
{
    qDebug() << "Setting input device";

    // Check if input device exists
    std::vector<unsigned int> allDeviceIDs = getAllDeviceIDs();
    if (vectorContains(allDeviceIDs, deviceID))
    {
        qWarning() << "Input device with id" << deviceID << "doesn't exist";
        return;
    }

    // Set device ID
    streamParams.inputParameters.deviceId = deviceID;
    streamParams.inputParameters.nChannels = getDeviceInfo(deviceID).inputChannels;

    // Update sample rate
    setDefaultSampleRate();
}


void StreamManager::setOutputDevice(unsigned int deviceID)
{
    qDebug() << "Setting output device";

    // Check if output device exists
    std::vector<unsigned int> allDeviceIDs = getAllDeviceIDs();
    if (vectorContains(allDeviceIDs, deviceID))
    {
        qWarning() << "Output device with id" << deviceID << "doesn't exist";
        return;
    }

    // Set device ID
    streamParams.outputParameters.deviceId = deviceID;
    streamParams.outputParameters.nChannels = getDeviceInfo(deviceID).outputChannels;

    // Update sample rate
    setDefaultSampleRate();
}


void StreamManager::setSampleRate(unsigned int sampleRate)
{
    bool sampleRateCompatible = true;

    // Check if input device selected
    if (streamParams.inputParameters.deviceId == 0)
        qWarning() << "No input device selected";
    else
    {
        // Check if sample rate works for input device
        std::vector<unsigned int> inputSampleRates = getInputDevice().sampleRates;
        if (!vectorContains(inputSampleRates, sampleRate))
        {
            qWarning() << "Sample rate of " << sampleRate << "doesn't work for input device" << getInputDevice().ID;
            sampleRateCompatible = false;
        }
    }

    // Check if output device selected
    if (streamParams.outputParameters.deviceId == 0)
        qWarning() << "No output device selected";
    else
    {
        // Check if sample rate works for output device
        std::vector<unsigned int> outputSampleRates = getOutputDevice().sampleRates;
        if (!vectorContains(outputSampleRates, sampleRate))
        {
            qWarning() << "Sample rate of " << sampleRate << "doesn't work for output device" << getOutputDevice().ID;
            sampleRateCompatible = false;
        }
    }

    if (sampleRateCompatible)
        streamParams.sampleRate = sampleRate;
}


void StreamManager::setDefaultSampleRate()
{
    std::vector<std::vector<unsigned int>> inputOutputSampleRates;

    // Check if input device exists
    if (streamParams.inputParameters.deviceId != 0)
        inputOutputSampleRates.push_back(getInputDevice().sampleRates);
    // Check if output device exists
    if (streamParams.outputParameters.deviceId != 0)
        inputOutputSampleRates.push_back(getOutputDevice().sampleRates);

    unsigned int defaultSampleRate = closestMatching(inputOutputSampleRates, (unsigned int) SAMPLE_RATE_REF);
    streamParams.sampleRate = defaultSampleRate;
}


// ========= get parameters =========


// ========= control audio stream =========
void StreamManager::openStream()
{
    if (!streamOpened && !streamStarted)
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
    if (streamOpened && !streamStarted)
        rtAudio.closeStream();
}

void StreamManager::startStream()
{
    if (streamOpened && !streamStarted)
        rtAudio.startStream();
}

void StreamManager::stopStream()
{
    if (streamOpened && streamStarted)
        rtAudio.stopStream();
}

void StreamManager::tickStream()
{
}

void StreamManager::abortStream()
{
    if (streamOpened && streamStarted)
        rtAudio.abortStream();
}


// ========= callback functions =========
int StreamManager::callback
(
    void *outputBuffer,
    void *inputBuffer,
    unsigned int nBufferFrames,
    double streamTime,
    RtAudioStreamStatus status,
    void *data
)
{
    auto iBuffer = (float *) inputBuffer;
    auto oBuffer = (float *) outputBuffer;
    auto streamData = (StreamData *) data;

    for (int s = 0; s < nBufferFrames; s++)
        for (int c = 0; c < streamData->iVecBuffers->size(); c++)
        {
            float sample = iBuffer[s * streamData->iVecBuffers->size() + c];
            (*streamData->iVecBuffers)[c][s] = sample;
        }

    // Processing function calls

    for (int s = 0; s < nBufferFrames; s++)
        for (int c = 0; c < streamData->oVecBuffers->size(); c++)
        {
            float sample = (*streamData->oVecBuffers)[c][s];
            oBuffer[s * streamData->oVecBuffers->size() + c] = sample;
        }

    return 0;
}
