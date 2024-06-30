#include "fftmanager.h"

#include <QDebug>
#include <cmath>
#include <omp.h>

#include "utils.h"

FFTManager::FFTManager
(
    unsigned int numFFTChannels,
    unsigned int fftSize,
    int direction,
    unsigned int flags,
    QObject *parent
)
    : QObject{parent}
{
    // Calling API
    setNumFFTChannels(numFFTChannels);
    setFFTSize(fftSize);

    // One-time initialization
    fftParams.direction = direction;
    fftParams.flags = flags;
}


FFTManager::~FFTManager()
{
    free(fftParams.dataIn);
    free(fftParams.dataOut);
}


// ========= slots =========
void FFTManager::update()
{
    calculateFFT(fftData.iVecFFTs, fftData.oVecFFTs);
}


// ========= setters =========
void FFTManager::setNumFFTChannels(unsigned int numFFTChannels)
{
    qDebug() << "Setting number of FFT channels";

    // Set number of FFT channels
    fftParams.numFFTChannels = numFFTChannels;

    // Resize FFT buffers
    fftData.iVecFFTs.resize(numFFTChannels);
    fftData.oVecFFTs.resize(numFFTChannels);

    // Resize FFT buffer channels
    setFFTSize(getFFTSize());
}


int FFTManager::setFFTSize(unsigned int fftSize)
{
    qDebug() << "Setting size of FFT";

    // Check if fftSize works
    if (!isPowerOfTwo(fftSize))
    {
        qDebug() << "fftSize of" << fftSize << "isn't power of 2";
        return -1;
    }

    // Set FFT size
    fftParams.fftSize = fftSize;

    // Resize input/output FFT channels
    #pragma omp parallel for
    for (std::vector<float>& iVecFFT : fftData.iVecFFTs)
        iVecFFT.resize(fftSize);
    #pragma omp parallel for
    for (std::vector<float>& oVecFFT : fftData.oVecFFTs)
        oVecFFT.resize(fftSize);

    // Configure FFTW
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

    return 0;
}


// ========= getters =========
unsigned int FFTManager::getNumFFTChannels()
{
    return fftParams.numFFTChannels;
}


unsigned int FFTManager::getFFTSize()
{
    return fftParams.fftSize;
}


// ========= math =========
void FFTManager::calculateFFT
(
    std::vector<std::vector<float>>& iVecFFTs,
    std::vector<std::vector<float>>& oVecFFTs
)
{
    for (int i = 0; i < iVecFFTs.size(); i++)
        calculateFFT(iVecFFTs[i], oVecFFTs[i]);
}


void FFTManager::calculateFFT
(
    std::vector<float>& iVecFFT,
    std::vector<float>& oVecFFT
)
{
    #pragma omp parallel for
    // copy vecIn into dataIn
    for (int i = 0; i < fftParams.fftSize; i++)
    {
        fftParams.dataIn[i][0] = (double) iVecFFT[i];
        fftParams.dataIn[i][1] = 0.0;
    }

    fftw_execute(fftParams.fftPlan);

    #pragma omp parallel for
    // copy vecOut magnitude into dataOut
    for (int i = 0; i < fftParams.fftSize; i++)
        oVecFFT[i] = (float) std::sqrt
        (
            std::pow(fftParams.dataOut[i][0], 2) +
            std::pow(fftParams.dataOut[i][1], 2)
        );
}
