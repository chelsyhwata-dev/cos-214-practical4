#include "WorkItem.h"

WorkItem::~WorkItem() {}

bool WorkItem::isBlocked() const {
    return false;
}

std::string WorkItem::getStateName() const {
    return "-";
}

bool WorkItem::replaceChild(WorkItem*, WorkItem*){
    return false;
}
