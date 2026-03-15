#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#include "game.h"

#define MODULEDIR "../games"

const char *getfileextension(const char *filename) {
    const char *dot = strchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot;
}

static int prompt_number(const char *prompt, int min, int max) {
    char buf[32];

    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) {
            return min;
        }

        int value = (int)strtol(buf, NULL, 10);
        if (value < min || value > max) {
            printf("Please enter a number between %d and %d.\n", min, max);
            continue;
        }

        return value;
    }
}

// Die Signatur muss const Game *all_games[] sein
void LoadModules(const Game *all_games[], int *game_total_count) {
    DIR *dirp = opendir(MODULEDIR);
    struct dirent *dp;

    if (!dirp) {
        perror("Failed to open Module directory");
        exit(EXIT_FAILURE);
    }

    while ((dp = readdir(dirp)) != NULL) {
        if (strcmp(getfileextension(dp->d_name), ".so") == 0) {
            char fullpath[512];
            snprintf(fullpath, sizeof(fullpath), "%s/%s", MODULEDIR, dp->d_name);

            void *handle = dlopen(fullpath, RTLD_NOW);
            if (!handle) {
                fprintf(stderr, "dlopen failed: %s\n", dlerror());
                continue;
            }

            typedef const Game* (*GameGetFn)();
            GameGetFn get_all = (GameGetFn) dlsym(handle, "game_get_all");

            if (get_all) {
                const Game *module_games = get_all();
                
                if (module_games && *game_total_count < 128) {
                    // Jetzt passt der Typ: Zeiger auf Zeiger
                    all_games[*game_total_count] = module_games;
                    
                    printf("Loaded: %s (from %s)\n", all_games[*game_total_count]->name, dp->d_name);
                    
                    // Hier prüfen wir den Wert, auf den der Zeiger zeigt
                    if (*game_total_count > 0) {
                        printf("Previous game was: %s\n", all_games[*game_total_count - 1]->name);
                    }
                    
                    // WICHTIG: Den Wert hinter dem Zeiger erhöhen
                    (*game_total_count)++;
                }
            }
        }
    }
    closedir(dirp);
    // Hier auch dereferenzieren für das printf
    printf("Total of %d games were loaded.\n", *game_total_count);
}

int main(void) {
    const Game *all_games[128]; 
    int game_total_count = 0;

    LoadModules(all_games, &game_total_count);
    srand((unsigned int)time(NULL));

    uint64_t balance = 1000;
    size_t game_count = 0;
    const Game *games/* = game_get_all(&game_count)*/;

    printf("Goodbye!\n");
    return 0;
}