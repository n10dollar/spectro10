#include "utils.h"

int highestMatchingSampleRate
(
    const std::vector<unsigned int>& sRateInput,
    const std::vector<unsigned int>& sRateOutput
)
{
    std::vector<unsigned int> inputCopy(sRateInput);
    std::vector<unsigned int> outputCopy(sRateOutput);

    std::sort(inputCopy.begin(), inputCopy.end());
    std::sort(outputCopy.begin(), outputCopy.end());

    int iInput = inputCopy.size();
    int iOutput = outputCopy.size();

    while (iInput >= 0 && iOutput >= 0)
    {
        if (sRateInput[iInput] == sRateInput[iOutput])
            return sRateInput[iInput];
        else if (sRateInput[iInput] < sRateInput[iOutput])
            iOutput--;
        else if (sRateInput[iInput] > sRateInput[iOutput])
            iInput--;
    }

    // no match: -1
    return -1;
}
