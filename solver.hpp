#include <vector>

enum patternState
{
    STATE_UNCERTAIN = -1,
    STATE_EMPTY = 0,
    STATE_FILL = 1
};

typedef std::vector<patternState> patterns_t;
typedef std::vector<int> clues_t;

class nonogramSolver
{
protected:
    int mTotalLength;
    clues_t mClues;
    bool checkObvious();
    bool checkFixed();
    bool checkPartial();
public:
    nonogramSolver();
    nonogramSolver(int totalLength, clues_t clues);
    ~nonogramSolver();
    bool solve();
    bool setLength(int totalLength);
    bool overWriteClues(const clues_t& clues);
private:
    bool isUpdated;
protected:
    patterns_t mPattern;
public:
    unsigned int countFilledBox() const;
    unsigned int countEmptyBox() const;
    unsigned int countUncertainBox() const;
    int operator [] (int index) const;
protected:
    int sumClueFill() const;
    bool checkPartialRecursive(patterns_t& possibility, int cluesIndex, int locateIndex);
    bool validatePattern();
    bool completedPattern();
public:
    bool overwritePattern(patterns_t& pattern);
};
