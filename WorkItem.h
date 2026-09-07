#ifndef WORKITEM_H

#define WORKITEM_H

#include <string>

class WorkItemIterator;

enum class IteratorType {
    PreOrder,
    Blocked
};

class WorkItem {
public:
    virtual ~WorkItem();

    virtual std::string getName() const= 0;

    virtual double getEstimatedHours()const= 0;

    virtual WorkItemIterator* createIterator(IteratorType type = IteratorType::PreOrder) const =0;

    virtual void display(int depth) const = 0;

    virtual bool isBlocked()const;

    virtual std::string getStateName()const;

    virtual bool replaceChild(WorkItem* oldItem, WorkItem* newItem);
};

#endif
