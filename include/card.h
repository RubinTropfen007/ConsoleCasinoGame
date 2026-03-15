// Literally just a header file for the Card struct and related functions.
#ifndef CASINO_CARD_H
#define CASINO_CARD_H
#include <stdint.h>

typedef struct Card {
    char name[64]; // e.g. "Ace of Spades"
    uint8_t value; // Blackjack value (2-10 for number cards, 10 for face cards, 11 for Ace)
} Card;
#endif