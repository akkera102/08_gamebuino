#include "lib_Gamebuino.h"
#include "card.h"

#ifndef PILE_H
#define PILE_H
class Pile {
  public:
    Pile(byte maxCards);
    ~Pile();
    void addCard(Card card);
    void addPile(Pile *pile);
    byte getCardCount() const;
    Card getCard(int indexFromTop) const;
    Card removeTopCard();
    void removeCards(int count, Pile* destination);
    void empty();
    void shuffle();
    void newDeck();
    byte getMaxCards() const;
    byte x, y;
    bool isTableau;
    
  private:
    Card *_cards;
    byte _maxCards;
    byte _count;
};
#endif
