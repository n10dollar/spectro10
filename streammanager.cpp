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
    setDefaultInputDevice();
    setDefaultOutputDevice();

    setAudioFormat(audioFormat);
    setBufferSize(bufferSize);

    streamParams.audioCallback = &audioCallback;
    streamParams.userData = &streamData;
    // streamParams.options;
}

// ========= setters =========
int StreamManager::setInputDevice(unsigned int deviceID)
{
    qDebug() << "Setting input device";

    // Check if input device exists
    std::vector<unsigned int> allDeviceIDs = getAllDeviceIDs();
    if (vectorContains(allDeviceIDs, deviceID))
    {
        qWarning() << "Input device with id" << deviceID << "doesn't exist";
        return -1;
    }

    // Set device ID
    streamParams.inputParameters.deviceId = deviceID;

    // Set number of input channels
    setNumInputChannels(getDeviceInfo(deviceID).inputChannels);

    // If current sample rate errors
    if (setSampleRate(getSampleRate()) == -1)
        setDefaultSampleRate();

    return 0;
}


void StreamManager::setDefaultInputDevice()
{
    setInputDevice(getDefaultInputDevice().ID);
}


int StreamManager::setNumInputChannels(unsigned int numInputChannels)
{
    // Channels in range: [0, maxChannels]
    if (numInputChannels < 0)
    {
        qWarning() << "numInputChannels negative:" << numInputChannels << "<" << 0;
        return -1;
    }
    if (numInputChannels > getInputDevice().inputChannels)
    {
        qWarning() << "numInputChannels greater than inputDevice.inputChannels:"
                   << numInputChannels << ">"
                   << getInputDevice().inputChannels;
        return -1;
    }

    // Set number of input channels
    streamParams.inputParameters.nChannels = numInputChannels;

    // Resize input buffers
    streamData.iVecBuffers.resize(numInputChannels);

    // Resize input buffer channels
    setBufferSize(getBufferSize());

    return 0;
}


int StreamManager::setOutputDevice(unsigned int deviceID)
{
    qDebug() << "Setting output device";

    // Check if output device exists
    std::vector<unsigned int> allDeviceIDs = getAllDeviceIDs();
    if (vectorContains(allDeviceIDs, deviceID))
    {
        qWarning() << "Output device with id" << deviceID << "doesn't exist";
        return -1;
    }

    // Set device ID
    streamParams.outputParameters.deviceId = deviceID;

    // Set number of output channels
    setNumOutputChannels(getDeviceInfo(deviceID).outputChannels);

    // If current sample rate errors
    if (setSampleRate(getSampleRate()) == -1)
        setDefaultSampleRate();

    return 0;
}


void StreamManager::setDefaultOutputDevice()
{
    setInputDevice(getDefaultOutputDevice().ID);
}


int StreamManager::setNumOutputChannels(unsigned int numOutputChannels)
{
    // Channels in range: [0, maxChannels]
    if (numOutputChannels < 0)
    {
        qWarning() << "numInputChannels negative:" << numOutputChannels << "<" << 0;
        return -1;
    }
    if (numOutputChannels > getOutputDevice().outputChannels)
    {
        qWarning() << "numInputChannels greater than inputDevice.inputChannels:"
                   << numOutputChannels << ">"
                   << getInputDevice().outputChannels;
        return -1;
    }

    // Set number of input channels
    streamParams.outputParameters.nChannels = numOutputChannels;

    // Resize output buffers
    streamData.oVecBuffers.resize(numOutputChannels);

    // Resize output buffer channels
    setBufferSize(getBufferSize());

    return 0;
}


int StreamManager::setSampleRate(unsigned int sampleRate)
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
    {
        streamParams.sampleRate = sampleRate;
        return 0;
    }
    return -1;
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


void StreamManager::setBufferSize(unsigned int bufferSize)
{
    // Check if bufferSize works
    if (!isPowerOfTwo(bufferSize))
    {
        qDebug() << "bufferSize of" << bufferSize << "isn't power of 2";
        return;
    }

    // Set buffer size
    streamParams.bufferFrames = bufferSize;

    // Resize input/output buffers channels
        for (std::vector<float>& iVecBuffer : streamData.iVecBuffers)
            iVecBuffer.resize(bufferSize);
        for (std::vector<float>& oVecBuffer : streamData.oVecBuffers)
            oVecBuffer.resize(bufferSize);
}


// ========= getters =========


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


// ========= callbacks =========
int StreamManager::audioCallback
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
