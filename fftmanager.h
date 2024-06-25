#ifndef FFTMANAGER_H
#define FFTMANAGER_H

#include <QObject>

#include <fftw3.h>

class FFTManager : public QObject
{
    Q_OBJECT
public:
    explicit FFTManager
    (
        unsigned int fftSize,
        int direction = FFTW_FORWARD,
        unsigned int flags = FFTW_MEASURE,
        QObject *parent = nullptr
    );
    ~FFTManager();

    template <typename T>
    void FFT(const std::vector<T>& vecIn, std::vector<T>& vecOut);

private:
    fftw_plan fftPlan;

    fftw_complex* dataIn;
    fftw_complex* dataOut;

    unsigned int fftSize;
    int direction;
    unsigned int flags;

signals:
};

#endif // FFTMANAGER_H
