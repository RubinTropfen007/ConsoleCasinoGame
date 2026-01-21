#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <errno.h>

#include "card.h"
#include "platform.h"

#define DECK_SIZE 52
#define DECK_AMOUNT 5
#define MAX_HAND 10
#define DEALER_STAND_VALUE 17
#define WIN_PAYOUT 2

// ----------------- Utility Functions -----------------
typedef enum {
    BUYCHIPS = 0,
    BETTING = 1,
    PLAYING = 2
} GenericEnumName;
// Calculate total value of a hand
int CalculateHandValue(PlayingCard hand[], int length) {
    if (!hand) return 0;

    int sum = 0;
    int aces = 0;

    for (int i = 0; i < length; i++) {
        sum += hand[i].value;
        if (strstr(hand[i].name, "Ace") != NULL) {
            aces++;
        }
    }

    while (sum > 21 && aces > 0) {
        sum -= 10; // count Ace as 1 instead of 11
        aces--;
    }

    return sum;
}

void ShuffleDeck(PlayingCard *deck, int total) {
    if (!deck || total <= 1) return;

    for (int i = total - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        PlayingCard temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

PlayingCard* GenerateDeck(void) {
    int totalCards = DECK_SIZE * DECK_AMOUNT;
    PlayingCard *deck = malloc(totalCards * sizeof(PlayingCard));
    if (!deck) return NULL;

    const char *suits[] = {"Hearts", "Diamonds", "Spades", "Clubs"};
    const char *ranks[] = {"Ace","2","3","4","5","6","7","8","9","10","Jack","Queen","King"};
    int values[] = {11,2,3,4,5,6,7,8,9,10,10,10,10};

    int k = 0;
    for (int d = 0; d < DECK_AMOUNT; d++) {
        for (int f = 0; f < 4; f++) {
            for (int r = 0; r < 13; r++) {
                snprintf(deck[k].name, sizeof(deck[k].name), "%s %s %s", ranks[r], "of", suits[f]);
                deck[k].value = values[r];
                k++;
            }
        }
    }
    return deck;
}

// Read input from User to convert to int64_t and set out to input
bool ReadInt64(const char *prompt, int64_t *out)
{
    char input[64];
    char *end;
    long long value;

    fputs(prompt, stdout);

    if (!fgets(input, sizeof input, stdin))
        return false;

    errno = 0;
    value = strtoll(input, &end, 10);

    if (end == input)
        return false;

    if (errno == ERANGE)
        return false;

    while (*end == ' ' || *end == '\t' || *end == '\n')
        end++;

    if (*end != '\0')
        return false;

    *out = (int64_t)value;
    return true;
}

// Player Turn Function returns false when player busted. Maybe call this function Recursively until busted or stands?
bool PlayerTurn(PlayingCard *Deck, PlayingCard *PlayerHand, int *PlayerCardAmount, int *Cursor, int64_t *Bet, int64_t *ChipsOnTable) {
    while (true) {
        int64_t Action;
        if (!ReadInt64("What would you like to do?\nHit(0), Stand(1), Double Down(2): ", &Action)) {
            // input failed, maybe treat as stand?
            return true;
        }

        switch (Action) {
            case 0: // Hit
                PlayerHand[*PlayerCardAmount] = Deck[(*Cursor)++];
                printf("You have drawn: %s\n", PlayerHand[*PlayerCardAmount].name);
                (*PlayerCardAmount)++;

                if (CalculateHandValue(PlayerHand, *PlayerCardAmount) > 21) {
                    printf("Busted!\n");
                    return false; // player busts
                }
                break;

            case 1: // Stand
                return true;

            case 2: // Doubling Down (Crazy)
                if((*Bet)*2 > *ChipsOnTable) {
                    printf("Not enough Chips to double down!");
                    break; // I guess we break here?
                }
                if(*PlayerCardAmount =! 2) {
                    printf("You can only double down on your first turn!");
                    break;
                }
                *ChipsOnTable -= *Bet;
                *Bet *= 2;
                PlayerHand[*PlayerCardAmount] = Deck[(*Cursor)++];
                printf("You have drawn: %s\n", PlayerHand[*PlayerCardAmount].name);
                (*PlayerCardAmount)++;
                if (CalculateHandValue(PlayerHand, *PlayerCardAmount) > 21) {
                    printf("Busted!\n");
                    return false;
                }
                return true;
            default:
                printf("Invalid input! Please enter 0 for Hit or 1 for Stand.\n");
                break;
        }
    }
}

bool DealerTurn(PlayingCard *Deck, PlayingCard *DealerHand, int *DealerCardAmount, int *Cursor) {
    printf("Dealer's turn...\n");

    printf("Dealer reveals: %s\n", DealerHand[1].name);
    printf("Dealer's total: %d\n", CalculateHandValue(DealerHand, *DealerCardAmount));

    while (true) {
        int handValue = CalculateHandValue(DealerHand, *DealerCardAmount);

        if (handValue >= DEALER_STAND_VALUE) {
            printf("Dealer stands with %d\n", handValue);
            return true;
        }

        printf("Dealer draws a card...\n");
        DealerHand[*DealerCardAmount] = Deck[(*Cursor)++];
        printf("Dealer drew: %s", DealerHand[*DealerCardAmount].name);
        (*DealerCardAmount)++;

        handValue = CalculateHandValue(DealerHand, *DealerCardAmount);
        printf("Dealer's total is now %d\n", handValue);

        if (handValue > 21) {
            printf("Dealer Busts!");
            return false;
        }
    }
}

void BlackjackLoop(int64_t *konto) {
    if (!konto) return;
    int state = BUYCHIPS;
    PlayingCard *Deck = GenerateDeck();
    int64_t BuyIn = 0;
    int64_t ChipsOnTable = 0;
    int64_t Bet = 0;

    while (true) {
        switch (state) {
            case BUYCHIPS: {

                while (true) {
                    printf("Your Balance: %lld\n", *konto);
                    if (!ReadInt64("How much would you like to buy in for?\n(0 to cancel): ", &BuyIn))
                    return; // Exit game cuz boom :(

                if (BuyIn == 0) {
                    // Return leftover chips to account if there even are any
                    *konto += ChipsOnTable;
                    return; // exit game
                }

                if (BuyIn < 0 || BuyIn > *konto) {
                    printf("Invalid amount!\n");
                    continue;
                }

                *konto -= BuyIn;
                ChipsOnTable += BuyIn; // add instead of set to account for chips from potential past rounds
                state = BETTING;
                break;
        }
        break;
    }

            case BETTING: {

                while (true) {
                    printf("Chips on table: %lld\n", ChipsOnTable);
                    if (!ReadInt64("How much would you like to bet?\n(0 to buy chips): ", &Bet))
                    return; // Something went boom

                    if (Bet == 0) {
                        state = BUYCHIPS;
                        break;
                    }

                    if (Bet < 0 || Bet > ChipsOnTable) {
                        printf("Invalid bet!\n");
                        continue; // repeat input
                    }

                    ChipsOnTable -= Bet;
                    state = PLAYING;
                    break;
                }
                break; // exit switch
            }
            case PLAYING: {
                // Stuff
                int CardsTotal = DECK_AMOUNT * DECK_SIZE;
                int Cursor = 0;
                ShuffleDeck(Deck, CardsTotal);

                // Hands
                PlayingCard PlayerHand[MAX_HAND];
                PlayingCard DealerHand[MAX_HAND];
                int PlayerCardAmount = 0;
                int DealerCardAmount = 0;

                // Initial Deal
                PlayerHand[0] = Deck[Cursor++];
                PlayerHand[1] = Deck[Cursor++];
                DealerHand[0] = Deck[Cursor++];
                DealerHand[1] = Deck[Cursor++];
                PlayerCardAmount = 2;
                DealerCardAmount = 2;
                printf("Your Cards: %s, %s | ", PlayerHand[0].name, PlayerHand[1].name);
                printf("Dealer's Cards: %s\n", DealerHand[0].name);

                bool PlayerAlive = PlayerTurn(Deck,PlayerHand,&PlayerCardAmount,&Cursor,&Bet,&ChipsOnTable);
                bool DealerAlive = DealerTurn(Deck,DealerHand,&DealerCardAmount,&Cursor);
                if (!PlayerAlive) {
                    printf("Player busted!\n");
                }else if(!DealerAlive) {
                    printf("Dealer busted! You win!\n");
                    ChipsOnTable += Bet * WIN_PAYOUT;
                }else {
                    int PlayerTotal = CalculateHandValue(PlayerHand, PlayerCardAmount);
                    int DealerTotal = CalculateHandValue(DealerHand, DealerCardAmount);

                    printf("Comparing Hands...\n");
                    if (PlayerTotal > DealerTotal) {
                        printf("You won!\n");
                        printf("Your hand: %d | Dealer's hand %d\n", PlayerTotal, DealerTotal);
                        ChipsOnTable += Bet * WIN_PAYOUT;
                    }
                    if (PlayerTotal == DealerTotal) {
                        printf("Both hands are equal!\n");
                        ChipsOnTable += Bet;
                    }
                    if (PlayerTotal < DealerTotal) {
                        printf("You lost!\n");
                    }
                }
                state = (ChipsOnTable > 0) ? BETTING : BUYCHIPS;
                break;
            }
        }
    }
    free(Deck);
}
