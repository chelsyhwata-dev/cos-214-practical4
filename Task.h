#ifndef TASK_H
#define TASK_H

class TaskState;
/**
 * @details
 * Task represents a single development ticket, such as "FIX login bug". It is the Leaf Component of the Composite Pattern and can therefore be stored inside a WorkGroup alogside other Tasks or nested WorkGroups.
 * 
 * Task also acts as the Context of the State pattern. It maintians a taskState object and delegates lifecycle operations(such as starting, blocking, approving, and reopening) to the current state. this allows the behaviour of a task to change at runtime without placing state-specific logic inside the Task class itself. 
 */

 #include <string>
 #include "WorkItem.h"

 //A newly created Task starts in the Backlog state.

 class Task : public WorkItem{
    public:
        //taskName is the name or description of the current task and the estimatedHours is the estimated number of hours required to complete the task.
        Task(const std::string& taskName, double estimatedHours);
        ~Task() override;
        std::string getName() const override;
        double getEstimatedHours() const override;

        /** @brief Creates an iterator for the task's children. 
         * @return A nullIterator because a task has no children WorkItems.
         * @details Returning a NullIterator allwos client code to interact with a task through the WorkItem interfcae without needing to check whether the object is a leaf. 
         */
        WorkItemIterator* createIterator(IteratorType type = IteratorType::PreOrder)const override;
        void display(int depth) const override;
        bool isBlocked() const override;
        std::string getStateName() const override;
        void start(); //Backlog -> InProgress
        void submitForReview(); //InProgress ->InReview
        void approve(); //InReview -> Done
        void reject(); // InReview ->InProgress (sent back for changes)
        void block(); //InProgress ->Blocked
        void unblock(); //Blocked ->InProgress
        void reopen(); //Done -> InProgress (bug was found later)

        //Used BY the state classes to actually change task's state pointer. Client code should not call this directly, it should call the action methods above instead.
        void setState(TaskState* newState);
    
    private:
        std::string name;
        double estimatedHours;
        TaskState* state; // task owns this pointer and deletes it on destruction or when transitioning to a new state.
 };

 #endif