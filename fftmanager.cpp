#include "fftmanager.h"

#include <cmath>

FFTManager::FFTManager
(
    std::vector<std::vector<float>>* iVecFFTs,
    unsigned int fftSize,
    int direction,
    unsigned int flags,
    QObject *parent
)
    : QObject{parent}
{
    // Configure data params
        fftData.numFFTBuffers = iVecFFTs->size();

        fftData.fftSize = fftSize;
        fftParams.fftSize = fftSize;

        fftParams.direction = direction;

        fftParams.flags = flags;

    // Configure FFT process
        fftParams.dataIn = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftParams.fftSize);
        fftParams.dataOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftParams.fftSize);

        fftParams.fftPlan = fftw_plan_dft_1d
        (
            fftParams.fftSize,
            fftParams.dataIn,
            fftParams.dataOut,
            fftParams.direction,
            fftParams.flags
        );

    // Configure FFT input/output
        fftData.iVecFFTs = iVecFFTs;

        // Resize vecFFTs based iVecBuffers input
        fftData.oVecFFTs.resize(fftData.numFFTBuffers);
        for (auto& vecFFT : fftData.oVecFFTs)
            vecFFT.resize(fftSize);
}


FFTManager::~FFTManager()
{
    free(fftParams.dataIn);
    free(fftParams.dataOut);
}


void FFTManager::update()
{
    for (int i = 0; i < fftData.numFFTBuffers; i++)
        FFT((*fftData.iVecFFTs)[i], fftData.oVecFFTs[i]);
}


// vecIn: real components of signal samples
// vecOut: magnitudes of signal FFT
void FFTManager::FFT
(
    std::vector<float>& iVecFFTs,
    std::vector<float>& oVecFFT
)
{
    // copy vecIn into dataIn
    for (int i = 0; i < fftParams.fftSize; i++)
    {
        fftParams.dataIn[i][0] = (double) iVecFFTs[i];
        fftParams.dataIn[i][1] = 0.0;
    }

    fftw_execute(fftParams.fftPlan);

    // copy vecOut magnitude into dataOut
    for (int i = 0; i < fftParams.fftSize; i++)
        oVecFFT[i] = (float) std::sqrt
        (
            std::pow(fftParams.dataOut[i][0], 2) +
            std::pow(fftParams.dataOut[i][1], 2)
        );
}
