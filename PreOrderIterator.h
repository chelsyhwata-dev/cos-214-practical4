#ifndef PREORDERITERATOR_H
#define PREORDERITERATOR_H

#include "WorkItemIterator.h"
#include "WorkItem.h"

#include <vector>

class PreOrderIterator : public WorkItemIterator{
public:
    explicit PreOrderIterator(const std::vector<WorkItem*>& topLevelChildren);

    bool hasNext()const override;

    WorkItem* next() override;
    void first()override;

private:
    void collect(WorkItem* item);

    std::vector<WorkItem*> items;
    
    size_t index;
};

#endif
