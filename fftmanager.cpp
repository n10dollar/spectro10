#include "fftmanager.h"

#include <cmath>

FFTManager::FFTManager
(
    std::vector<float>* iVecBuffer,
    unsigned int fftSize,
    int direction,
    unsigned int flags,
    QObject *parent
)
    : QObject{parent}, fftData{iVecBuffer, {}}, fftSize(fftSize), direction(direction), flags(flags)
{
    dataIn = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->fftSize);
    dataOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->fftSize);

    fftPlan = fftw_plan_dft_1d(this->fftSize, dataIn, dataOut, this->direction, this->flags);

    fftData.vecFFT.resize(fftSize);
}


FFTManager::~FFTManager()
{
    free(dataIn);
    free(dataOut);
}


void FFTManager::update()
{
    FFT();
}


// vecIn: real components of signal samples
// vecOut: magnitudes of signal FFT
void FFTManager::FFT()
{
    // copy vecIn into dataIn
    for (int i = 0; i < fftSize; i++)
    {
        dataIn[i][0] = (double) (*fftData.iVecBuffer)[i];
        dataIn[i][1] = 0.0;
    }

    fftw_execute(fftPlan);

    // copy vecOut magnitude into dataOut
    for (int i = 0; i < fftSize; i++)
        fftData.vecFFT[i] = (float) std::sqrt
        (
            std::pow(dataOut[i][0], 2) +
            std::pow(dataOut[i][1], 2)
        );
}
