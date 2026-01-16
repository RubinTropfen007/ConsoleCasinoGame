#include "card.h"

#ifndef BLACKJACK_H
#define BLACKJACK_H
int CalculateHandValue(Karte Hand[], int Laenge);

void ShuffleDeck(Karte *deck, int gesamtAnzahl);

Karte* GenerateDeck();

void BlackjackLoop(int64_t *konto);

#endif // BLACKJACK_H
