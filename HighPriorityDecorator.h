/* ================================================================
 * FILE:          HighPriorityDecorator.h / .cpp
 * OWNER:         Person 2
 * PATTERN(S):    Decorator (1 of 3 concrete decorators)
 * DOMAIN ROLE:   Flags a ticket as urgent - the kind of thing a
 *                team lead would mark on a critical production bug.
 * HOW IT FITS:   Wraps any WorkItem (normally a Task). Only changes
 *                getName() (adds "[URGENT]" prefix) and display()
 *                (adds an extra line). Leaves hours, state, and
 *                iteration completely untouched - shows a decorator
 *                doesn't have to touch every method.
 * STATUS:        Complete and tested.
 * STILL NEEDED:  Person 3: use this in at least one demo scenario
 *                so a decorated object visibly "participates in
 *                normal system behaviour" per the spec (Task 3).
 * ================================================================ */

#ifndef HIGHPRIORITYDECORATOR_H
#define HIGHPRIORITYDECORATOR_H

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
