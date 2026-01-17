// platform_unix.c
#include "platform.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <time.h>

// ---------------- Save path ----------------
void GetSavePath(char *buffer, size_t size) {
    char *home = getenv("HOME"); // e.g., /home/user
    if (!home) home = ".";       // fallback to current folder
    snprintf(buffer, size, "%s/.casino_save.bin", home);
}

// ---------------- Sleep ----------------
void SleepMs(int ms) {
    sleep(ms / 1000);
}

// ---------------- Case-insensitive string compare ----------------
int StrCaseCmp_Custom(const char *a, const char *b) {
    return strcasecmp (a, b); // POSIX
}

// ---------------- Clear Console ----------------
void ClearConsole(void) {
    // Clear screen and move cursor to top-left
    printf("\x1b[2J\x1b[H");
    fflush(stdout); // ensure it updates immediately
}
