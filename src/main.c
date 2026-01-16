#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "blackjack.h"
#include "beg.h"
#include "platform.h"

typedef enum {
    UNKNOWN = -1,
    EXIT = 0,
    BETTELN,
    BLACKJACK,
    ROULETTE,
    POKER,
    SETTINGS
} Command;

// ----------------- Command Parser -----------------
Command ParseCommand(const char *input) {
    if (StrCaseCmp(input, "Betteln") == 0) return BETTELN;
    if (StrCaseCmp(input, "Blackjack") == 0) return BLACKJACK;
    if (StrCaseCmp(input, "Roulette") == 0) return ROULETTE;
    if (StrCaseCmp(input, "Poker") == 0) return POKER;
    if (StrCaseCmp(input, "Settings") == 0) return SETTINGS;
    if (StrCaseCmp(input, "Exit") == 0) return EXIT;
    return UNKNOWN;
}

// ----------------- Menu -----------------
int Menu(int64_t *konto) {
    char input[32];

    ClearConsole();

    printf("\nDein Guthaben: %lld$\n", *konto);
    printf("Was mˆchtest du tun? (Betteln, Blackjack, Roulette, Poker, Settings, Exit)\n> ");

    if (!fgets(input, sizeof(input), stdin)) return 0;
    input[strcspn(input, "\n")] = 0;  // remove newline

    Command cmd = ParseCommand(input);

    switch (cmd) {
        case BETTELN:
            ClearConsole();
            Beg(konto);
            break;
        case BLACKJACK:
            ClearConsole();
            BlackjackLoop(konto);
            break;
        case EXIT:
            printf("Bis zum n‰chsten Mal!\n");
            return 0;
        case UNKNOWN:
            printf("Unbekannter Befehl! Bitte achte auf Groﬂ/Kleinschreibung.\n");
            break;
        default:
            printf("Dieses Spiel ist noch in der Entwicklung.\n");
            break;
    }
    return 1;
}

// ----------------- Save / Load -----------------
void SaveGameBinary(int64_t konto) {
    char path[512];
    GetSavePath(path, sizeof(path));

    FILE *file = fopen(path, "wb");
    if (file) {
        fwrite(&konto, sizeof(int64_t), 1, file);
        fclose(file);
        printf("\n[System] Kontostand (%lld) bin‰r gesichert.\n", konto);
    } else {
        printf("[System] Fehler beim Speichern!\n");
    }
}

int64_t LoadGameBinary() {
    char path[512];
    GetSavePath(path, sizeof(path));

    FILE *file = fopen(path, "rb");
    int64_t loaded = 1000LL;  // default starting balance

    if (file) {
        fread(&loaded, sizeof(int64_t), 1, file);
        fclose(file);
    }
    return loaded;
}

// ----------------- Main -----------------
int main(void) {
    srand((unsigned int)time(NULL));
    int64_t kontostand = LoadGameBinary();
    int returnCode = 1;

    printf("Willkommen zum Casino!\n");

    while (returnCode != 0) {
        returnCode = Menu(&kontostand);
    }

    // Save the game
    SaveGameBinary(kontostand);

    return returnCode;
}
