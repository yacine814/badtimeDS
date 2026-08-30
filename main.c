#include <nds.h>
#include "game.h"

int main(void)
{
    consoleDemoInit();

    initGame();

    while (1)
    {
        scanKeys();

        int keys = keysHeld();

        updateGame(keys);

        drawGame();

        swiWaitForVBlank();
    }

    return 0;
}