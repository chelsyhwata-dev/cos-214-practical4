#include "BlockedWorkIterator.h"

BlockedWorkIterator::BlockedWorkIterator(const std::vector<WorkItem*>& topLevelChildren) {
    for (WorkItem* item : topLevelChildren) {
        collect(item);
    }
    index = 0;
}

bool BlockedWorkIterator::hasNext() const {
    return index < items.size();
}

WorkItem* BlockedWorkIterator::next() {
    return items[index++];
}

void BlockedWorkIterator::first() {
    index = 0;
}

void BlockedWorkIterator::collect(WorkItem* item) {
    if (item->isBlocked()) {
        items.push_back(item);
    }
    WorkItemIterator* sub = item->createIterator();
    while (sub->hasNext()) {
        WorkItem* child = sub->next();
        if (child->isBlocked()) {
            items.push_back(child);
        }
    }
    delete sub;
}