#ifndef WORKITEMDECORATOR_H
#define WORKITEMDECORATOR_H

/**
 * @brief Defines the abstract Decorator base class for WorkItem objects. 
 * 
 * @details
 * Implementation fo the base class for the Decorator design pattern. 
 * This class allows additional responsibilities or attributes to be added to a WorkItem at runtime without creating a seperate subclass for every possible combination of behaviours. By default, WorkItemDecorator forward all WorkItem operations to the wrapped object. Concrete decorators can override only the operations for which they provide additional or modified behaviour.
 */

#include "WorkItem.h"

class WorkItemDecorator : public WorkItem {
    public:
        explicit WorkItemDecorator(WorkItem* item);
        ~WorkItemDecorator() override;
        std::string getName() const override;
        double getEstimatedHours() const override;
        WorkItemIterator* createIterator() const override;
        void display(int depth) const override;
        bool isBlocked() const override;
        std::string getStateName() const override;

    protected:
        WorkItem* wrapped; //pointer owned by the WorkItemDecorator. The wrapped object is deleted by the decorators destructor.ConcreteDecorator's can use this pointer to access the underlying WorkItem when implementing additional behaviour.
};

#endif
