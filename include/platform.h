// platform.h
#pragma once
#include <stddef.h>
#include <stdint.h>

// ---------------- OS-specific file paths ----------------

// Returns the full path to the save file (e.g., AppData on Windows, $HOME on Unix)
void GetSavePath(char *buffer, size_t size);

// ---------------- Sleep / Timing ----------------

// Sleep for given milliseconds (ms)
void SleepMs(int ms);

// ---------------- String utilities ----------------

// Case-insensitive string compare (works on Windows and Unix)
int StrCaseCmp_Custom(const char *a, const char *b);

//
void ClearConsole(void);
