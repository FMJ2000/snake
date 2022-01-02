#ifndef _CONST_HPP
#define _CONST_HPP

#define TITLE "snaekkkk"
#define FRAME_RATE 30
#define FONT_FILE "textures/arial.ttf"
#define HS_FILE "data/score.csv"
#define PAUSE 0
#define PLAY 1
#define STOP 2

#define BORDER_WIDTH 3
#define NUM_FRUIT 10
#define NUM_WALLS 8
#define SNAKE_R 20
#define SNAKE_HEAD_R 12
#define SNAKE_T 2
#define SNAKE_V 30
#define SNAKE_MAX_V 10
#define FRUIT_R 15
#define LOG_D 6
#define LOG_T 0.2
#define MAX_SPAWN 5
#define COL_MOD 3
#define WALL_WIDTH 50
#define MAX_ITER 20
#define SPAWN_T 3

#define BACKGROUND 0x000033ff
#define MAP_COL 0x567d46ff
#define SNAKE_COL 0x880808ff
#define SNAKE_SPAWN_COL 0x8808087f
#define FRUIT_COL 0x6f2da8ff
#define WALL_COL 0x006400ff

#define POPUP_PAUSE "Press Spacebar to play"
#define POPUP_STOP "Welcome to Snaekkkk.\nEnter your username: "
#define LEADERBOARD "Leaderboard:\n"
#define CHR_ENTER 13
#define CHR_SPACE 32
#define CHR_BACKSPACE '\b'
#define CHR_AA 65
#define CHR_ZZ 90
#define CHR_A 97
#define CHR_Z 122
#define LEADER_MAX 10

const int WINDOW[2] = { 1600, 900 };
const int MAP[4] = { 50, 50, 1200, 800 };
const int PLAY_AREA[4] = { MAP[0] + SNAKE_R*2 + BORDER_WIDTH, MAP[1] + SNAKE_R*2 + BORDER_WIDTH, MAP[2] - SNAKE_R*4 - 2*BORDER_WIDTH, MAP[3] - SNAKE_R*4 - 2*BORDER_WIDTH };
const int INFO[2] = { 1300, 100 };
const int LEADER[2] = { 1300, 500 };
const int SNAKE_INIT[2] = { MAP[0] + MAP[2]/2, MAP[1] + MAP[3]/2 };
const int POPUP_INIT[2] = { 450, 450 };
const int FRUIT_INFO[2] = { 10, 3 };
const int WALL_LENGTH[2] = { 100, 400 };

#endif