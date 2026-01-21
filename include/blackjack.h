#include "card.h"

#ifndef BLACKJACK_H
#define BLACKJACK_H
int CalculateHandValue(PlayingCard Hand[], int Laenge);

void ShuffleDeck(PlayingCard *deck, int gesamtAnzahl);

PlayingCard* GenerateDeck();

void BlackjackLoop(int64_t *konto);

#endif // BLACKJACK_H
