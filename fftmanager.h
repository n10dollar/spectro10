#ifndef FFTMANAGER_H
#define FFTMANAGER_H

#include <QObject>

#include <vector>
#include <fftw3.h>

#define FFT_DIRECTION FFTW_FORWARD
#define FFT_FLAGS FFTW_MEASURE

typedef struct
{
    // Floating point PCM data
    // Range: [-1, 1]
    std::vector<std::vector<float>> iVecFFTs;
    // FFT output
    std::vector<std::vector<float>> oVecFFTs;
}
FFTData;


typedef struct
{
    // FFTW3 params
        fftw_plan fftPlan;

        fftw_complex* dataIn;
        fftw_complex* dataOut;

        unsigned int fftSize;
        int direction;
        unsigned int flags;

    // Additional params
        unsigned int numFFTChannels;
}
FFTParams;


class FFTManager : public QObject
{
    Q_OBJECT
public:
    explicit FFTManager
    (
        unsigned int numFFTChannels,
        unsigned int fftSize,
        int direction = FFT_DIRECTION,
        unsigned int flags = FFT_FLAGS,
        QObject *parent = nullptr
    );
    ~FFTManager();

    // Setters
        void setNumFFTChannels(unsigned int numFFTChannels);
        int setFFTSize(unsigned int fftSize);

    // Getters
        unsigned int getNumFFTChannels();
        unsigned int getFFTSize();

    FFTData fftData;

public slots:
    void update();

private:
    FFTParams fftParams;

    // iVecFFT(s): real components of signal samples
    // oVecFFT(s): magnitudes of signal calculateFFT
        void calculateFFT
        (
            std::vector<std::vector<float>>& iVecFFTs,
            std::vector<std::vector<float>>& oVecFFTs
        );
        void calculateFFT
        (
            std::vector<float>& iVecFFT,
            std::vector<float>& oVecFFT
        );
};

#endif // FFTMANAGER_H
