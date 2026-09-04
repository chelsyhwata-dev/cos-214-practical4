#include "PreOrderIterator.h"

PreOrderIterator::PreOrderIterator(const std::vector<WorkItem*>& topLevelChildren) {
    for(WorkItem*item:topLevelChildren){
        collect(item);
    }
    index=0;
}

bool PreOrderIterator::hasNext() const {
    return index<items.size();
}

WorkItem* PreOrderIterator::next() {
    return items[index++];
    
}

void PreOrderIterator::first() {
    index=0;
}

void PreOrderIterator::collect(WorkItem* item) {
    items.push_back(item);
    WorkItemIterator*sb= item->createIterator();
    while(sb->hasNext()){
        items.push_back(sb->next());
    }
    delete sb;
}
