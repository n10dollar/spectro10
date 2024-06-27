#include "utils.h"

#include <QDebug>
#include <cmath>
#include <climits>

int closestMatchingSampleRate
(
    const std::vector<unsigned int>& sRateInput,
    const std::vector<unsigned int>& sRateOutput,
    int srRef
)
{
    std::vector<unsigned int> inputCopy(sRateInput);
    std::vector<unsigned int> outputCopy(sRateOutput);

    std::sort(inputCopy.begin(), inputCopy.end());
    std::sort(outputCopy.begin(), outputCopy.end());

    std::vector<int> matching;

    int iInput = inputCopy.size() - 1;
    int iOutput = outputCopy.size() - 1;

    while (iInput >= 0 && iOutput >= 0)
    {
        if (sRateInput[iInput] == sRateInput[iOutput])
        {
            matching.push_back(sRateInput[iInput]);
            iOutput--;
            iInput--;
        }
        else if (sRateInput[iInput] < sRateInput[iOutput])
            iOutput--;
        else if (sRateInput[iInput] > sRateInput[iOutput])
            iInput--;
    }

    // no match: -1
    int closest40 = -1;
    int minAbsVal = INT_MAX;

    for (int sampleRate : matching)
        if (std::abs(sampleRate - srRef) < minAbsVal)
        {
            minAbsVal = std::abs(sampleRate - srRef);
            closest40 = sampleRate;
        }

    qDebug() << "Closest matching sample rate to " << srRef << ": " << closest40;
    return closest40;
}


template<typename T>
void averageVectors
(
    std::vector<std::vector<T>>& vecs,
    std::vector<T>& vecAvg,
    int numVec,
    int length
)
{
    for (int i = 0; i < length; i++)
    {
        float sumPt = 0;
        for (auto& vec : vecs)
            sumPt += vec[i];
        float avgPt = sumPt / ((float) numVec);
        vecAvg.push_back((T) avgPt);
    }
}


template void averageVectors<float>
(
    std::vector<std::vector<float>>& vecs,
    std::vector<float>& vecAvg,
    int numVec,
    int length
);
