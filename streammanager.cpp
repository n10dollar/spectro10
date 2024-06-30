#include "streammanager.h"

#include <QDebug>
#include <omp.h>

#include "utils.h"

StreamManager::StreamManager
(
    RtAudioFormat audioFormat,
    unsigned int bufferSize,
    QObject *parent
)
    : QObject{parent}
{
    // Calling API
        setDefaultInputDevice();
        setDefaultOutputDevice();

        setAudioFormat(audioFormat);
        setBufferSize(bufferSize);

    // One-time initialization
        streamParams.audioCallback = &audioCallback;
        streamParams.userData = this;
        // streamParams.options;

    streamParams.streamState = CLOSED;
}


StreamManager::~StreamManager()
{}


// ========= Setters =========
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

    // Get old device's number input channels
    unsigned int oldNumInputChannels = getInputDevice().inputChannels;

    // Set device ID
    streamParams.inputParameters.deviceId = deviceID;

    // If number of input channels errors
    if (setNumInputChannels(oldNumInputChannels) == -1)
        setDefaultNumInputChannels();

    // If current sample rate errors
    if (setSampleRate(getSampleRate()) == -1)
        setDefaultSampleRate();

    // Restart stream for changes to take effect
    restartStream();

    return 0;
}


void StreamManager::setDefaultInputDevice()
{
    setInputDevice(getDefaultInputDevice().ID);
}


int StreamManager::setNumInputChannels(unsigned int numInputChannels)
{
    qDebug() << "Setting number of input channels";

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

    // Restart stream for changes to take effect
    restartStream();

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

    // Get old device's number input channels
    unsigned int oldNumOutputChannels = getInputDevice().outputChannels;

    // Set device ID
    streamParams.outputParameters.deviceId = deviceID;

    // If number of output channels errors
    if (setNumInputChannels(oldNumOutputChannels) == -1)
        setDefaultNumOutputChannels();

    // If current sample rate errors
    if (setSampleRate(getSampleRate()) == -1)
        setDefaultSampleRate();

    // Restart stream for changes to take effect
    restartStream();

    return 0;
}


void StreamManager::setDefaultOutputDevice()
{
    setInputDevice(getDefaultOutputDevice().ID);
}


int StreamManager::setNumOutputChannels(unsigned int numOutputChannels)
{
    qDebug() << "Setting number of output channels";

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

    // Restart stream for changes to take effect
    restartStream();

    return 0;
}


int StreamManager::setSampleRate(unsigned int sampleRate)
{
    qDebug() << "Setting sample rate";

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
        // Restart stream for changes to take effect
        restartStream();
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

    setSampleRate(defaultSampleRate);
}


int StreamManager::setBufferSize(unsigned int bufferSize)
{
    qDebug() << "Setting buffer size";

    // Check if bufferSize works
    if (!isPowerOfTwo(bufferSize))
    {
        qDebug() << "bufferSize of" << bufferSize << "isn't power of 2";
        return -1;
    }

    // Set buffer size
    streamParams.bufferFrames = bufferSize;

    // Resize input/output buffers channels
    #pragma omp parallel for
    for (std::vector<float>& iVecBuffer : streamData.iVecBuffers)
        iVecBuffer.resize(bufferSize);
    #pragma omp parallel for
    for (std::vector<float>& oVecBuffer : streamData.oVecBuffers)
        oVecBuffer.resize(bufferSize);

    // Restart stream for changes to take effect
    restartStream();

    return 0;
}


void StreamManager::setAudioProcessing(AudioProcessing audioProcessing)
{
    streamParams.audioProcessing = audioProcessing;
}


// ========= Getters =========
RtAudio::DeviceInfo StreamManager::getInputDevice()
{
    return rtAudio.getDeviceInfo(streamParams.inputParameters.deviceId);
}


RtAudio::DeviceInfo StreamManager::getOutputDevice()
{
    return rtAudio.getDeviceInfo(streamParams.outputParameters.deviceId);
}


RtAudio::DeviceInfo StreamManager::getDefaultInputDevice()
{
    return rtAudio.getDeviceInfo(rtAudio.getDefaultInputDevice());
}


RtAudio::DeviceInfo StreamManager::getDefaultOutputDevice()
{
    return rtAudio.getDeviceInfo(rtAudio.getDefaultOutputDevice());
}


unsigned int StreamManager::getSampleRate()
{
    return streamParams.sampleRate;
}


unsigned int StreamManager::getBufferSize()
{
    return streamParams.bufferFrames;
}


AudioProcessing StreamManager::getAudioProcessing()
{
    return streamParams.audioProcessing;
}


// ========= Helpers =========
void StreamManager::restartStream()
{
    if (streamParams.streamState == OPENED)
    {
        closeStream();
        openStream();
    }
    else if (streamParams.streamState == STARTED)
    {
        stopStream();
        closeStream();
        openStream();
        startStream();
    }
    else if (streamParams.streamState == CLOSED)
    {}
}


// ========= Control audio stream =========
void StreamManager::openStream()
{
    if (streamParams.streamState == CLOSED)
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
        streamParams.streamState = OPENED;
    }
}


void StreamManager::startStream()
{
    if (streamParams.streamState == OPENED)
    {
        rtAudio.startStream();
        streamParams.streamState = STARTED;
    }
}


void StreamManager::stopStream()
{
    if (streamParams.streamState == STARTED)
    {
        rtAudio.stopStream();
        streamParams.streamState = OPENED;
    }
}


void StreamManager::abortStream()
{
    if (streamParams.streamState == STARTED)
    {
        rtAudio.abortStream();
        streamParams.streamState = OPENED;
    }
}


void StreamManager::closeStream()
{
    if (streamParams.streamState == OPENED)
    {
        rtAudio.closeStream();
        streamParams.streamState = CLOSED;
    }
}


void StreamManager::tickStream()
{}


// ========= Callbacks =========
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
    auto streamManager = (StreamManager *) data;

    #pragma omp parallel for
    for (int s = 0; s < nBufferFrames; s++)
        for (int c = 0; c < streamManager->getInputDevice().inputChannels; c++)
        {
            float sample = iBuffer[s * streamManager->getInputDevice().inputChannels + c];
            streamManager->streamData.iVecBuffers[c][s] = sample;
        }

    // Processing function call
    streamManager->streamParams.audioProcessing(streamManager->streamData);

    #pragma omp parallel for
    for (int s = 0; s < nBufferFrames; s++)
        for (int c = 0; c < streamManager->getOutputDevice().outputChannels; c++)
        {
            float sample = streamManager->streamData.oVecBuffers[c][s];
            oBuffer[s * streamManager->getOutputDevice().outputChannels + c] = sample;
        }

    return 0;
}
