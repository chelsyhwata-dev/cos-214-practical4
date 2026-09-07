#ifndef WORKGROUP_H

#define WORKGROUP_H


#include "WorkItem.h"
#include <vector>

#include <string>

class WorkGroup : public WorkItem {
public:
    explicit WorkGroup(const std::string&name);

    ~WorkGroup()override;

    void add(WorkItem*item);
    bool remove(WorkItem* item);

    std::string getName() const override;

    double getEstimatedHours()const override;

    WorkItemIterator* createIterator(IteratorType type = IteratorType::PreOrder)const override;
    void display(int depth) const override;

    size_t childCount() const;
    bool replaceChild(WorkItem* oldItem, WorkItem* newItem) override;

private:
    std::string name;
    std::vector<WorkItem*>children;
};

#endif
