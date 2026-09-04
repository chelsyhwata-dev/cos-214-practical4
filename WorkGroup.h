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

    WorkItemIterator* createIterator()const override;
    void display(int depth) const override;

    size_t childCount() const;

private:
    std::string name;
    std::vector<WorkItem*>children;
};

#endif
