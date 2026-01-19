#include "profiler.h"

void Profiler::record(size_t pc) {
    execution_counts_[pc]++;
}

int Profiler::getCount(size_t pc) const {
    auto it = execution_counts_.find(pc);
    return it != execution_counts_.end() ? it->second : 0;
}

bool Profiler::isHot(size_t pc, int threshold) const {
    return getCount(pc) > threshold;
}
