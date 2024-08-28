#include <stdio.h>

#include "card.h"

int main() {
  Card *deck[52];
  Card *hand[5];
  fill_deck(deck);
  fill_hand(hand, deck);
  PrintDeck(hand, 5);
  PrintDeck(deck, 52);
  AnalyzePokerHand(hand);
}