#include "fftmanager.h"

#include <cmath>
#include <QDebug>

FFTManager::FFTManager
(
    unsigned int fftSize,
    int direction,
    unsigned int flags,
    QObject *parent
)
    : QObject{parent}
{
    // Configure data params
    fftParams.direction = direction;
    fftParams.flags = flags;

    resize(fftSize);
}


FFTManager::~FFTManager()
{
    free(fftParams.dataIn);
    free(fftParams.dataOut);
}


void FFTManager::resize(unsigned int fftSize)
{
    fftParams.fftSize = fftSize;

    fftParams.dataIn = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftSize);
    fftParams.dataOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftSize);

    fftParams.fftPlan = fftw_plan_dft_1d
    (
        fftParams.fftSize,
        fftParams.dataIn,
        fftParams.dataOut,
        fftParams.direction,
        fftParams.flags
    );
}


void FFTManager::update()
{
    FFT(*fftData.iVecFFTs, *fftData.oVecFFTs);
    // qDebug() << "FFT updated!";
}


void FFTManager::FFT
(
    std::vector<std::vector<float>>& iVecFFTs,
    std::vector<std::vector<float>>& oVecFFTs
)
{
    for (int i = 0; i < iVecFFTs.size(); i++)
        FFT(iVecFFTs[i], oVecFFTs[i]);
}



// vecIn: real components of signal samples
// vecOut: magnitudes of signal FFT
void FFTManager::FFT
(
    std::vector<float>& iVecFFT,
    std::vector<float>& oVecFFT
)
{
    // copy vecIn into dataIn
    for (int i = 0; i < fftParams.fftSize; i++)
    {
        fftParams.dataIn[i][0] = (double) iVecFFT[i];
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
