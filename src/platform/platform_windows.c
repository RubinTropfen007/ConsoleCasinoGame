// platform_windows.c
#include "platform.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ---------------- Save path ----------------
void GetSavePath(char *buffer, size_t size) {
    char *appData = getenv("APPDATA"); // e.g., C:\Users\<user>\AppData\Roaming
    if (!appData) appData = ".";       // fallback to current folder
    snprintf(buffer, size, "%s\\casino_save.bin", appData);
}

// ---------------- Sleep ----------------
void SleepMs(int ms) {
    Sleep(ms); // Windows Sleep expects milliseconds
}

// ---------------- Case-insensitive string compare ----------------
int StrCaseCmp_Custom(const char *a, const char *b) {
    return _stricmp(a, b); // Windows-specific
}

// ---------------- Clear Console ----------------
void ClearConsole(void) {
/*    // Enable ANSI escape codes in Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode)) {
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    // Clear screen and move cursor to top-left
  printf("\e[1;1H\e[2J");
*/
    system("cls");
}
