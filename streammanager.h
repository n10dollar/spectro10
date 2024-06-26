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


typedef struct
{
    // Floating point PCM data
    // Range: [-1, 1]
    std::vector<float> iVecBuffer;
    std::vector<float> oVecBuffer;
}
CallbackData;


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

    CallbackData callbackData;

    RtAudioFormat audioFormat;
    unsigned int sampleRate;
    unsigned int bufferSize;

    unsigned int numInputChannels;
    unsigned int numOutputChannels;

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
