#ifndef CARD_H
#define CARD_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  char *suit;
  int rank;
} Card;

/**
 * @brief Creates a new Card struct.
 *
 * @param suit: The suite of the card ("Clubs", "Diamonds", "Hearts", "Spades").
 * @param rank: The rank of the card (A value between 1 and 13)
 * @return A new Card*
 */
Card *CardNew(char *suit, int rank);

/**
 * @brief Fills an empty Card *arr[] with standard 52-card deck cards.
 *
 * @param deck: An empty Card *arr[52].
 */
void fill_deck(Card *deck[]);

/**
 * @brief Selects 5 random cards from "deck" and copies them to "hand".
 *
 * @param hand: An array to copy 5 cards into.
 * @param deck: An array to choose 5 cards from. Assumes it has at least 52
 * elements.
 */
void fill_hand(Card *hand[], Card *deck[]);

/**
 * @brief Prints the rank and suite of a Card *arr[]
 *
 * @param deck: An array of Card*.
 * @param deck_length The length of deck.
 */
void PrintDeck(Card *deck[], int deck_length);

/**
 * @brief Prints if the hand contains one or ore of the following:
 * {"Pair", "Two pairs", "Three of a Kind", "Flush", "Straight", "Four of a
 * kind"}
 *
 * @param hand
 */
void AnalyzePokerHand(Card *hand[]);

/**
 * @brief Counts the amount of pairs a hand contains. A pair is defined as 2
 * Cards with matching ranks and different suits.
 *
 * @param hand
 * @return int: Amount of pairs found in hand.
 */
int CountPairsInHand(Card *hand[]);

/**
 * @brief Checks if the hand contains "Three of a Kind". "Three of a Kind" is
 * described as a hand containing 3 cards of matching suits.
 *
 * @param hand
 * @return true
 * @return false
 */
bool ContainsThreeOfAKind(Card *hand[]);

/**
 * @brief Checks if the hand contains "Four of a Kind". "Four of a Kind" is
 * described as a hand containing 4 cards of matching suits.
 *
 * @param hand
 * @return true
 * @return false
 */
bool ContainsFourOfAKind(Card *hand[]);

/**
 * @brief Checks if the hand contains a "Flush". "Flush" is described as all 5
 * cards of a hand having matching suites.
 *
 * @param hand
 * @return true
 * @return false
 */
bool ContainsFlush(Card *hand[]);

/**
 * @brief Checks if the hand contains a "Straight". "Straight" is defined in the
 * following way: A hand that, when ordered by ranks, forms a sequence
 * (ex. 2 of Hearts, 3 of Hearts, 4 of Spades, 5 of Diamonds, 6 of Clubs)
 *
 * @param hand
 * @return true
 * @return false
 */
bool ContainsStraight(Card *hand[]);

/**
 * @brief Returns 1 if c1's rank > c2's rank, -1 if c1's rank < c2's rank
 * and 0 their ranks are equal.
 *
 * @param c1
 * @param c2
 * @return int
 */
int CompareCardsByRank(const void *c1, const void *c2);

#endif /* CARD_H */