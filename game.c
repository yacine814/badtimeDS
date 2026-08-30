#include <nds.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"

#define MAX_BONES 16

typedef struct
{
    int x;
    int y;

    int active;

    int speed;

} Bone;

Bone bones[MAX_BONES];

// ❤️ joueur

int hp = 92;

float player_x = 120;
float player_y = 110;

// 📦 vraie box Undertale

int box_x = 40;
int box_y = 30;

int box_w = 170;
int box_h = 120;

// 🎮 vitesse joueur

float player_speed = 2.2f;

// 🦴 timer os

int spawn_timer = 0;

// ==========================
// 🎨 DESSIN
// ==========================

void drawPixel(int x, int y, u16 color)
{
    if (x < 0 || x >= 256 || y < 0 || y >= 192)
        return;

    VRAM_A[y * 256 + x] = color;
}

void drawRect(int x, int y, int w, int h, u16 color)
{
    for (int iy = 0; iy < h; iy++)
    {
        for (int ix = 0; ix < w; ix++)
        {
            drawPixel(x + ix, y + iy, color);
        }
    }
}

// ❤️ coeur pixel art

void drawHeart(int x, int y)
{
    u16 red = RGB15(31,0,0);

    drawRect(x + 2, y, 4, 4, red);

    drawRect(x, y + 2, 8, 4, red);

    drawRect(x + 1, y + 6, 6, 3, red);

    drawRect(x + 2, y + 9, 4, 2, red);
}

// 🦴 os

void drawBone(int x, int y)
{
    u16 white = RGB15(31,31,31);

    drawRect(x + 2, y, 8, 24, white);

    drawRect(x, y, 12, 4, white);

    drawRect(x, y + 20, 12, 4, white);
}

// 📦 vraie box

void drawBox()
{
    u16 white = RGB15(31,31,31);

    // haut

    drawRect(box_x, box_y, box_w, 3, white);

    // bas

    drawRect(box_x, box_y + box_h, box_w, 3, white);

    // gauche

    drawRect(box_x, box_y, 3, box_h, white);

    // droite

    drawRect(box_x + box_w, box_y, 3, box_h, white);
}

// ==========================
// 🦴 OS
// ==========================

void spawnBone()
{
    for (int i = 0; i < MAX_BONES; i++)
    {
        if (!bones[i].active)
        {
            bones[i].active = 1;

            bones[i].x = box_x + 10 + rand() % (box_w - 20);

            bones[i].y = box_y;

            bones[i].speed = 2 + rand() % 2;

            break;
        }
    }
}

// ==========================
// 🚀 INIT
// ==========================

void initGame()
{
    videoSetMode(MODE_FB0);

    vramSetBankA(VRAM_A_MAIN_BG);

    consoleDemoInit();

    for (int i = 0; i < MAX_BONES; i++)
    {
        bones[i].active = 0;
    }
}

// ==========================
// 🎮 UPDATE
// ==========================

void updateGame(int keys)
{
    touchPosition touch;

    touchRead(&touch);

    int usingPad = 0;
    int usingTouch = 0;

    // 🎮 FLECHES

    if (keys & KEY_LEFT)
    {
        player_x -= player_speed;
        usingPad = 1;
    }

    if (keys & KEY_RIGHT)
    {
        player_x += player_speed;
        usingPad = 1;
    }

    if (keys & KEY_UP)
    {
        player_y -= player_speed;
        usingPad = 1;
    }

    if (keys & KEY_DOWN)
    {
        player_y += player_speed;
        usingPad = 1;
    }

    // 📱 tactile

    if (keysHeld() & KEY_TOUCH)
    {
        usingTouch = 1;
    }

    // ⏸️ freeze si les deux

    if (!(usingPad && usingTouch))
    {
        if (usingTouch)
        {
            player_x = touch.px;
            player_y = touch.py;
        }
    }

    // 📦 limites vraie box

    if (player_x < box_x + 4)
        player_x = box_x + 4;

    if (player_x > box_x + box_w - 12)
        player_x = box_x + box_w - 12;

    if (player_y < box_y + 4)
        player_y = box_y + 4;

    if (player_y > box_y + box_h - 12)
        player_y = box_y + box_h - 12;

    // 🦴 spawn

    spawn_timer++;

    if (spawn_timer > 20)
    {
        spawnBone();

        spawn_timer = 0;
    }

    // 💀 update os

    for (int i = 0; i < MAX_BONES; i++)
    {
        if (bones[i].active)
        {
            bones[i].y += bones[i].speed;

            // collision

            if (player_x < bones[i].x + 12 &&
                player_x + 8 > bones[i].x &&
                player_y < bones[i].y + 24 &&
                player_y + 10 > bones[i].y)
            {
                hp--;
            }

            // hors box

            if (bones[i].y > box_y + box_h)
            {
                bones[i].active = 0;
            }
        }
    }
}

// ==========================
// 🎨 DRAW
// ==========================

void drawGame()
{
    // 🖤 fond noir

    drawRect(0,0,256,192,RGB15(0,0,0));

    // 📦 vraie box

    drawBox();

    // ❤️ coeur

    drawHeart((int)player_x, (int)player_y);

    // 🦴 os

    for (int i = 0; i < MAX_BONES; i++)
    {
        if (bones[i].active)
        {
            drawBone(bones[i].x, bones[i].y);
        }
    }

    // 📊 UI

    iprintf(\"\\x1b[1;1HHP: %d   \", hp);

    // ☠️ game over

    if (hp <= 0)
    {
        drawRect(0,0,256,192,RGB15(0,0,0));

        iprintf(\"\\x1b[10;8HGAME OVER\");
    }
}