#ifndef STREAMMANAGER_H
#define STREAMMANAGER_H

#include <QObject>

#include <RtAudio.h>


typedef struct
{
    RtAudio::StreamParameters* outputParameters;
    RtAudio::StreamParameters* inputParameters;

    RtAudioFormat format;
    unsigned int sampleRate;
    unsigned int* bufferFrames;

    RtAudioCallback callback;

    void* userData;
    RtAudio::StreamOptions* options;
}
StreamParams;


class StreamManager : public QObject
{
    Q_OBJECT
public:
    explicit StreamManager(QObject *parent = nullptr);

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
};

#endif // STREAMMANAGER_H
