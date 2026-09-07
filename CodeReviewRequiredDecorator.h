/* ================================================================
 * FILE:          CodeReviewRequiredDecorator.h / .cpp
 * OWNER:         Person 2
 * PATTERN(S):    Decorator (2 of 3 concrete decorators)
 * DOMAIN ROLE:   Marks a ticket as needing a formal code review
 *                before merging - adds the reviewer's time to the
 *                estimate rather than pretending it's free.
 * HOW IT FITS:   Wraps any WorkItem. Overrides getEstimatedHours()
 *                to add +2h on top of whatever the wrapped item
 *                reports, and display() to add a note. This is the
 *                decorator to point to when showing that stacking
 *                changes the numbers a group rolls up
 *                (WorkGroup::getEstimatedHours() sums these).
 * STATUS:        Complete and tested (verified rolled-up hours
 *                come out correct when stacked with other
 *                decorators).
 * STILL NEEDED:  Nothing further from Person 2 - ready to use in
 *                Person 3's demo scenarios.
 * ================================================================ */

#ifndef CODEREVIEWREQUIREDDECORATOR_H
#define CODEREVIEWREQUIREDDECORATOR_H

#include "WorkItemDecorator.h"

// ============================================================
// Marks a task as needing a formal code review before it can be
// considered done. Changes: adds a fixed amount of extra hours
// on top of the wrapped item's own estimate (to account for the
// reviewer's time), and adds a note when displayed.
// ============================================================
class CodeReviewRequiredDecorator : public WorkItemDecorator {
public:
    explicit CodeReviewRequiredDecorator(WorkItem* item);

    double getEstimatedHours() const override;
    void display(int depth) const override;

private:
    static constexpr double REVIEW_OVERHEAD_HOURS = 2.0;
};

#endif
