#include "NullIterator.h"

bool NullIterator::hasNext()const {
    return false;
}

WorkItem* NullIterator::next(){
    return nullptr;
}

void NullIterator::first() {
}