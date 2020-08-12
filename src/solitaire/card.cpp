#include "card.h"

Card::Card(Value value, Suit suit, bool faceDown) {
  // lowest 4 bits, value
  // bits 5 and 6, suite
  // highest bit, 1 if face down
  _value = value | (suit << 4) | (faceDown ? 0x80 : 0);
}

bool Card::isFaceDown() const {
  return 0x80 & _value;
}

Value Card::getValue() const {
  return static_cast<Value>(0x0f & _value);
}

Suit Card::getSuit() const {
  return static_cast<Suit>((_value >> 4) & 0x03);
}

bool Card::isRed() const {
  return getSuit() == heart || getSuit() == diamond;
}

void Card::flip() {
  if (isFaceDown()) _value = _value & 0x7f;
  else _value = _value | 0x80;
}

