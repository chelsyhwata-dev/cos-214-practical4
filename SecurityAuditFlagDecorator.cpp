#include "SecurityAuditFlagDecorator.h"
#include <iostream>

SecurityAuditFlagDecorator::SecurityAuditFlagDecorator(WorkItem* item)
    : WorkItemDecorator(item) {
}

double SecurityAuditFlagDecorator::getEstimatedHours() const {
    return wrapped->getEstimatedHours() + AUDIT_OVERHEAD_HOURS;
}

void SecurityAuditFlagDecorator::display(int depth) const {
    wrapped->display(depth);

    std::cout << std::string(depth * 2, ' ')
              << " >> [" << wrapped->getName()
              << "] flagged for SECURITY AUDIT (+"
              << AUDIT_OVERHEAD_HOURS << "h)" << std::endl;
    
}

bool SecurityAuditFlagDecorator::isEscalated() const {
    return true;
}
