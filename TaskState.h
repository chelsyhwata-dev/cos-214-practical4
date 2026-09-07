#ifndef TASKSTATE_H
#define TASKSTATE_H

/**
 *  @brief Defines the abstract and concrete states used by the Task lifecycle.
 * 
 * @details
 * Implements the State pattern for the task objects. A task progresses through several lifecycle states, like Backlog -> InProgress -> InRevieww -> Done. Lifecycle operations are delegated to the current state, allowing each state to determine which transitions are valid. Invalid lifecycle operations use the default behaviour provided by TaskState rather than causing the program to crash or silently ignoring the operation. 
 * 
 */

 #include <string>

 class Task; 

 class TaskState {
    public:
        virtual ~TaskState();
        virtual void start(Task* task);
        virtual void submitForReview(Task* task);
        virtual void approve(Task* task);
        virtual void reject(Task* task);
        virtual void block(Task* task);
        virtual void unblock(Task* task);
        virtual void reopen(Task* task);

        virtual std::string getName() const=0;

        virtual bool isBlocked() const;
 };

 //CONCRETE STATES. 

 class BacklogState : public TaskState {
    public:
        void start(Task* task) override;
        std::string getName() const override;
 };

 class InProgressState : public TaskState {
    public:
        void submitForReview(Task* task) override;
        void block(Task* task) override;
        std::string getName() const override;
 };

 class InReviewState : public TaskState {
    public:
        void approve(Task* task) override;
        void reject(Task* task) override;
        std::string getName() const override;
 };

 class BlockedState : public TaskState {
    public:
        void unblock(Task* task) override;
        bool isBlocked() const override;
        std::string getName() const override; 
 };

 class DoneState : public TaskState {
    public: 
        void reopen(Task* task) override;
        std::string getName() const override;
 };

 #endif 

