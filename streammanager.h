#ifndef STREAMMANAGER_H
#define STREAMMANAGER_H

#include <QObject>
#include <RtAudio.h>
#include <vector>

#define SAMPLE_RATE_REF 40000

#define AUDIO_FORMAT RTAUDIO_FLOAT32
#define BUFFER_SIZE 1024

typedef struct
{
    // Floating point PCM data
    // Range: [-1, 1]
        // Generated by StreamManager
        std::vector<std::vector<float>> iVecBuffers;
        // Pulled from elsewhere
        std::vector<std::vector<float>>* oVecBuffers;
}
StreamData;


typedef struct
{
    RtAudio::StreamParameters inputParameters;
    RtAudio::StreamParameters outputParameters;

    RtAudioFormat audioFormat;
    unsigned int sampleRate;
    unsigned int bufferFrames;

    RtAudioCallback audioCallback;

    void* userData;
    RtAudio::StreamOptions options;
}
StreamParams;


class StreamManager : public QObject
{
    Q_OBJECT
public:
    explicit StreamManager
    (
        RtAudioFormat audioFormat = AUDIO_FORMAT,
        unsigned int bufferSize = BUFFER_SIZE,
        QObject *parent = nullptr
    );

    std::vector<RtAudio::DeviceInfo> getAllDevices();
    std::vector<unsigned int> getAllDeviceIDs();
    RtAudio::DeviceInfo getDeviceInfo(unsigned int deviceID);

    void setInputDevice(unsigned int deviceID);
    RtAudio::DeviceInfo getInputDevice();

    void setOutputDevice(unsigned int deviceID);
    RtAudio::DeviceInfo getOutputDevice();

    void setSampleRate(unsigned int sampleRate);
    void setDefaultSampleRate();
    unsigned int getSampleRate();

    void setBufferSize(unsigned int bufferSize);
    unsigned int getBufferSize();

    void setProcessing(? processing);
    ? getCallback();

    StreamData streamData;

public slots:
    void openStream();
    void closeStream();

    void startStream();
    void stopStream();

    void tickStream();
    void abortStream();

private:
    RtAudio rtAudio;
    StreamParams streamParams;

    bool streamOpened;
    bool streamStarted;

    static int callback
    (
        void *outputBuffer,
        void *inputBuffer,
        unsigned int nBufferFrames,
        double streamTime,
        RtAudioStreamStatus status,
        void *data
    );
};

#endif // STREAMMANAGER_H
