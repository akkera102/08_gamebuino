#include "lib_Gamebuino.h"
#include "pile.h"

#define UNDO_STACK_SIZE 10

struct UndoAction {
    byte special = 0;
    Pile *source;
    Pile *destination;
    void setCardCount(byte cardCount) { special = (special & 0xf0) | (cardCount & 0x0f); }
    byte getCardCount() { return special & 0x0f; }
    void setRevealed() { special |= 0x80; }
    bool wasRevealed() { return special & 0x80; }
    void setDraw() { special |= 0x40; }
    bool wasDraw() { return special & 0x40; }
    void setFlippedTalon() { special |= 0x20; }
    bool wasFlippedTalon() { return special & 0x20; }
};

class UndoStack {
    public:
        UndoStack();
        void pushAction(UndoAction action);
        UndoAction popAction();
        bool isEmpty() const;

    private:
        UndoAction actions[UNDO_STACK_SIZE];
        byte index;
        byte count;
};
