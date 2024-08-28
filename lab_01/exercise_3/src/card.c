#include "card.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

Card *CardNew(char *suit, int rank) {
  Card *c = malloc(sizeof(Card));
  c->suit = strdup(suit);
  c->rank = rank;
  return c;
}

void fill_deck(Card *deck[]) {
  char *suits[] = {"Clubs", "Diamonds", "Hearts", "Spades"};
  int deck_idx = 0;
  for (int suit_idx = 0; suit_idx < 4; suit_idx++) {
    char *current_suit = suits[suit_idx];
    for (int rank = 1; rank <= 13; rank++) {
      deck[deck_idx] = CardNew(current_suit, rank);
      deck_idx++;
    }
  }
}

void fill_hand(Card *hand[], Card *deck[]) {
  srand(time(NULL));  // Init seed to current time since UNIX 00:00:00 UTC
  for (int i = 0; i < 5; i++) {
    int random_idx = rand() % 52;
    hand[i] = deck[random_idx];
  }
}

void PrintDeck(Card *deck[], int deck_length) {
  for (int i = 0; i < deck_length; i++) {
    Card current_card = *deck[i];
    printf("%d of %s\n", current_card.rank, current_card.suit);
  }
}

void AnalyzePokerHand(Card *hand[]) {
  int pairs = CountPairsInHand(hand);
  if (pairs == 1) {
    printf("Contains pattern 'Pairs'\n");
  }
  if (pairs == 2) {
    printf("Contains pattern 'Two Pairs'\n");
  }
  if (ContainsThreeOfAKind(hand)) {
    printf("Contains pattern 'Three of a Kind'\n");
  }
  if (ContainsFourOfAKind(hand)) {
    printf("Contains pattern 'Four of a Kind'\n");
  }
  if (ContainsStraight(hand)) {
    printf("Contains pattern 'Straight'\n");
  }
  if (ContainsFlush(hand)) {
    printf("Contains pattern 'Flush'\n");
  }
}

int CountPairsInHand(Card *hand[]) {
  int pairs = 0;
  int size = 5;
  int visited[size];
  for (int i = 0; i < size; i++) {
    visited[i] = 0;
  }

  for (int i = 0; i < size; i++) {
    if (visited[i] == 1) {
      continue;
    }

    int rank_count = 1;
    for (int j = i + 1; j < size; j++) {
      if (hand[i]->rank == hand[j]->rank) {
        rank_count++;
        visited[j] = 1;
      }
    }
    if (rank_count >= 2) {
      pairs++;
    }
  }

  return pairs;
}

bool ContainsThreeOfAKind(Card *hand[]) {
  int three_of_a_kind = 0;
  int size = 5;
  int visited[size];
  for (int i = 0; i < size; i++) {
    visited[i] = 0;
  }

  for (int i = 0; i < size; i++) {
    if (visited[i] == 1) {
      continue;
    }

    int suit_count = 1;
    for (int j = i + 1; j < size; j++) {
      if (strcmp(hand[i]->suit, hand[j]->suit) == 0) {
        suit_count++;
        visited[j] = 1;
      }
    }
    if (suit_count >= 3) {
      three_of_a_kind++;
    }
  }

  return three_of_a_kind >= 1;
}

bool ContainsFourOfAKind(Card *hand[]) {
  int four_of_a_kind = 0;
  int size = 5;
  int visited[size];
  for (int i = 0; i < size; i++) {
    visited[i] = 0;
  }

  for (int i = 0; i < size; i++) {
    if (visited[i] == 1) {
      continue;
    }

    int suit_count = 1;
    for (int j = i + 1; j < size; j++) {
      if (strcmp(hand[i]->suit, hand[j]->suit) == 0) {
        suit_count++;
        visited[j] = 1;
      }
    }
    if (suit_count >= 4) {
      four_of_a_kind++;
    }
  }

  return four_of_a_kind >= 1;
}

bool ContainsFlush(Card *hand[]) {
  char *initial_suit = hand[0]->suit;
  int size = 5;
  for (int i = 1; i < size; i++) {
    if (strcmp(initial_suit, hand[i]->suit) != 0) {
      return false;
    }
  }
  return true;
}

bool ContainsStraight(Card *hand[]) {
  int size = 5;
  Card *sorted_hand[size];
  for (int i = 0; i < size; i++) {
    sorted_hand[i] = hand[i];
  }
  qsort(sorted_hand, size, sizeof(Card *), CompareCardsByRank);

  int initial_rank = hand[0]->rank;
  for (int i = 1; i < size; i++) {
    Card current_card = *hand[i];
    if (current_card.rank != initial_rank + 1) {
      return false;
    }
    initial_rank = current_card.rank;
  }
  return true;
}

int CompareCardsByRank(const void *c1, const void *c2) {
  int c1_rank = (*(Card **)c1)->rank;
  int c2_rank = (*(Card **)c2)->rank;
  return (c1_rank > c2_rank) - (c1_rank < c2_rank);
}