#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stddef.h>

// A simple "ABI" between the launcher (main) and individual games.
// Each game exposes a `Game` struct instance and the launcher calls the
// `play` callback with a mutable balance.

typedef struct Game {
    const char *name;
    const char *description;

    // Optional lifecycle hooks.
    void (*init)(void);
    void (*play)(uint64_t *balance);
    void (*cleanup)(void);
} Game;

// Retrieve the list of built-in games.
// The returned array is owned by the library; callers should not free it.
const Game *game_get_all();

#endif // GAME_H