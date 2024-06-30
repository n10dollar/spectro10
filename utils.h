#ifndef UTILS_H
#define UTILS_H

#include <QDebug>

#include <vector>
#include <set>
#include <cmath>
#include <algorithm>

template<typename T>
T closestMatching
(
    const std::vector<std::vector<T>>& vecsData,
    T ref
)
{
    std::set<T> matchingData;
    for (std::vector<T>& vecData : vecsData)
        matchingData.insert(vecData.begin(), vecData.end());

    // Check if vecsData is empty
    if (matchingData.empty())
    {
        qWarning() << "closestMatching() input data empty";
        qWarning() << "returning default-initialized value";
        return T();
    }

    for (std::vector<T>& vecData : vecsData)
    {
        std::set<T> vecDataSet(vecData.begin(), vecData.end());

        std::set<T> intersection;
        std::set_intersection
        (
            matchingData.begin(),
            matchingData.end(),
            vecDataSet.begin(),
            vecDataSet.end(),
            std::inserter(intersection, intersection.begin())
        );

        matchingData = intersection;
    };

    T closestPoint = std::min_element
    (
        matchingData.begin(),
        matchingData.end(),
        [ref](T a, T b) { return std::abs(a - ref) < std::abs(b - ref); }
    );

    return closestPoint;
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


template<typename T>
bool isPowerOfTwo(T num)
{
    return (num > 0) && ((num & (num - 1)) == 0);
}

#endif // UTILS_H
