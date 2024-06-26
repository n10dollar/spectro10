#ifndef FFTMANAGER_H
#define FFTMANAGER_H

#include <QObject>

#include <vector>
#include <fftw3.h>


typedef struct
{
    // Floating point PCM data
    // Range: [0, 1]
    std::vector<float>* iVecBuffer;

    // Floating point FFT data
    // Range: [0, 1]
    std::vector<float> vecFFT;
}
FFTData;


class FFTManager : public QObject
{
    Q_OBJECT
public:
    explicit FFTManager
    (
        std::vector<float>* iVecBuffer,
        unsigned int fftSize,
        int direction = FFTW_FORWARD,
        unsigned int flags = FFTW_MEASURE,
        QObject *parent = nullptr
    );
    ~FFTManager();

    void FFT();

    FFTData fftData;

public slots:
    void update();

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
