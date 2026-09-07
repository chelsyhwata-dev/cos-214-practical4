#ifndef SECURITYAUDITFLAGDECORATOR_H
#define SECURITYAUDITFLAGDECORATOR_H
#include "WorkItemDecorator.h"

/**
 * @brief Defines the SecurityAuditFlagDecorator, the concrete decorator
 * 
 * @details
 * SecurityAuditFlagDecorator is a ConcreteDecorator. It marks a WorkItem as requiring additional security auditing, such as tasks involving authetication or payment systems. Adds a fixed security audit overhead of 1.5hrs to the estimated completion time and adds secruity related information when the WorkItem is displayed. The decorator can be stacked with other WorkItem decorators.
 */

 class SecurityAuditFlagDecorator : public WorkItemDecorator {
    public:
        explicit SecurityAuditFlagDecorator(WorkItem* item);
        double getEstimatedHours() const override;
        void display(int depth) const override;
        bool isEscalated() const override;
    
    private:
    static constexpr double AUDIT_OVERHEAD_HOURS = 1.5;
 };

 #endif