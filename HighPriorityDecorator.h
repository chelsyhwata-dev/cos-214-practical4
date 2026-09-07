#ifndef HIGHPRIORITYDECORATOR_H
#define HIGHPRIORITYDECORATOR_H

/** @brief Defines the HighPriorityDecorator concrete decorator
 * 
 * @details 
 * HighPriorityDecorator is a concrete Decorator in the decorator design pattern. It marks a WorkItem as high priority, such as a critical production bug that requires urgent attention. 
 * 
 * The decorator adds an "[URGENT]" prefix to the OwrkItem's name and adds an additional indication when the OwkItem is displayed. Other WorkItem operations, including estimated hours, state, blocked status, and iteration, are forwarded unchnaged to the wrapped objects. 
  */

#include "WorkItemDecorator.h"

// ============================================================
// Marks a task/group as high priority.
// Changes: the displayed name gets a "[URGENT]" tag in front of
// it. Everything else (hours, iterator, state, blocked) is left
// exactly as the wrapped item reports it - we only override
// getName() and display().
// ============================================================
class HighPriorityDecorator : public WorkItemDecorator {
public:
    explicit HighPriorityDecorator(WorkItem* item);

    std::string getName() const override;
    void display(int depth) const override;
};

#endif
