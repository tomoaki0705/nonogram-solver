#include "solver.hpp"
#include <cassert>
#include <numeric>


nonogramSolver::nonogramSolver()
    : mTotalLength(0)
    , isUpdated(false)
{
}

void initPattern(int totalLength, patterns_t& pattern)
{
    pattern = patterns_t(totalLength, STATE_UNCERTAIN);
}

nonogramSolver::nonogramSolver(int totalLength, clues_t clues)
    : mTotalLength(totalLength)
    , isUpdated(false)
{
    overWriteClues(clues);
    initPattern(mTotalLength, mPattern);
}


nonogramSolver::~nonogramSolver()
{
}


bool nonogramSolver::solve()
{
    bool solved = false;
    isUpdated = false;
    if (solved == false)
    {
        solved = checkObvious();
    }
    if (solved == false)
    {
        solved = checkFixed();
    }
    if (solved == false)
    {
        solved = checkPartial();
    }
    return solved;
}


bool nonogramSolver::setLength(int totalLength)
{
    mTotalLength = totalLength;
    initPattern(totalLength, mPattern);
    return true;
}


bool nonogramSolver::overWriteClues(const clues_t& clues)
{
    assert(clues.empty() == false);
    mClues.clear();
    for (auto&& it = clues.begin(); it < clues.end(); it++)
    {
        mClues.push_back(*it);
    }
    return false;
}

bool nonogramSolver::checkPartial()
{
    if (mClues.size() == 1 && mTotalLength < mClues[0] * 2)
    {
        patterns_t possibility(mPattern);
        bool result = checkPartialRecursive(possibility, 0, 0);
        mPattern = possibility;
        return countUncertainBox() == 0 ? true : false;
    }
    else
    {
        patterns_t possibility(mPattern);
        bool result = checkPartialRecursive(possibility, 0, 0);
        if (result)
        {
            mPattern = possibility;
            if (completedPattern())
            {
                for (auto it = mPattern.begin(); it < mPattern.end(); it++)
                {
                    if (*it == STATE_UNCERTAIN)
                    {
                        *it = STATE_EMPTY;
                    }
                }
            }
            return countUncertainBox() == 0 ? true : false;
        }
    }
    return false;
}

bool nonogramSolver::checkFixed()
{
    if ((sumClueFill() + mClues.size() - 1) == mTotalLength)
    {
        size_t index = 0;
        for (auto&& it = mClues.begin(); it != mClues.end(); it++)
        {
            for (size_t i = index; i < index + *it; i++)
            {
                mPattern[i] = STATE_FILL;
            }
            if (index + *it < mTotalLength)
            {
                mPattern[index + *it] = STATE_EMPTY;
            }
            index += *it + 1;
        }
        return true;
    }
    return false;
}

bool nonogramSolver::checkObvious()
{
    if (mClues.size() == 1 && mClues[0] == mTotalLength)
    {
        for (size_t i = 0; i < mTotalLength; i++)
        {
            mPattern[i] = STATE_FILL;
        }
        return true;
    }
    if (mClues.size() == 1 && mClues[0] == 0)
    {
        for (size_t i = 0; i < mTotalLength; i++)
        {
            mPattern[i] = STATE_EMPTY;
        }
        return true;
    }
    return false;
}

template<typename T>
unsigned int countInVector(const patterns_t& v, T k)
{
    return (unsigned int)std::count(v.begin(), v.end(), k);
}

unsigned int nonogramSolver::countUncertainBox() const
{
    return countInVector(mPattern, STATE_UNCERTAIN);
}


unsigned int nonogramSolver::countFilledBox() const
{
    return countInVector(mPattern, STATE_FILL);
}


unsigned int nonogramSolver::countEmptyBox() const
{
    return countInVector(mPattern, STATE_EMPTY);
}


int nonogramSolver::sumClueFill() const
{
    return (int)std::accumulate(mClues.begin(), mClues.end(), 0);
}

int nonogramSolver::operator [] (int index) const
{
    assert(index < mTotalLength);
    return mPattern[index];
}

bool nonogramSolver::checkPartialRecursive(patterns_t & possibility, int cluesIndex, int locateIndex)
{
    bool placable = false;
    bool firstLoop = true;
    bool possibleToAssign = false;
    patterns_t possibilityLocal(possibility);
    for (size_t i = locateIndex; i <= mTotalLength - mClues[cluesIndex]; i++)
    {
        bool assignedFlag = true;
        // check if this block can be placed (no conflicts with EMPTY)
        for (size_t j = 0; j < mClues[cluesIndex]; j++)
        {
            if (possibilityLocal[i + j] == STATE_EMPTY)
            {
                assignedFlag = false;
                break;
            }
        }
        if (assignedFlag && 1 <= i && possibility[i - 1] == STATE_FILL)
        {
            assignedFlag = false;
        }
        if (assignedFlag && i + mClues[cluesIndex] < mTotalLength && possibility[i + mClues[cluesIndex]] == STATE_FILL)
        {
            assignedFlag = false;
        }
        // if conflicts, try next location
        if (assignedFlag == false)
        {
            continue;
        }
        patterns_t work(possibility);
        for (size_t j = 0; j < mClues[cluesIndex]; j++)
        {
            work[i + j] = STATE_FILL;
        }
        // if more clues, try another step
        if (cluesIndex < mClues.size() - 1)
        {
            bool result = checkPartialRecursive(work, cluesIndex + 1, i + mClues[cluesIndex] + 1);
            if (result == false)
            {
                // this loaction conflicts for latter clues
                continue;
            }
        }
        if (sumClueFill() < countInVector(work, STATE_FILL))
        {
            // filled more than the clues
            continue;
        }
        possibleToAssign = true;
        if (firstLoop)
        {
            firstLoop = false;
            possibilityLocal = work;
        }
        else
        {
            // verify with the possibilityLocal
            for (size_t j = 0; j < possibilityLocal.size(); j++)
            {
                if (work[j] != STATE_FILL || possibilityLocal[j] != STATE_FILL)
                {
                    possibilityLocal[j] = STATE_UNCERTAIN;
                }
            }
            int a = 0;
        }
    }
    possibility = possibilityLocal;

    return possibleToAssign;
}


bool nonogramSolver::overwritePattern(patterns_t& pattern)
{
    assert(pattern.size() == mPattern.size());
    mPattern = pattern;
    return true;
}


bool nonogramSolver::completedPattern()
{
    int counter = 0;
    clues_t verifyPattern;
    bool isCompleted = true;
    enum patternState state = STATE_UNCERTAIN;
    for (auto it = mPattern.begin(); it != mPattern.end(); it++)
    {
        switch (*it)
        {
        case STATE_FILL:
            counter++;
            state = STATE_FILL;
            break;
        case STATE_EMPTY:
        case STATE_UNCERTAIN:
        default:
            if (state == STATE_FILL)
            {
                verifyPattern.push_back(counter);
                counter = 0;
                state = *it;
            }
            break;
            break;
        }
    }
    if (mClues.size() != verifyPattern.size())
    {
        isCompleted = false;
    }
    if (isCompleted)
    {
        for (size_t i = 0; i < mClues.size(); i++)
        {
            if (mClues[i] != verifyPattern[i])
            {
                isCompleted = false;
                break;
            }
        }
    }
    return isCompleted;
}

bool nonogramSolver::validatePattern()
{
    if (sumClueFill() < countInVector(mPattern, STATE_FILL))
    {
        // already filled more than the clues
        return false;
    }
    return true;
}
