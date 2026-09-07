#include "CodeReviewRequiredDecorator.h"
#include <iostream>

CodeReviewRequiredDecorator::CodeReviewRequiredDecorator(WorkItem* item)
    : WorkItemDecorator(item) {
}

double CodeReviewRequiredDecorator::getEstimatedHours() const {
    // Take whatever the wrapped item reports and add our own
    // fixed overhead on top - this is the "adjustment" a
    // decorator is meant to provide.
    return wrapped->getEstimatedHours() + REVIEW_OVERHEAD_HOURS;
}

void CodeReviewRequiredDecorator::display(int depth) const {
    std::cout << std::string(depth * 2, ' ')
              << "  >> requires code review (+"
              << REVIEW_OVERHEAD_HOURS << "h)" << std::endl;
    wrapped->display(depth);
}
