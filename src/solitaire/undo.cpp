#include "undo.h"

UndoStack::UndoStack() {
    index = count = 0;
}

void UndoStack::pushAction(UndoAction action) {
    actions[index++] = action;
    if (index >= UNDO_STACK_SIZE) index = 0;
    count++;
    if (count > UNDO_STACK_SIZE) count = UNDO_STACK_SIZE;
}

UndoAction UndoStack::popAction() {
    if (!isEmpty()) {
        if (index == 0) index = UNDO_STACK_SIZE - 1;
        else index--;
        count--;
        return actions[index];
    }
    // Invalid
    return UndoAction();
}

bool UndoStack::isEmpty() const {
    return count == 0;
}