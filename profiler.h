#ifndef PROFILER_H
#define PROFILER_H

#include <map>

class Profiler {
public:
    void record(size_t pc);
    int getCount(size_t pc) const;
    bool isHot(size_t pc, int threshold = 100) const;

private:
    std::map<size_t, int> execution_counts_;
};

#endif // PROFILER_H
