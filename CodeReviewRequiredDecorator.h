#ifndef CODEREVIEWREQUIREDDECORATOR_H
#define CODEREVIEWREQUIREDDECORATOR_H

/**
 * @brief Defines the CodeReviewRequiredDecorator concrete decorator. 
 * 
 * @details
 * It marks a WorkItem as requiring a formal code review before thr work can be considered complete. Teh decorator adds a fixed 2-hour review overhead to the estimated completion time and adds a code-review indication when the WorkItem is displayed. The decorator obviously cna be stacked with other WorkItem decorators. Each decorator contributes its own additional behaviour or overhead while preserving the WorkItem interface.
 */

#include "WorkItemDecorator.h"

class CodeReviewRequiredDecorator : public WorkItemDecorator {

public: 
    explicit CodeReviewRequiredDecorator(WorkItem* item);
    double getEstimatedHours() const override;
    void display(int depth) const override;

private: 
    static constexpr double REVIEW_OVERHEAD_HOURS = 2.0;

};

#endif