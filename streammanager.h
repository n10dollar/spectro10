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
        // Generated by stream
        std::vector<std::vector<float>> iVecBuffers;
        // Pulled from elsewhere
        std::vector<std::vector<float>> oVecBuffers;
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


typedef void (*AudioProcessing)(StreamData streamData);


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

    // Setters
        int setInputDevice(unsigned int deviceID);
        void setDefaultInputDevice();

        int setNumInputChannels(unsigned int numInputChannels);

        int setOutputDevice(unsigned int deviceID);
        void setDefaultOutputDevice();

        int setNumOutputChannels(unsigned int numOutputChannels);

        void setAudioFormat(RtAudioFormat audioFormat);

        int setSampleRate(unsigned int sampleRate);
        void setDefaultSampleRate();

        void setBufferSize(unsigned int bufferSize);

        void setAudioProcessing(AudioProcessing audioProcessing);

    // Getters
        RtAudio::DeviceInfo getInputDevice();
        RtAudio::DeviceInfo getOutputDevice();

        RtAudio::DeviceInfo getDefaultInputDevice();
        RtAudio::DeviceInfo getDefaultOutputDevice();

        unsigned int getSampleRate();

        unsigned int getBufferSize();

        AudioProcessing getCallback();

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

    static int audioCallback
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
