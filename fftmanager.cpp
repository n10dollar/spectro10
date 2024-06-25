#include "fftmanager.h"

#include <cmath>

FFTManager::FFTManager(unsigned int fftSize, int direction, unsigned int flags, QObject *parent)
    : QObject{parent}
{
    this->fftSize = fftSize;
    this->direction = direction;
    this->flags = flags;

    dataIn = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->fftSize);
    dataOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->fftSize);

    fftPlan = fftw_plan_dft_1d(this->fftSize, dataIn, dataOut, this->direction, this->flags);
}


FFTManager::~FFTManager()
{
    free(dataIn);
    free(dataOut);
}


// vecIn: real components of signal samples
// vecOut: magnitudes of signal FFT
template <typename T>
void FFTManager::FFT(const std::vector<T>& vecIn, std::vector<T>& vecOut)
{
    // copy vecIn into dataIn
    for (int i = 0; i < fftSize; i++)
    {
        dataIn[i][0] = (double) vecIn[i];
        dataIn[i][1] = 0.0;
    }

    fftw_execute(fftPlan);

    // copy vecOut magnitude into dataOut
    for (int i = 0; i < fftSize; i++)
        vecOut[i] = (T) std::sqrt
        (
            std::pow(dataOut[i][0], 2) +
            std::pow(dataOut[i][1], 2)
        );
}
