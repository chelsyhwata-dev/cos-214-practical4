#ifndef WORKITEMITERATOR_H
#define WORKITEMITERATOR_H

class WorkItem;

class WorkItemIterator {
public:
    virtual ~WorkItemIterator();
    virtual bool hasNext() const= 0;

    virtual WorkItem* next() = 0;

    virtual void first() = 0;
};

#endif
