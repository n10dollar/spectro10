#include "audioprocessor.h"

#include <QDebug>

#include "utils.h"

AudioProcessor::AudioProcessor(QObject *parent)
    : QObject{parent}, fftManager(&audioData.iVecBuffersNormalized, streamManager.streamData.numInputChannels)
{
    audioData.streamData = &streamManager.streamData;
    audioData.fftData = &fftManager.fftData;

    // Init iVecBuffersNormalized
    audioData.iVecBuffersNormalized.resize(audioData.streamData->numInputChannels);
    for (auto& iBuffer : audioData.iVecBuffersNormalized)
        iBuffer.resize(audioData.streamData->bufferSize);

    // Init oVecBuffersNormalized
    audioData.oVecBuffersNormalized.resize(audioData.streamData->numOutputChannels);
    for (auto& oBuffer : audioData.oVecBuffersNormalized)
        oBuffer.resize(audioData.streamData->bufferSize);

    audioData.iVecBufferAvg.resize(audioData.streamData->bufferSize);
    audioData.oVecBufferAvg.resize(audioData.streamData->bufferSize);

    audioData.oVecFiltered.resize(audioData.streamData->bufferSize);

    audioData.oVecFFTAvg.resize(audioData.fftData->fftSize);

    audioData.vecOscilliscope.resize(audioData.streamData->bufferSize);
    // Cut off at Nyquist bin
    audioData.vecSpectrogram.resize(audioData.fftData->fftSize / 2);

    // Create filters
    cycfi::q::lowpass filterLP(2000_Hz, (float) audioData.streamData->bufferSize, .707);
    cycfi::q::highpass filterHP(2000_Hz, (float) audioData.streamData->bufferSize, .707);

    // Add filters
    filterData.filters.push_back(filterLP);
    filterData.filters.push_back(filterHP);
}


void AudioProcessor::update()
{
    streamBuffersNormalize();

    calculateAverages();

    filterStream(audioData.iVecBufferAvg, audioData.oVecFiltered);

    fftManager.update();

    processOscilliscope();
    processSpectrogram();
}


void AudioProcessor::streamBuffersNormalize()
{
    for (int c = 0; c < audioData.streamData->numInputChannels; c++)
        for (int s = 0; s < audioData.streamData->bufferSize; s++)
            audioData.iVecBuffersNormalized[c][s] = (audioData.streamData->iVecBuffers[c][s] + 1.0f) / 2.0f;

    // qDebug() << "Stream pt (float) "
    //          << audioData.streamData->bufferSize / 2 << ": "
    //          << audioData.iVecBuffersNormalized[0][audioData.streamData->bufferSize / 2];

    // for (int c = 0; c < audioData.streamData->numOutputChannels; c++)
    //     for (int s = 0; s < audioData.streamData->bufferSize; s++)
    //         audioData.oVecBuffersNormalized[c][s] = (audioData.streamData->iVecBuffers[c][s] + 1.0f) / 2.0f;
}


void AudioProcessor::calculateAverages()
{
    averageVectors<float>
    (
        audioData.iVecBuffersNormalized,
        audioData.iVecBufferAvg,
        audioData.streamData->numInputChannels,
        audioData.streamData->bufferSize
    );

    averageVectors<float>
    (
        audioData.oVecBuffersNormalized,
        audioData.oVecBufferAvg,
        audioData.streamData->numOutputChannels,
        audioData.streamData->bufferSize
    );

    averageVectors<float>
    (
        audioData.fftData->oVecFFTs,
        audioData.oVecFFTAvg,
        audioData.fftData->numFFTBuffers,
        audioData.fftData->fftSize
    );
}


void AudioProcessor::processOscilliscope()
{
    for (int i = 0; i < audioData.vecOscilliscope.size(); i++)
        audioData.vecOscilliscope[i] = audioData.iVecBufferAvg[i];
}


void AudioProcessor::processSpectrogram()
{
    for (int i = 0; i < audioData.vecSpectrogram.size(); i++)
        audioData.vecSpectrogram[i] = audioData.oVecFFTAvg[i] / audioData.fftData->fftSize;
}


void AudioProcessor::filterStream
(
    std::vector<float>& iVecFilter,
    std::vector<float>& oVecFilter
)
{
    // Filter signal
    for (int i = 0; i < iVecFilter.size(); i++)
    {
        float centeredPt = 2.0f * iVecFilter[i] - 1.0f;
        for (auto& filter : filterData.filters)
            centeredPt = filter(centeredPt);
        float unCenteredPt = (centeredPt + 1.0f) / 2.0f;
        oVecFilter[i] = unCenteredPt;
    }

    // qDebug() << oVecFilter[iVecFilter.size() / 2];
}
