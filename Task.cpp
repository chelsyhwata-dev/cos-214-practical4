#include "Task.h"
#include "TaskState.h"
#include "NullIterator.h"
#include <iostream>

Task::Task(const std::string& taskName, double hours)
    :name(taskName), estimatedHours(hours){
        state = new BacklogState();
}

Task::~Task(){
    delete state;
}

std::string Task::getName() const {
    return name;
}

double Task::getEstimatedHours() const {
    return estimatedHours;
}

WorkItemIterator* Task::createIterator() const {
    return new NullIterator();
}

void Task::display(int depth) const {
    std::cout << std::string(depth * 2, '  ') << "[Task] " << name << "  (" << estimatedHours << "h) - " << getStateName() << std::endl;
}

bool Task::isBlocked() const {
    //delegate to whatever state we're currently in.
    return state->isBlocked();
}

std::string Task::getStateName() const {
    return state->getName();
}

//Lifecycle operations that delegate to the current state.
void Task::start() {
    state->start(this);
}

void Task::submitForReview() {
    state->submitForReview(this);
}

void Task::approve() {
    state->approve(this);
}

void Task::reject() {
    state->reject(this);
}

void Task::block() {
    state->block(this);
}

void Task::unblock() {
    state->unblock(this);
}

void Task::reopen() {
    state->reopen(this);
}

void Task::setState(TaskState* newState) {
    delete state; //delete the old state
    state = newState; //set the new state
}


