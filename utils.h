#ifndef UTILS_H
#define UTILS_H

#include <QDebug>

#include <vector>
#include <cmath>
#include <climits>

#define SR_REF 40000
int closestMatchingSampleRate
(
    const std::vector<unsigned int>& sRateInput,
    const std::vector<unsigned int>& sRateOutput,
    int srRef = SR_REF
)
{
    std::vector<unsigned int> inputSortedCopy(sRateInput);
    std::vector<unsigned int> outputSortedCopy(sRateOutput);

    std::sort(inputSortedCopy.begin(), inputSortedCopy.end());
    std::sort(outputSortedCopy.begin(), outputSortedCopy.end());

    std::vector<int> matchingSampleRates;

    int iInput = inputSortedCopy.size() - 1;
    int iOutput = outputSortedCopy.size() - 1;

    while (iInput >= 0 && iOutput >= 0)
    {
        if (inputSortedCopy[iInput] == outputSortedCopy[iOutput])
        {
            matchingSampleRates.push_back(sRateInput[iInput]);
            iOutput--;
            iInput--;
        }
        else if (inputSortedCopy[iInput] < outputSortedCopy[iOutput])
            iOutput--;
        else if (inputSortedCopy[iInput] > outputSortedCopy[iOutput])
            iInput--;
    }

    qDebug() << "Matching Sample Rates:";
    qDebug() << matchingSampleRates;

    // no match: -1
    int closest40 = -1;
    int minAbsVal = INT_MAX;

    for (int sampleRate : matchingSampleRates)
        if (std::abs(sampleRate - srRef) < minAbsVal)
        {
            minAbsVal = std::abs(sampleRate - srRef);
            closest40 = sampleRate;
        }

    qDebug() << "Closest matching sample rate to" << srRef << ":" << closest40;
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
        vecAvg[i] = (T) avgPt;
    }
}


template<typename T>
bool vectorContains(std::vector<T>& vec, T ref)
{
    if (std::find(vec.begin(), vec.end(), ref) == vec.end())
        return false;
    return true;
}

#endif // UTILS_H
