#include "TaskState.h"
#include "Task.h"
#include <iostream>

TaskState::~TaskState(){}

//default "invalid transitions" behaviour

void TaskState::start(Task* task) {
    std::cout << "Cannot start task \"" << task->getName()
              << "\" from state " << getName() << std::endl;
}
void TaskState::submitForReview(Task* task) {
    std::cout << "Cannot submit task \"" << task->getName()
              << "\" for review from state " << getName() << std::endl;
}
void TaskState::approve(Task* task) {
    std::cout << "Cannot approve task \"" << task->getName()
              << "\" from state " << getName() << std::endl;
}
void TaskState::reject(Task* task) {
    std::cout << "Cannot reject task \"" << task->getName()
              << "\" from state " << getName() << std::endl;
}
void TaskState::block(Task* task) {
    std::cout << "Cannot block task \"" << task->getName()
              << "\" from state " << getName() << std::endl;
}
void TaskState::unblock(Task* task) {
    std::cout << "Cannot unblock task \"" << task->getName()
              << "\" - it isn't blocked (currently " << getName() << ")" << std::endl;
}
void TaskState::reopen(Task* task) {
    std::cout << "Cannot reopen task \"" << task->getName()
              << "\" from state " << getName() << std::endl;
}

bool TaskState::isBlocked() const {
    return false; //only BlockedState overrides this to true
}

void BacklogState::start(Task* task) {
    std::cout << "Task \"" << task->getName() << "\" started." << std::endl;
    task->setState(new InProgressState());
}
std::string BacklogState::getName() const {
    return "Backlog";
}


void InProgressState::submitForReview(Task* task) {
    std::cout << "Task \"" << task->getName() << "\" submitted for review." << std::endl;
    task->setState(new InReviewState());
}
void InProgressState::block(Task* task) {
    std::cout << "Task \"" << task->getName() << "\" was blocked." << std::endl;
    task->setState(new BlockedState());
}
std::string InProgressState::getName() const {
    return "InProgress";
}


void InReviewState::approve(Task* task) {
    std::cout << "Task \"" << task->getName() << "\" approved and marked done." << std::endl;
    task->setState(new DoneState());
}
void InReviewState::reject(Task* task) {
    std::cout << "Task \"" << task->getName() << "\" changes requested - back to in progress." << std::endl;
    task->setState(new InProgressState());
}
std::string InReviewState::getName() const {
    return "InReview";
}

void BlockedState::unblock(Task* task) {
    std::cout << "Task \"" << task->getName() << "\" unblocked - back to in progress." << std::endl;
    task->setState(new InProgressState());
}
std::string BlockedState::getName() const {
    return "Blocked";
}
bool BlockedState::isBlocked() const {
    return true;
}


void DoneState::reopen(Task* task) {
    std::cout << "Task \"" << task->getName() << "\" reopened." << std::endl;
    task->setState(new InProgressState());
}
std::string DoneState::getName() const {
    return "Done";
}