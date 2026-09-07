#include "WorkGroup.h"
#include "PreOrderIterator.h"
#include "BlockedWorkIterator.h"
#include <algorithm>
#include <iostream>

WorkGroup::WorkGroup(const std::string&groupName):name(groupName){
}

WorkGroup::~WorkGroup() {
    for(WorkItem*child:children){
        delete child;
    }
}

void WorkGroup::add(WorkItem* item) {
    children.push_back(item);
}

bool WorkGroup::remove(WorkItem*item) {
    auto newEnd=std::remove(children.begin(),children.end(),item);
    if(newEnd==children.end()){
        return false;//bc item wasnt here
    }
    children.erase(newEnd, children.end());
    return true;
}

std::string WorkGroup::getName() const {
    return name;
}

double WorkGroup::getEstimatedHours() const {
    double tot=0.0;
    for(const WorkItem*kid:children){
        tot+=kid->getEstimatedHours();
    }
    return tot;
}

WorkItemIterator* WorkGroup::createIterator(IteratorType type) const {
    switch (type) {
        case IteratorType::Blocked:
            return new BlockedWorkIterator(children);
        case IteratorType::PreOrder:
        default:
            return new PreOrderIterator(children);
    }
}

void WorkGroup::display(int depth) const {
    std::cout<< std::string(depth*2,' ')<<"[Group] "<<name<<" (rolled-up estimate: "<<getEstimatedHours()<<"h)"<<std::endl;
    for(const WorkItem*child:children){
        child->display(depth+1);
    }
}

size_t WorkGroup::childCount() const {
    return children.size();
}

bool WorkGroup::replaceChild(WorkItem* oldItem, WorkItem* newItem){
    for (WorkItem*& child : children){
        if(child == oldItem){
            child = newItem;
            return true;
        }
    }

    for(WorkItem* child : children){
        if(child->replaceChild(oldItem, newItem)){
            return true;
        }
    }

    return false;
}
