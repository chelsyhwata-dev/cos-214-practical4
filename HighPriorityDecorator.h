#ifndef HIGHPRIORITYDECORATOR_H
#define HIGHPRIORITYDECORATOR_H
#include "WorkItemDecorator.h"

/** @brief Defines the HighPriorityDecorator concrete decorator
 * 
 * @details 
 * HighPriorityDecorator is a concrete Decorator in the decorator design pattern. It marks a WorkItem as high priority, such as a critical production bug that requires urgent attention. 
 * 
 * The decorator adds an "[URGENT]" prefix to the OwrkItem's name and adds an additional indication when the OwkItem is displayed. Other WorkItem operations, including estimated hours, state, blocked status, and iteration, are forwarded unchnaged to the wrapped objects. 
  */

class HighPriorityDecorator : public WorkItemDecorator {
    explicit HighPriorityDecorator(WorkItem* item);

    std::string getName() const override;
    void display(int depth) const override;
};

#endif