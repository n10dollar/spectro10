#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <QObject>

#include "streammanager.h"
#include "fftmanager.h"

class AudioProcessor : public QObject
{
    Q_OBJECT
public:
    explicit AudioProcessor(QObject *parent = nullptr);

    StreamManager streamManager;
    FFTManager fftmanager;

signals:
};

#endif // AUDIOPROCESSOR_H
