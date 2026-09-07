#include <iostream>
#include <vector>
#include "WorkGroup.h"
#include "Task.h"
#include "PreOrderIterator.h"
#include "BlockedWorkIterator.h"
#include "SecurityAuditFlagDecorator.h"
#include "HighPriorityDecorator.h"
#include "CodeReviewRequiredDecorator.h"

void showFullBoard(WorkGroup* epic) {
    std::cout << "\n-- Full Board (PreOrderIterator - complete traversal) --" << std::endl;
    epic->display(0);
}

// Runs an escalation sweep over everything inside `feature` (which may be
// nested several levels deep - Features contain Stories, Stories contain
// Tasks). Any blocked item gets wrapped in a SecurityAuditFlagDecorator.
//
// Fix: rather than assuming the blocked item is a direct child of `feature`
// (which breaks the moment the hierarchy gets deeper than one level),
// this locates whichever group actually owns the item - at any depth -
// via WorkGroup::replaceChild() and swaps the raw pointer for the
// decorated one in place. Ownership moves cleanly to the decorator; the
// original item is never deleted or re-inserted at the wrong spot.
void runEscalationWorkflow(WorkGroup* feature) {
    std::cout << "\n-- Escalation Sweep on '" << feature->getName() << "' --" << std::endl;

    std::vector<WorkItem*> snapshot;
    WorkItemIterator* it = feature->createIterator(IteratorType::PreOrder);
    while (it->hasNext()) {
        snapshot.push_back(it->next());
    }
    delete it;

    for (WorkItem* item : snapshot) {
        if (item->isBlocked()) {
            std::cout << " -> " << item->getName() << " is BLOCKED. Escalating..." << std::endl;

            WorkItem* decorated = new SecurityAuditFlagDecorator(item);
            std::cout << "    [LOG] Escalation logged for '" << item->getName() << "'" << std::endl;

            // Find item wherever it actually lives inside `feature`'s
            // subtree (direct child or nested inside a Story) and swap
            // it for the decorated version in place.
            bool replaced = feature->replaceChild(item, decorated);
            if (!replaced) {
                std::cout << "    [WARN] Could not locate '" << item->getName()
                          << "' inside '" << feature->getName() << "' to replace it." << std::endl;
                delete decorated; // avoid a leak if this ever happens
            }
        }
    }
}

// Demonstrates that PreOrderIterator (visits everything) and
// BlockedWorkIterator (visits only blocked items) are two independent,
// meaningful traversals over the *same* runtime structure: advancing one
// does not affect the other, and both can be alive at once.
void demoIndependentTraversals(WorkGroup* epic) {
    std::cout << "\n-- Two independent iterators over the same structure --" << std::endl;

    WorkItemIterator* full = epic->createIterator(IteratorType::PreOrder);
    WorkItemIterator* blockedOnly = epic->createIterator(IteratorType::Blocked);

    std::cout << "Advancing the FULL traversal by up to 2 steps:" << std::endl;
    for (int i = 0; i < 2 && full->hasNext(); ++i) {
        std::cout << "  [Full] " << full->next()->getName() << std::endl;
    }

    std::cout << "The BLOCKED-only traversal is completely unaffected by that "
                 "(it wasn't touched, and it uses a different selection rule):" << std::endl;
    if (!blockedOnly->hasNext()) {
        std::cout << "  (no blocked items right now)" << std::endl;
    }
    while (blockedOnly->hasNext()) {
        std::cout << "  [Blocked] " << blockedOnly->next()->getName() << std::endl;
    }

    std::cout << "Continuing the FULL traversal from exactly where it left off:" << std::endl;
    while (full->hasNext()) {
        std::cout << "  [Full] " << full->next()->getName() << std::endl;
    }

    delete full;
    delete blockedOnly;
}

// Demonstrates stacking two decorators on the same item at once, and
// shows the decorated object participating in normal system behaviour
// (its name, hours and display output all change) while still being
// swapped correctly into whatever group actually owns it.
void runPriorityReviewEscalation(WorkGroup* owningGroup, WorkItem* target) {
    std::cout << "\n-- Stacking decorators on '" << target->getName() << "' --" << std::endl;
    std::cout << "Before: \"" << target->getName() << "\" estimated at "
              << target->getEstimatedHours() << "h" << std::endl;

    // CodeReviewRequiredDecorator wraps HighPriorityDecorator wraps target.
    WorkItem* decorated = new CodeReviewRequiredDecorator(new HighPriorityDecorator(target));

    bool replaced = owningGroup->replaceChild(target, decorated);
    if (!replaced) {
        std::cout << "    [WARN] Could not locate '" << target->getName()
                  << "' inside '" << owningGroup->getName() << "'." << std::endl;
        delete decorated;
        return;
    }

    std::cout << "After stacking [CodeReviewRequired(HighPriority(...))]:" << std::endl;
    decorated->display(1);
    std::cout << "New name via decorator chain: \"" << decorated->getName() << "\"" << std::endl;
    std::cout << "New estimate: " << decorated->getEstimatedHours() << "h" << std::endl;
}

int main() {
    std::cout << "===================================" << std::endl;
    std::cout << " TaskForge - Software Delivery Demo" << std::endl;
    std::cout << "===================================" << std::endl;

    // Hierarchy (3 levels of nesting below the root/client boundary):
    //
    // epic (root, WorkGroup)
    //  |- featureA (WorkGroup)               <- level 1
    //  |   |- storyA1 (WorkGroup)            <- level 2
    //  |   |   |- task1 (Task)               <- level 3
    //  |   |   `- task2 (Task)               <- level 3
    //  |   `- task4 (Task, direct child)     <- level 2, sibling of storyA1
    //  `- featureB (WorkGroup)               <- level 1
    //      `- task3 (Task)                   <- level 2
    //
    // featureA holds both a nested group (storyA1) and a direct task
    // (task4) as siblings, which is exactly where treating leaves and
    // groups uniformly (Composite) actually matters.
    WorkGroup* epic = new WorkGroup("Epic: Checkout Revamp");
    WorkGroup* featureA = new WorkGroup("Feature: Payment Flow");
    WorkGroup* featureB = new WorkGroup("Feature: Cart UI");
    WorkGroup* storyA1 = new WorkGroup("Story: Gateway Integration");

    Task* task1 = new Task("Integrate payment gateway", 8.0);
    Task* task2 = new Task("Add retry logic", 4.0);
    Task* task3 = new Task("Redesign cart page", 5.0);
    Task* task4 = new Task("Update payment docs", 1.0);

    storyA1->add(task1);
    storyA1->add(task2);

    featureA->add(storyA1);
    featureA->add(task4);

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
        std::cout << "6. Show two independent iterators over the same structure" << std::endl;
        std::cout << "7. Stack decorators (HighPriority + CodeReview) on a task" << std::endl;
        std::cout << "8. Exit demo" << std::endl;
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
            // task2 lives two levels below featureA (featureA -> storyA1 -> task2),
            // which is exactly the case that used to break the escalation sweep.
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
            demoIndependentTraversals(epic);

        } else if (choice == 7) {
            // Stack HighPriorityDecorator and CodeReviewRequiredDecorator on
            // task4 (a direct child of featureA) and show the combined effect.
            runPriorityReviewEscalation(featureA, task4);
            showFullBoard(epic);

        } else if (choice == 8) {
            running = false;

        } else {
            std::cout << "Not a valid option, try again." << std::endl;
        }
    }

    delete epic;
    std::cout << "\nDemo ended. Hierarchy cleaned up." << std::endl;
    return 0;
}
