// platform_windows.c
#include "platform.h"

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void InitTerminal(void) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

// ---------------- Save path ----------------
void GetSavePath(char *buffer, size_t size) {
    char *appData = getenv("APPDATA");
    if (!appData) appData = ".";
    snprintf(buffer, size, "%s\\casino_save.bin", appData);
}

// ---------------- Sleep ----------------
void SleepMs(int ms) {
    Sleep(ms);
}

// ---------------- Case-insensitive string compare ----------------
int StrCaseCmp_Custom(const char *a, const char *b) {
    return _stricmp(a, b);
}

// ---------------- Clear Console ----------------
void ClearConsole(void) {
    system("cls"); // This because Escape Sequences don't actually clear the Console.
}
