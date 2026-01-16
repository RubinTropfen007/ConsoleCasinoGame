#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "platform.h"

int64_t Beg(int64_t *konto)
{
    if (*konto >= 2000LL) {
        printf("Du hast bereits genug Geld!\nUnverschämt!\n");
        SleepMs(2000);
        return 0;
    }

    int64_t betrag = rand() % 200; // Bettelbetrag
    *konto += betrag;

    printf("Du hast %lld Geld erbettelt!\n", betrag);
    SleepMs(2000);
    return betrag;
}
