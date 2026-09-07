#include <iostream>
#include <vector>
#include <cassert>

#include "WorkGroup.h"
#include "Task.h"
#include "WorkItemIterator.h"
#include "PreOrderIterator.h"
#include "BlockedWorkIterator.h"
#include "NullIterator.h"
#include "HighPriorityDecorator.h"
#include "CodeReviewRequiredDecorator.h"
#include "SecurityAuditFlagDecorator.h"

namespace {

void section(const std::string& title) {
    std::cout << "\n============================================================\n";
    std::cout << title << "\n";
    std::cout << "============================================================\n";
}

void showFullBoard(WorkGroup* epic) {
    std::cout << "\n-- Full Board (Composite::display, recursive rollup) --\n";
    epic->display(0);
}

} 

// ------------------------------------------------------------------
// Exercises every TaskState transition, both the valid ones (which
// live in the five concrete state classes) and the invalid ones
// (which fall back to TaskState's default "cannot do that" behaviour).
// Runs on a standalone Task that is never added to a WorkGroup, so it
// is deleted explicitly by the caller.
// ------------------------------------------------------------------
void runStateMachineCoverage() {
    section("Task State Machine - Task A (full walk, valid + invalid)");

    Task* a = new Task("Fix login redirect bug", 3.0);
    std::cout << a->getName() << " starts in: " << a->getStateName()
              << " (blocked=" << a->isBlocked() << ")\n";

    // --- Invalid ops while still in Backlog (hits TaskState defaults) ---
    std::cout << "\n[Backlog] trying every op except start() - all should be rejected:\n";
    a->submitForReview(); // invalid: default
    a->approve();         // invalid: default
    a->reject();           // invalid: default
    a->block();            // invalid: default
    a->unblock();          // invalid: default
    a->reopen();           // invalid: default
    assert(a->getStateName() == "Backlog");

    // --- Backlog -> InProgress (valid) ---
    std::cout << "\n[Backlog] start() - valid transition:\n";
    a->start(); // BacklogState::start
    assert(a->getStateName() == "InProgress");

    std::cout << "\n[InProgress] trying invalid ops (start again, unblock, reopen, reject):\n";
    a->start();    // invalid: InProgress doesn't override start
    a->unblock();  // invalid: default, not blocked
    a->reopen();   // invalid: default
    a->reject();   // invalid: default, not in review

    // --- InProgress -> Blocked (valid) ---
    std::cout << "\n[InProgress] block() - valid transition:\n";
    a->block(); // InProgressState::block
    assert(a->getStateName() == "Blocked");
    assert(a->isBlocked() == true); // BlockedState::isBlocked override

    std::cout << "\n[Blocked] trying invalid ops (block again, start, submitForReview):\n";
    a->block();            // invalid: default
    a->start();            // invalid: default
    a->submitForReview();  // invalid: default

    // --- Blocked -> InProgress (valid) ---
    std::cout << "\n[Blocked] unblock() - valid transition:\n";
    a->unblock(); // BlockedState::unblock
    assert(a->getStateName() == "InProgress");
    assert(a->isBlocked() == false);

    // --- InProgress -> InReview (valid) ---
    std::cout << "\n[InProgress] submitForReview() - valid transition:\n";
    a->submitForReview(); // InProgressState::submitForReview
    assert(a->getStateName() == "InReview");

    std::cout << "\n[InReview] trying invalid op (block):\n";
    a->block(); // invalid: default, InReview doesn't override block

    // --- InReview -> Done (valid) ---
    std::cout << "\n[InReview] approve() - valid transition:\n";
    a->approve(); // InReviewState::approve
    assert(a->getStateName() == "Done");

    std::cout << "\n[Done] trying invalid op (start):\n";
    a->start(); // invalid: default

    // --- Done -> InProgress (valid, bug found later) ---
    std::cout << "\n[Done] reopen() - valid transition:\n";
    a->reopen(); // DoneState::reopen
    assert(a->getStateName() == "InProgress");

    std::cout << "\nTask A finished the walk in state: " << a->getStateName() << "\n";

    // WorkItem::replaceChild default (Task does not override it) - a leaf
    // has no children to replace, so this must simply report failure.
    Task* decoyOld = new Task("decoy-old", 0.0);
    Task* decoyNew = new Task("decoy-new", 0.0);
    bool replaced = a->replaceChild(decoyOld, decoyNew);
    std::cout << "Task::replaceChild on a leaf (should be false): " << replaced << "\n";
    assert(!replaced);
    delete decoyOld;
    delete decoyNew;

    // Task::createIterator always returns a NullIterator - verify all
    // three NullIterator operations directly.
    section("Leaf Iteration - NullIterator via Task::createIterator");
    WorkItemIterator* leafIt = a->createIterator();
    std::cout << "NullIterator::hasNext() on a leaf: " << leafIt->hasNext() << "\n";
    assert(!leafIt->hasNext());
    WorkItem* none = leafIt->next();
    std::cout << "NullIterator::next() on a leaf (should be nullptr): "
              << (none == nullptr ? "nullptr" : "NOT NULL") << "\n";
    assert(none == nullptr);
    leafIt->first(); // no-op, but must not crash
    std::cout << "NullIterator::first() called (no-op) - still hasNext()==" << leafIt->hasNext() << "\n";
    delete leafIt;

    delete a; // standalone Task, never attached to a WorkGroup

    // --- Task B: covers InReviewState::reject(), the one valid
    // transition not exercised by Task A's walk. ---
    section("Task State Machine - Task B (InReview -> reject)");
    Task* b = new Task("Add pagination to reports", 2.0);
    b->start();            // Backlog -> InProgress
    b->submitForReview();  // InProgress -> InReview
    std::cout << b->getName() << " is now: " << b->getStateName() << "\n";
    b->reject(); // InReviewState::reject -> InProgress
    std::cout << "After reject(): " << b->getStateName() << "\n";
    assert(b->getStateName() == "InProgress");
    delete b;
}

int main() {
    std::cout << "===================================\n";
    std::cout << " TaskForge - Automated Test Driver\n";
    std::cout << "===================================\n";

    // ----------------------------------------------------------------
    // Build a hierarchy with three levels of nesting below the root,
    // mixing individual Tasks and nested WorkGroups at the same level
    // (Composite pattern):
    //
    // epic (root)                                    <- root
    //  |- featureA "Payment Flow"                     <- level 1
    //  |   |- storyA1 "Gateway Integration"            <- level 2
    //  |   |   |- task1 "Integrate payment gateway"     <- level 3
    //  |   |   `- task2 "Add retry logic"                <- level 3
    //  |   `- task4 "Update payment docs"               <- level 2 (leaf sibling of storyA1)
    //  `- featureB "Cart UI"                           <- level 1
    //      `- task3 "Redesign cart page"                 <- level 2
    // ----------------------------------------------------------------
    section("Building the Composite hierarchy");

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

    std::cout << "featureA child count: " << featureA->childCount() << "\n";
    std::cout << "featureB child count: " << featureB->childCount() << "\n";
    std::cout << "epic isEscalated(): " << epic->isEscalated() << "\n";
    std::cout << "featureA isEscalated(): " << featureA->isEscalated() << "\n";

    showFullBoard(epic);

    // ----------------------------------------------------------------
    // Full state-machine coverage on standalone tasks (kept separate
    // from the tree so the lifecycle walk can't interact with the
    // decorator/composite demos below).
    // ----------------------------------------------------------------
    runStateMachineCoverage();

    // ----------------------------------------------------------------
    // Iterator coverage: PreOrderIterator (complete traversal) and
    // BlockedWorkIterator (selection-rule traversal), including two
    // independent iterators alive over the same structure at once.
    // ----------------------------------------------------------------
    section("Iterator - PreOrderIterator (complete traversal)");
    {
        WorkItemIterator* full = epic->createIterator(IteratorType::PreOrder);
        std::cout << "Visiting every item under epic:\n";
        while (full->hasNext()) {
            WorkItem* item = full->next();
            std::cout << "  [Full] " << item->getName() << "  state=" << item->getStateName() << "\n";
        }
        std::cout << "first() rewinds the same iterator:\n";
        full->first();
        int recount = 0;
        while (full->hasNext()) { full->next(); ++recount; }
        std::cout << "Second pass visited " << recount << " items.\n";
        delete full;
    }

    // Block task2 (nested two levels below featureA) so the Blocked
    // traversal below has something meaningful to find.
    task2->start();
    task2->block();
    std::cout << "\n" << task2->getName() << " is now " << task2->getStateName()
              << " (blocked=" << task2->isBlocked() << ")\n";

    section("Iterator - BlockedWorkIterator (selection-rule traversal)");
    {
        WorkItemIterator* blocked = featureA->createIterator(IteratorType::Blocked);
        std::cout << "Blocked items under featureA:\n";
        if (!blocked->hasNext()) {
            std::cout << "  (none)\n";
        }
        while (blocked->hasNext()) {
            std::cout << "  [Blocked] " << blocked->next()->getName() << "\n";
        }
        delete blocked;
    }

    section("Two independent iterators over the same structure");
    {
        WorkItemIterator* full = epic->createIterator(IteratorType::PreOrder);
        WorkItemIterator* blockedOnly = epic->createIterator(IteratorType::Blocked);

        std::cout << "Advance the FULL traversal by 2 steps:\n";
        for (int i = 0; i < 2 && full->hasNext(); ++i) {
            std::cout << "  [Full] " << full->next()->getName() << "\n";
        }

        std::cout << "The BLOCKED traversal is untouched by that (different iterator instance):\n";
        while (blockedOnly->hasNext()) {
            std::cout << "  [Blocked] " << blockedOnly->next()->getName() << "\n";
        }

        std::cout << "Resume the FULL traversal exactly where it left off:\n";
        while (full->hasNext()) {
            std::cout << "  [Full] " << full->next()->getName() << "\n";
        }

        delete full;
        delete blockedOnly;
    }

    // ----------------------------------------------------------------
    // Runtime structural change (Composite mutation): move task4 from
    // featureA to featureB using WorkGroup::remove / WorkGroup::add.
    // ----------------------------------------------------------------
    section("Runtime structural change - moving a task between groups");
    std::cout << "Before move: featureA=" << featureA->childCount()
              << " featureB=" << featureB->childCount() << "\n";
    bool removed = featureA->remove(task4);
    std::cout << "featureA->remove(task4): " << removed << "\n";
    assert(removed);
    featureB->add(task4);
    std::cout << "After move: featureA=" << featureA->childCount()
              << " featureB=" << featureB->childCount() << "\n";
    // Removing something that is no longer present must fail cleanly.
    bool removedAgain = featureA->remove(task4);
    std::cout << "featureA->remove(task4) again (should be false): " << removedAgain << "\n";
    assert(!removedAgain);

    // ----------------------------------------------------------------
    // Decorator coverage: three concrete decorators, applied at
    // runtime, swapped into the tree in place via WorkGroup::replaceChild
    // (both a direct-child replace and a recursive/nested replace),
    // and one case of two decorators stacked on the same item.
    // ----------------------------------------------------------------
    section("Decorator - HighPriorityDecorator on a direct child");
    std::cout << "Before: \"" << task4->getName() << "\"\n";
    WorkItem* decoratedTask4 = new HighPriorityDecorator(task4);
    std::cout << "After wrapping, name becomes: \"" << decoratedTask4->getName() << "\"\n";
    std::cout << "Estimated hours unchanged (forwarded): " << decoratedTask4->getEstimatedHours() << "h\n";
    std::cout << "isBlocked() forwarded: " << decoratedTask4->isBlocked() << "\n";
    std::cout << "getStateName() forwarded: " << decoratedTask4->getStateName() << "\n";
    bool swapped4 = featureB->replaceChild(task4, decoratedTask4);
    std::cout << "featureB->replaceChild(task4, decorated) direct-child swap: " << swapped4 << "\n";
    assert(swapped4);
    decoratedTask4->display(1);

    section("Decorator - CodeReviewRequiredDecorator on a direct child");
    std::cout << "Before: \"" << task3->getName() << "\" estimated " << task3->getEstimatedHours() << "h\n";
    WorkItem* decoratedTask3 = new CodeReviewRequiredDecorator(task3);
    std::cout << "After wrapping, estimate becomes: " << decoratedTask3->getEstimatedHours() << "h\n";
    bool swapped3 = featureB->replaceChild(task3, decoratedTask3);
    std::cout << "featureB->replaceChild(task3, decorated) direct-child swap: " << swapped3 << "\n";
    assert(swapped3);
    decoratedTask3->display(1);

    section("Decorator - SecurityAuditFlagDecorator on a deeply nested child");
    std::cout << "task2 lives at epic -> featureA -> storyA1 -> task2 (depth 3).\n";
    std::cout << "Before: isEscalated()=" << task2->isEscalated()
              << " estimate=" << task2->getEstimatedHours() << "h\n";
    WorkItem* decoratedTask2 = new SecurityAuditFlagDecorator(task2);
    std::cout << "After wrapping: isEscalated()=" << decoratedTask2->isEscalated()
              << " estimate=" << decoratedTask2->getEstimatedHours() << "h\n";
    // Calling replaceChild on epic (the root) forces WorkGroup's
    // recursive search branch, since task2 is not a direct child of epic.
    bool swapped2 = epic->replaceChild(task2, decoratedTask2);
    std::cout << "epic->replaceChild(task2, decorated) recursive/nested swap: " << swapped2 << "\n";
    assert(swapped2);
    decoratedTask2->display(1);

    section("Decorator - stacking two decorators on the same item");
    std::cout << "Before: \"" << task1->getName() << "\" estimated " << task1->getEstimatedHours() << "h\n";
    // CodeReviewRequiredDecorator wraps HighPriorityDecorator wraps task1.
    WorkItem* stacked = new CodeReviewRequiredDecorator(new HighPriorityDecorator(task1));
    bool swapped1 = epic->replaceChild(task1, stacked); // also exercises the recursive branch
    std::cout << "epic->replaceChild(task1, stacked) recursive swap: " << swapped1 << "\n";
    assert(swapped1);
    std::cout << "Stacked name (HighPriority forwards through CodeReview): \"" << stacked->getName() << "\"\n";
    std::cout << "Stacked estimate (both overheads applied): " << stacked->getEstimatedHours() << "h\n";
    stacked->display(1);

    section("Decorator - createIterator() forwarded through a decorated WorkGroup");
    // Wrap the whole storyA1 group (not just a leaf) to show the
    // decorator's forwarding createIterator() reaching a *real* iterator
    // (not a NullIterator), and swap it in via the recursive branch too.
    WorkItem* decoratedStory = new HighPriorityDecorator(storyA1);
    bool swappedStory = featureA->replaceChild(storyA1, decoratedStory);
    std::cout << "featureA->replaceChild(storyA1, decorated) group swap: " << swappedStory << "\n";
    assert(swappedStory);
    WorkItemIterator* viaDecorator = decoratedStory->createIterator(IteratorType::PreOrder);
    std::cout << "Traversing through the decorated group's forwarded iterator:\n";
    while (viaDecorator->hasNext()) {
        std::cout << "  [Via decorator] " << viaDecorator->next()->getName() << "\n";
    }
    delete viaDecorator;

    section("Decorator - replaceChild failure path (item not present)");
    Task* orphan = new Task("Not in the tree", 0.0);
    WorkItem* orphanDecorated = new HighPriorityDecorator(orphan);
    bool failedSwap = featureB->replaceChild(orphan, orphanDecorated);
    std::cout << "featureB->replaceChild(orphan, decorated) (should be false): " << failedSwap << "\n";
    assert(!failedSwap);
    delete orphanDecorated; // never entered the tree, so we own and free it ourselves

    section("Final board after all structural, state and decorator changes");
    showFullBoard(epic);

    std::cout << "\nAll functions exercised successfully.\n";

    delete epic; // recursively deletes every WorkGroup/Task/Decorator still owned by the tree
    std::cout << "Demo ended. Hierarchy cleaned up.\n";
    return 0;
}