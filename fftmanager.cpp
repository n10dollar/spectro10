#include "fftmanager.h"

#include <cmath>

FFTManager::FFTManager
(
    std::vector<std::vector<float>>* iVecBuffers,
    unsigned int fftSize,
    int direction,
    unsigned int flags,
    QObject *parent
)
    : QObject{parent}, fftData{iVecBuffers, {}}, fftSize(fftSize), direction(direction), flags(flags)
{
    dataIn = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->fftSize);
    dataOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->fftSize);

    fftPlan = fftw_plan_dft_1d(this->fftSize, dataIn, dataOut, this->direction, this->flags);

    // Resize vecFFTs based iVecBuffers input
    fftData.vecFFTs.resize(iVecBuffers->size());
    for (auto& vecFFT : fftData.vecFFTs)
        vecFFT.resize(fftSize);
}


FFTManager::~FFTManager()
{
    free(dataIn);
    free(dataOut);
}


void FFTManager::update()
{
    for (int i = 0; i < fftData.iVecBuffers->size(); i++)
        FFT((*fftData.iVecBuffers)[i], fftData.vecFFTs[i]);
}


// vecIn: real components of signal samples
// vecOut: magnitudes of signal FFT
void FFTManager::FFT
(
    std::vector<float>& iVecBuffer,
    std::vector<float>& vecFFT
)
{
    // copy vecIn into dataIn
    for (int i = 0; i < fftSize; i++)
    {
        dataIn[i][0] = (double) iVecBuffer[i];
        dataIn[i][1] = 0.0;
    }

    fftw_execute(fftPlan);

    // copy vecOut magnitude into dataOut
    for (int i = 0; i < fftSize; i++)
        vecFFT[i] = (float) std::sqrt
        (
            std::pow(dataOut[i][0], 2) +
            std::pow(dataOut[i][1], 2)
        );
}
