#ifndef UTILS_H
#define UTILS_H

#include <vector>

#define SR_REF 40000
int closestMatchingSampleRate
(
    const std::vector<unsigned int>& sRateInput,
    const std::vector<unsigned int>& sRateOutput,
    int srRef = SR_REF
);

#endif // UTILS_H
