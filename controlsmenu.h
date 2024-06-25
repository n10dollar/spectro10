#ifndef CONTROLSMENU_H
#define CONTROLSMENU_H

#include <QWidget>

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


class ControlsMenu : public QWidget
{
    Q_OBJECT
public:
    explicit ControlsMenu(QWidget *parent = nullptr);

    RtAudio* rtAudio;

private:
    StreamParams streamParams;

private slots:
    void openStream();
    void closeStream();

    void startStream();
    void stopStream();

    void tickStream();
    void abortStream();
};

#endif // CONTROLSMENU_H
