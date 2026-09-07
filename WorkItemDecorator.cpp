#include "WorkItemDecorator.h"

WorkItemDecorator::WorkItemDecorator(WorkItem* item) : wrapped(item){}

WorkItemDecorator::~WorkItemDecorator() {
    delete wrapped;
}

std::string WorkItemDecorator::getName() const {
    return wrapped->getName();
}

double WorkItemDecorator::getEstimatedHours() const {
    return wrapped->getEstimatedHours();
}

WorkItemIterator* WorkItemDecorator::createIterator(IteratorType type) const {
    return wrapped->createIterator();
}

void WorkItemDecorator::display(int depth) const {
    wrapped->display(depth);
}

bool WorkItemDecorator::isBlocked() const {
    return wrapped->isBlocked();
}

std::string WorkItemDecorator::getStateName() const {
    return wrapped->getStateName();
}