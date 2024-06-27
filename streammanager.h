#ifndef STREAMMANAGER_H
#define STREAMMANAGER_H

#include <QObject>
#include <RtAudio.h>
#include <vector>

#define AUDIO_FORMAT RTAUDIO_SINT32
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 512

#define NUM_INPUT_CHANNELS 1
#define NUM_OUTPUT_CHANNELS 2

typedef struct
{
    // Floating point PCM data
    // Range: [-1, 1]
    std::vector<std::vector<float>> iVecBuffers;
    std::vector<std::vector<float>> oVecBuffers;

    RtAudioFormat audioFormat;
    unsigned int sampleRate;
    unsigned int bufferSize;

    unsigned int numInputChannels;
    unsigned int numOutputChannels;
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

    static int nullCallback
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
