#include <iostream>
#include <vector>
#include "WorkGroup.h"
#include "Task.h"
#include "PreOrderIterator.h"
#include "BlockedWorkIterator.h"
#include "SecurityAuditFlagDecorator.h"

void showFullBoard(WorkGroup* epic) {
    std::cout << "\n-- Full Board (PreOrderIterator - complete traversal) --" << std::endl;
    epic->display(0);
}

void runEscalationWorkflow(WorkGroup* feature) {
    std::cout << "\n-- Escalation Sweep on '" << feature->getName() << "' --" << std::endl;

    std::vector<WorkItem*> snapshot;
    WorkItemIterator* it = feature->createIterator();
    while (it->hasNext()) {
        snapshot.push_back(it->next());
    }
    delete it;

    for (WorkItem* item : snapshot) {
        if (item->isBlocked()) {
            std::cout << " -> " << item->getName() << " is BLOCKED. Escalating..." << std::endl;

            WorkItem* decorated = new SecurityAuditFlagDecorator(item);
            std::cout << "    [LOG] Escalation logged for '" << item->getName() << "'" << std::endl;

            // Swap raw pointer for decorated one on the SAME parent - single ownership chain
            feature->remove(item);
            feature->add(decorated);
        }
    }
}

int main() {
    std::cout << "===================================" << std::endl;
    std::cout << " TaskForge - Software Delivery Demo" << std::endl;
    std::cout << "===================================" << std::endl;

    WorkGroup* epic = new WorkGroup("Epic: Checkout Revamp");
    WorkGroup* featureA = new WorkGroup("Feature: Payment Flow");
    WorkGroup* featureB = new WorkGroup("Feature: Cart UI");

    Task* task1 = new Task("Integrate payment gateway", 8.0);
    Task* task2 = new Task("Add retry logic", 4.0);
    Task* task3 = new Task("Redesign cart page", 5.0);

    featureA->add(task1);
    featureA->add(task2);
    featureB->add(task3);
    epic->add(featureA);
    epic->add(featureB);

    bool running = true;
    while (running) {
        std::cout << "\n--- What would you like to do? ---" << std::endl;
        std::cout << "1. View full board (full traversal)" << std::endl;
        std::cout << "2. Progress a task through its lifecycle" << std::endl;
        std::cout << "3. Attempt an INVALID transition (watch the console message)" << std::endl;
        std::cout << "4. Block a task, then run escalation sweep (blocked traversal + decorator)" << std::endl;
        std::cout << "5. Move a task between Features (runtime structural change)" << std::endl;
        std::cout << "6. Exit demo" << std::endl;
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            showFullBoard(epic);

        } else if (choice == 2) {
            std::cout << task1->getName() << " is currently: " << task1->getStateName() << std::endl;
            task1->start();
            task1->submitForReview();
            task1->approve();
            std::cout << "Final state: " << task1->getStateName() << std::endl;

        } else if (choice == 3) {
            std::cout << task3->getName() << " is currently: " << task3->getStateName() << std::endl;
            std::cout << "Attempting approve() while still in Backlog..." << std::endl;
            task3->approve(); // BacklogState only overrides start() - base TaskState::approve() prints a rejection message
            std::cout << "Still: " << task3->getStateName() << std::endl;

        } else if (choice == 4) {
            task2->start();
            task2->block();
            std::cout << task2->getName() << " is now: " << task2->getStateName()
                      << " (blocked: " << task2->isBlocked() << ")" << std::endl;
            runEscalationWorkflow(featureA);
            showFullBoard(epic);

        } else if (choice == 5) {
            std::cout << "Before move -> Feature B children: " << featureB->childCount() << std::endl;
            featureB->remove(task3);
            featureA->add(task3);
            std::cout << "After move -> Feature B children: " << featureB->childCount() << std::endl;
            std::cout << "(Snapshot policy: iterators created before this point are unaffected;" << std::endl;
            std::cout << " a new iterator created now reflects the change.)" << std::endl;
            showFullBoard(epic);

        } else if (choice == 6) {
            running = false;

        } else {
            std::cout << "Not a valid option, try again." << std::endl;
        }
    }

    delete epic;
    std::cout << "\nDemo ended. Hierarchy cleaned up." << std::endl;
    return 0;
}