#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "card.h"
#include "platform.h"

#define DECK_SIZE 52
#define DECK_AMOUNT 5
#define MAX_HAND 10

// ----------------- Utility Functions -----------------

// Calculate total value of a hand
int CalculateHandValue(Karte hand[], int length) {
    if (!hand) return 0;

    int sum = 0;
    int aces = 0;

    for (int i = 0; i < length; i++) {
        sum += hand[i].wert;
        if (strstr(hand[i].name, "Ass") != NULL) {
            aces++;
        }
    }

    while (sum > 21 && aces > 0) {
        sum -= 10; // count Ace as 1 instead of 11
        aces--;
    }

    return sum;
}

// Shuffle a deck in-place
void ShuffleDeck(Karte *deck, int total) {
    if (!deck || total <= 1) return;

    for (int i = total - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Karte temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

// Generate a deck (multiple decks combined)
Karte* GenerateDeck(void) {
    int totalCards = DECK_SIZE * DECK_AMOUNT;
    Karte *deck = malloc(totalCards * sizeof(Karte));
    if (!deck) return NULL;

    const char *suits[] = {"Herz", "Karo", "Pik", "Kreuz"};
    const char *ranks[] = {"Ass","2","3","4","5","6","7","8","9","10","Bube","Dame","König"};
    int values[] = {11,2,3,4,5,6,7,8,9,10,10,10,10};

    int k = 0;
    for (int d = 0; d < DECK_AMOUNT; d++) {
        for (int f = 0; f < 4; f++) {
            for (int r = 0; r < 13; r++) {
                snprintf(deck[k].name, sizeof(deck[k].name), "%s %s", suits[f], ranks[r]);
                deck[k].wert = values[r];
                k++;
            }
        }
    }

    ShuffleDeck(deck, totalCards);
    return deck;
}

// ----------------- Blackjack Table Loop -----------------

void BlackjackLoop(int64_t *konto) {
    if (!konto) return;

    bool playingAtTable = true;
    Karte *mainDeck = GenerateDeck();
    if (!mainDeck) {
        printf("Fehler: Deck konnte nicht erstellt werden.\n");
        return;
    }
    int cursor = 0;

    while (playingAtTable) {
        // ---------------- Buy-in ----------------
        char line[32];
        int64_t BuyIn = 0;
        printf("\nWie viele Chips möchtest du kaufen? (0 zum Abbrechen): ");
        if (!fgets(line, sizeof(line), stdin)) break;
        BuyIn = atoll(line);

        if (BuyIn <= 0) break;
        if (BuyIn > *konto) {
            printf("Komm wieder, wenn du flüssig bist!\n");
            SleepMs(2000);
            continue;
        }
        ClearConsole();
        *konto -= BuyIn;
        int64_t ChipsOnTable = BuyIn;

        // ---------------- Betting Loop ----------------
        while (ChipsOnTable > 0) {
            int64_t Bet = 0;
            printf("\nChips am Tisch: %lld | Kontostand: %lld\n", ChipsOnTable, *konto);
            printf("Einsatz: ");
            if (!fgets(line, sizeof(line), stdin)) break;
            Bet = atoll(line);

            if (Bet > ChipsOnTable || Bet <= 0) {
                printf("Ungültiger Einsatz!\n");
                continue;
            }

            ChipsOnTable -= Bet;

            // ---------------- Hands ----------------
            Karte DealerHand[MAX_HAND] = {0};
            Karte SpielerHand[MAX_HAND] = {0};
            int DeKartenAnzahl = 0;
            int SpKartenAnzahl = 0;

            bool PlayerTurn = true;
            bool DealerTurn = false;

            // Initial Deal
            SpielerHand[0] = mainDeck[cursor++];
            DealerHand[0]  = mainDeck[cursor++];
            SpielerHand[1] = mainDeck[cursor++];
            DealerHand[1]  = mainDeck[cursor++];
            SpKartenAnzahl = 2;
            DeKartenAnzahl = 2;

            int SpielerHandWert = CalculateHandValue(SpielerHand, SpKartenAnzahl);
            int DealerHandWert = CalculateHandValue(DealerHand, DeKartenAnzahl);

            if (DealerHandWert == 21) {
                printf("Der Dealer hat Blackjack!\n");
                SleepMs(4000);
                continue;
            }
            ClearConsole();
            printf("Deine Karten: %s, %s\n", SpielerHand[0].name, SpielerHand[1].name);
            printf("Dealer Hand: %d | Deine Hand: %d\n", DealerHand[0].wert, SpielerHandWert);

            // ---------------- Player Turn ----------------
            while (PlayerTurn) {
                char action[16];
                printf("\nWas möchtest du tun? (Hit, Stand): ");
                if (!fgets(action, sizeof(action), stdin)) break;
                action[strcspn(action, "\n")] = 0;

                if (StrCaseCmp(action, "Hit") == 0 || StrCaseCmp(action, "H") == 0) {
                    ClearConsole();
                    SpielerHand[SpKartenAnzahl] = mainDeck[cursor++];
                    printf("Du hast gezogen: %s\n", SpielerHand[SpKartenAnzahl].name);
                    SpKartenAnzahl++;
                    SpielerHandWert = CalculateHandValue(SpielerHand, SpKartenAnzahl);
                    printf("Deine Hand: %d\n", SpielerHandWert);

                    if (SpielerHandWert > 21) {
                        printf("Bust! Total: %d\n", SpielerHandWert);
                        PlayerTurn = false;
                    }
                }

                if (StrCaseCmp(action, "Stand") == 0 || StrCaseCmp(action, "S") == 0) {
                    ClearConsole();
                    PlayerTurn = false;
                    DealerTurn = true;
                }


            }

            // ---------------- Dealer Turn ----------------
            if (DealerTurn) {
                printf("Der Dealer deckt auf: %s\n", DealerHand[1].name);
                SleepMs(2000);
                printf("Dealer Hand: %d\n", DealerHandWert);
                SleepMs(1000);

                while (DealerTurn) {
                    printf("Dealer ist dran...\n");
                    SleepMs(2000);

                    if (DealerHandWert >= 17) {
                        printf("Dealer hat >=17\n");
                        DealerTurn = false;
                    }

                    while (DealerHandWert < 17) {
                        printf("Dealer zieht eine Karte.\n");
                        DealerHand[DeKartenAnzahl] = mainDeck[cursor++];
                        SleepMs(2000);
                        printf("Dealer zieht: %s | Wert: %d\n",
                               DealerHand[DeKartenAnzahl].name, DealerHand[DeKartenAnzahl].wert);
                        DeKartenAnzahl++;
                        DealerHandWert = CalculateHandValue(DealerHand, DeKartenAnzahl);

                        if (DealerHandWert > 21) {
                            printf("Dealer Bust! Wert: %d\n", DealerHandWert);
                            ChipsOnTable += Bet*2;
                            DealerTurn = false;
                            break;
                        }

                        if (DealerHandWert >= 17) {
                            printf("Dealer >=17: %d\n", DealerHandWert);
                            DealerTurn = false;
                            break;
                        }
                    }
                }

                // ---------------- Result ----------------
                if (DealerHandWert <= 21) {
                    printf("-------- Ergebnis --------\n");
                    printf("Deine Hand: %d\nDealer Hand: %d\n", SpielerHandWert, DealerHandWert);
                    if (DealerHandWert > SpielerHandWert) {
                        printf("Verloren! Dealer gewinnt.\n");
                    } else if (DealerHandWert == SpielerHandWert) {
                        printf("Push! Einsatz zurück.\n");
                        ChipsOnTable += Bet;
                    } else {
                        printf("Gewonnen! Gewinn: %lld\n", Bet*2);
                        ChipsOnTable += Bet*2;
                    }
                }
            }

            // ---------------- Shuffle if needed ----------------
            if (cursor > (DECK_SIZE * DECK_AMOUNT * 0.8)) {
                ShuffleDeck(mainDeck, DECK_SIZE * DECK_AMOUNT);
                cursor = 0;
            }
        }

        // ---------------- Ask for new buy-in ----------------
        char choice[8];
        printf("\nKeine Chips mehr. Neue kaufen? (y/n): ");
        if (!fgets(choice, sizeof(choice), stdin)) break;
        choice[strcspn(choice, "\n")] = 0;

        if (StrCaseCmp(choice, "n") == 0) {
            playingAtTable = false;
        }
    }

    free(mainDeck);
}
