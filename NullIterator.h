#ifndef NULLITERATOR_H
#define NULLITERATOR_H

#include "WorkItemIterator.h"

class NullIterator : public WorkItemIterator {
public:
    bool hasNext() const override;
    WorkItem* next() override;
    void first() override;
};

#endif
