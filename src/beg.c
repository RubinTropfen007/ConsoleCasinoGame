#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "platform.h"

#define BEGGING_ACCOUNT_LIMIT 2000
#define BEGGING_MAX_PAYOUT 200

int64_t Beg(int64_t *konto)
{
    int64_t amount;

    if (*konto >= BEGGING_ACCOUNT_LIMIT) {
        printf("You Already have enough Cash!\nThe Audacity...\n");

    }else{
        // Payout from begging
        amount = rand() % BEGGING_MAX_PAYOUT;
        *konto += amount;
        printf("You have gained %lld $ from begging!\n", amount);

    }
    SleepMs(1000);
    return amount;
}
