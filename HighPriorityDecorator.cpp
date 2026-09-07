#include "HighPriorityDecorator.h"
#include <iostream>

HighPriorityDecorator::HighPriorityDecorator(WorkItem* item)
    : WorkItemDecorator(item) {
}

std::string HighPriorityDecorator::getName() const {
    return "[URGENT] " + wrapped->getName();
}

void HighPriorityDecorator::display(int depth) const {
    // Print our own extra marker line, then let the wrapped
    // item (or the next decorator down the stack) print itself
    // as normal. This is how stacked decorators each get to add
    // their own line without needing to know about each other.
    std::cout << std::string(depth * 2, ' ')
              << "  >> flagged HIGH PRIORITY" << std::endl;
    wrapped->display(depth);
}
