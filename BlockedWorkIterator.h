#ifndef BLOCKEDWORKITERATOR_H
#define BLOCKEDWORKITERATOR_H

#include "WorkItemIterator.h"
#include "WorkItem.h"
#include <vector>

class BlockedWorkIterator : public WorkItemIterator {
public:
    explicit BlockedWorkIterator(const std::vector<WorkItem*>& topLevelChildren);

    bool hasNext() const override;
    WorkItem* next() override;
    void first() override;

private:
    void collect(WorkItem* item);

    std::vector<WorkItem*> items;
    size_t index;
};

#endif