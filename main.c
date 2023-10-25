#include "lib\SMSlib.h"
#include "lib\PSGlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include ".\banks\bank2.h"
#include ".\banks\bank3.h"
#include ".\banks\bank4.h"
#include ".\banks\bank5.h"
#include ".\banks\bank6.h"
#include ".\banks\bank7.h"
#include ".\banks\bank8.h"
#include ".\banks\bank9.h"
#include ".\banks\bank10.h"
#include ".\banks\bank11.h"
#include ".\banks\bank12.h"
#include ".\banks\bank13.h"
#include ".\banks\bank14.h"
#include ".\banks\bank15.h"
#include ".\banks\bank16.h"
#include ".\banks\bank17.h"
#include ".\banks\bank18.h"
#include ".\banks\bank19.h"
#include ".\banks\bank20.h"
#include ".\banks\bank21.h"
#include ".\banks\bank22.h"
#include ".\banks\bank23.h"
#include ".\banks\bank24.h"
#include ".\banks\bank25.h"
#include ".\banks\bank26.h"
#include ".\banks\bank27.h"

// Event types
#define EVENT_BEFORE_VBLANK 0
#define EVENT_VBLANK_LOAD 1
#define EVENT_AFTER_VBLANK 2

// Direction types
#define DIRECTION_NONE 0
#define DIRECTION_RIGHT 1
#define DIRECTION_UP 2
#define DIRECTION_LEFT 3
#define DIRECTION_DOWN 4

// Map types
#define MAP_TYPE_A1_BASE 1
#define MAP_TYPE_A1_ANGLERS_TUNNEL 2
#define MAP_TYPE_A1_ANIMAL_VILLAGE 3
#define MAP_TYPE_A1_BEACH 4
#define MAP_TYPE_A1_BOTTLE_GROTTO 5
#define MAP_TYPE_A1_CATFISH_MAW 6
#define MAP_TYPE_A1_CEMETARY 7
#define MAP_TYPE_A1_DESERT 8
#define MAP_TYPE_A1_EAGLES_TOWER 9
#define MAP_TYPE_A1_EGG 10
#define MAP_TYPE_A1_FOREST 11
#define MAP_TYPE_A1_MABE_VILLAGE 12
#define MAP_TYPE_A1_MANSION 13
#define MAP_TYPE_A1_SHOPS 14
#define MAP_TYPE_A1_SWAMP 15
#define MAP_TYPE_A1_TAIL_CAVE 16
#define MAP_TYPE_A1_TURTLE_ROCK 17
#define MAP_TYPE_A2_TAIL_CAVE_01 18
#define MAP_TYPE_A2_TAIL_CAVE_02 19
#define MAP_TYPE_A2_BOTTLE_GROTTO_01 20
#define MAP_TYPE_A2_BOTTLE_GROTTO_02 21
#define MAP_TYPE_A2_KEY_CAVERN_01 22
#define MAP_TYPE_A2_KEY_CAVERN_02 23
#define MAP_TYPE_A2_ANGLERS_TUNNEL_01 24
#define MAP_TYPE_A2_ANGLERS_TUNNEL_02 25
#define MAP_TYPE_A2_CATFISHS_MAW_01 26
#define MAP_TYPE_A2_CATFISHS_MAW_02 27
#define MAP_TYPE_A2_FACE_SHRINE_01 28
#define MAP_TYPE_A2_FACE_SHRINE_02 29
#define MAP_TYPE_A2_CAVES_01 30

// Game states
#define GAME_STATE_TITLE 1
#define GAME_STATE_INVENTORY 2
#define GAME_STATE_AREA_01 3
#define GAME_STATE_AREA_02 4
#define GAME_STATE_AREA_03 5

// Global variables
unsigned char Map[1280];            // Current and previous map data
unsigned char ScrollIndex;          // Map scroll index
unsigned char ScrollDir;            // Map scroll direction
unsigned char MapId;                // Current map id
unsigned char PrevMapId;            // Previous map id
unsigned char MapType;              // Current map type
unsigned char PrevMapType;          // Previous map type
unsigned char AreaBank;             // Current area array bank
const unsigned char* AreaBin;       // Current area array
unsigned char GameState;            // Overall program flow
unsigned char PrevGameState;        // Previous loaded state
unsigned char SubState;             // A game state's sub state, for managed flow of a game state
unsigned char SubStateMax;          // Substate threshold
unsigned char SubState2;            // A game state's sub state, for managed flow of a game state
unsigned char SubStateMax2;         // Substate threshold
unsigned char FrameCounter;         // Frame counter, used for various things
unsigned short Timer;               // General timer
unsigned short TimerMax;            // Timer threshold
unsigned short Timer2;              // General timer
unsigned short TimerMax2;           // Timer threshold
unsigned char AudioCurrentBank;     // Current audio bank
unsigned int KeysPressed;           // Keys pressed
unsigned int KeysHeld;              // Keys held down

// NOTE: Looks like the original game put all maps within 3 areas (256 byte arrays), sans color dungeon
// https://github.com/zladx/LADX-Disassembly/tree/main/docs
// TODO: Make the areas into bin files, instead of using RAM

// Area 01 map bin banks (Temp)
unsigned char Area01MapBanks[256] = {
    a01_00_00_map_bin_bank, a01_00_01_map_bin_bank, a01_00_02_map_bin_bank, a01_00_03_map_bin_bank, a01_00_04_map_bin_bank, a01_00_05_map_bin_bank, a01_00_06_map_bin_bank, a01_00_07_map_bin_bank, a01_00_08_map_bin_bank, a01_00_09_map_bin_bank, a01_00_10_map_bin_bank, a01_00_11_map_bin_bank, a01_00_12_map_bin_bank, a01_00_13_map_bin_bank, a01_00_14_map_bin_bank, a01_00_15_map_bin_bank,
    a01_01_00_map_bin_bank, a01_01_01_map_bin_bank, a01_01_02_map_bin_bank, a01_01_03_map_bin_bank, a01_01_04_map_bin_bank, a01_01_05_map_bin_bank, a01_01_06_map_bin_bank, a01_01_07_map_bin_bank, a01_01_08_map_bin_bank, a01_01_09_map_bin_bank, a01_01_10_map_bin_bank, a01_01_11_map_bin_bank, a01_01_12_map_bin_bank, a01_01_13_map_bin_bank, a01_01_14_map_bin_bank, a01_01_15_map_bin_bank,
    a01_02_00_map_bin_bank, a01_02_01_map_bin_bank, a01_02_02_map_bin_bank, a01_02_03_map_bin_bank, a01_02_04_map_bin_bank, a01_02_05_map_bin_bank, a01_02_06_map_bin_bank, a01_02_07_map_bin_bank, a01_02_08_map_bin_bank, a01_02_09_map_bin_bank, a01_02_10_map_bin_bank, a01_02_11_map_bin_bank, a01_02_12_map_bin_bank, a01_02_13_map_bin_bank, a01_02_14_map_bin_bank, a01_02_15_map_bin_bank,
    a01_03_00_map_bin_bank, a01_03_01_map_bin_bank, a01_03_02_map_bin_bank, a01_03_03_map_bin_bank, a01_03_04_map_bin_bank, a01_03_05_map_bin_bank, a01_03_06_map_bin_bank, a01_03_07_map_bin_bank, a01_03_08_map_bin_bank, a01_03_09_map_bin_bank, a01_03_10_map_bin_bank, a01_03_11_map_bin_bank, a01_03_12_map_bin_bank, a01_03_13_map_bin_bank, a01_03_14_map_bin_bank, a01_03_15_map_bin_bank,
    a01_04_00_map_bin_bank, a01_04_01_map_bin_bank, a01_04_02_map_bin_bank, a01_04_03_map_bin_bank, a01_04_04_map_bin_bank, a01_04_05_map_bin_bank, a01_04_06_map_bin_bank, a01_04_07_map_bin_bank, a01_04_08_map_bin_bank, a01_04_09_map_bin_bank, a01_04_10_map_bin_bank, a01_04_11_map_bin_bank, a01_04_12_map_bin_bank, a01_04_13_map_bin_bank, a01_04_14_map_bin_bank, a01_04_15_map_bin_bank,
    a01_05_00_map_bin_bank, a01_05_01_map_bin_bank, a01_05_02_map_bin_bank, a01_05_03_map_bin_bank, a01_05_04_map_bin_bank, a01_05_05_map_bin_bank, a01_05_06_map_bin_bank, a01_05_07_map_bin_bank, a01_05_08_map_bin_bank, a01_05_09_map_bin_bank, a01_05_10_map_bin_bank, a01_05_11_map_bin_bank, a01_05_12_map_bin_bank, a01_05_13_map_bin_bank, a01_05_14_map_bin_bank, a01_05_15_map_bin_bank,
    a01_06_00_map_bin_bank, a01_06_01_map_bin_bank, a01_06_02_map_bin_bank, a01_06_03_map_bin_bank, a01_06_04_map_bin_bank, a01_06_05_map_bin_bank, a01_06_06_map_bin_bank, a01_06_07_map_bin_bank, a01_06_08_map_bin_bank, a01_06_09_map_bin_bank, a01_06_10_map_bin_bank, a01_06_11_map_bin_bank, a01_06_12_map_bin_bank, a01_06_13_map_bin_bank, a01_06_14_map_bin_bank, a01_06_15_map_bin_bank,
    a01_07_00_map_bin_bank, a01_07_01_map_bin_bank, a01_07_02_map_bin_bank, a01_07_03_map_bin_bank, a01_07_04_map_bin_bank, a01_07_05_map_bin_bank, a01_07_06_map_bin_bank, a01_07_07_map_bin_bank, a01_07_08_map_bin_bank, a01_07_09_map_bin_bank, a01_07_10_map_bin_bank, a01_07_11_map_bin_bank, a01_07_12_map_bin_bank, a01_07_13_map_bin_bank, a01_07_14_map_bin_bank, a01_07_15_map_bin_bank,
    a01_08_00_map_bin_bank, a01_08_01_map_bin_bank, a01_08_02_map_bin_bank, a01_08_03_map_bin_bank, a01_08_04_map_bin_bank, a01_08_05_map_bin_bank, a01_08_06_map_bin_bank, a01_08_07_map_bin_bank, a01_08_08_map_bin_bank, a01_08_09_map_bin_bank, a01_08_10_map_bin_bank, a01_08_11_map_bin_bank, a01_08_12_map_bin_bank, a01_08_13_map_bin_bank, a01_08_14_map_bin_bank, a01_08_15_map_bin_bank,
    a01_09_00_map_bin_bank, a01_09_01_map_bin_bank, a01_09_02_map_bin_bank, a01_09_03_map_bin_bank, a01_09_04_map_bin_bank, a01_09_05_map_bin_bank, a01_09_06_map_bin_bank, a01_09_07_map_bin_bank, a01_09_08_map_bin_bank, a01_09_09_map_bin_bank, a01_09_10_map_bin_bank, a01_09_11_map_bin_bank, a01_09_12_map_bin_bank, a01_09_13_map_bin_bank, a01_09_14_map_bin_bank, a01_09_15_map_bin_bank,
    a01_10_00_map_bin_bank, a01_10_01_map_bin_bank, a01_10_02_map_bin_bank, a01_10_03_map_bin_bank, a01_10_04_map_bin_bank, a01_10_05_map_bin_bank, a01_10_06_map_bin_bank, a01_10_07_map_bin_bank, a01_10_08_map_bin_bank, a01_10_09_map_bin_bank, a01_10_10_map_bin_bank, a01_10_11_map_bin_bank, a01_10_12_map_bin_bank, a01_10_13_map_bin_bank, a01_10_14_map_bin_bank, a01_10_15_map_bin_bank,
    a01_11_00_map_bin_bank, a01_11_01_map_bin_bank, a01_11_02_map_bin_bank, a01_11_03_map_bin_bank, a01_11_04_map_bin_bank, a01_11_05_map_bin_bank, a01_11_06_map_bin_bank, a01_11_07_map_bin_bank, a01_11_08_map_bin_bank, a01_11_09_map_bin_bank, a01_11_10_map_bin_bank, a01_11_11_map_bin_bank, a01_11_12_map_bin_bank, a01_11_13_map_bin_bank, a01_11_14_map_bin_bank, a01_11_15_map_bin_bank,
    a01_12_00_map_bin_bank, a01_12_01_map_bin_bank, a01_12_02_map_bin_bank, a01_12_03_map_bin_bank, a01_12_04_map_bin_bank, a01_12_05_map_bin_bank, a01_12_06_map_bin_bank, a01_12_07_map_bin_bank, a01_12_08_map_bin_bank, a01_12_09_map_bin_bank, a01_12_10_map_bin_bank, a01_12_11_map_bin_bank, a01_12_12_map_bin_bank, a01_12_13_map_bin_bank, a01_12_14_map_bin_bank, a01_12_15_map_bin_bank,
    a01_13_00_map_bin_bank, a01_13_01_map_bin_bank, a01_13_02_map_bin_bank, a01_13_03_map_bin_bank, a01_13_04_map_bin_bank, a01_13_05_map_bin_bank, a01_13_06_map_bin_bank, a01_13_07_map_bin_bank, a01_13_08_map_bin_bank, a01_13_09_map_bin_bank, a01_13_10_map_bin_bank, a01_13_11_map_bin_bank, a01_13_12_map_bin_bank, a01_13_13_map_bin_bank, a01_13_14_map_bin_bank, a01_13_15_map_bin_bank,
    a01_14_00_map_bin_bank, a01_14_01_map_bin_bank, a01_14_02_map_bin_bank, a01_14_03_map_bin_bank, a01_14_04_map_bin_bank, a01_14_05_map_bin_bank, a01_14_06_map_bin_bank, a01_14_07_map_bin_bank, a01_14_08_map_bin_bank, a01_14_09_map_bin_bank, a01_14_10_map_bin_bank, a01_14_11_map_bin_bank, a01_14_12_map_bin_bank, a01_14_13_map_bin_bank, a01_14_14_map_bin_bank, a01_14_15_map_bin_bank,
    a01_15_00_map_bin_bank, a01_15_01_map_bin_bank, a01_15_02_map_bin_bank, a01_15_03_map_bin_bank, a01_15_04_map_bin_bank, a01_15_05_map_bin_bank, a01_15_06_map_bin_bank, a01_15_07_map_bin_bank, a01_15_08_map_bin_bank, a01_15_09_map_bin_bank, a01_15_10_map_bin_bank, a01_15_11_map_bin_bank, a01_15_12_map_bin_bank, a01_15_13_map_bin_bank, a01_15_14_map_bin_bank, a01_15_15_map_bin_bank
};

// Area 01 map bin pointers (Temp)
const unsigned char* Area01MapBins[256] = {
    a01_00_00_map_bin, a01_00_01_map_bin, a01_00_02_map_bin, a01_00_03_map_bin, a01_00_04_map_bin, a01_00_05_map_bin, a01_00_06_map_bin, a01_00_07_map_bin, a01_00_08_map_bin, a01_00_09_map_bin, a01_00_10_map_bin, a01_00_11_map_bin, a01_00_12_map_bin, a01_00_13_map_bin, a01_00_14_map_bin, a01_00_15_map_bin,
    a01_01_00_map_bin, a01_01_01_map_bin, a01_01_02_map_bin, a01_01_03_map_bin, a01_01_04_map_bin, a01_01_05_map_bin, a01_01_06_map_bin, a01_01_07_map_bin, a01_01_08_map_bin, a01_01_09_map_bin, a01_01_10_map_bin, a01_01_11_map_bin, a01_01_12_map_bin, a01_01_13_map_bin, a01_01_14_map_bin, a01_01_15_map_bin,
    a01_02_00_map_bin, a01_02_01_map_bin, a01_02_02_map_bin, a01_02_03_map_bin, a01_02_04_map_bin, a01_02_05_map_bin, a01_02_06_map_bin, a01_02_07_map_bin, a01_02_08_map_bin, a01_02_09_map_bin, a01_02_10_map_bin, a01_02_11_map_bin, a01_02_12_map_bin, a01_02_13_map_bin, a01_02_14_map_bin, a01_02_15_map_bin,
    a01_03_00_map_bin, a01_03_01_map_bin, a01_03_02_map_bin, a01_03_03_map_bin, a01_03_04_map_bin, a01_03_05_map_bin, a01_03_06_map_bin, a01_03_07_map_bin, a01_03_08_map_bin, a01_03_09_map_bin, a01_03_10_map_bin, a01_03_11_map_bin, a01_03_12_map_bin, a01_03_13_map_bin, a01_03_14_map_bin, a01_03_15_map_bin,
    a01_04_00_map_bin, a01_04_01_map_bin, a01_04_02_map_bin, a01_04_03_map_bin, a01_04_04_map_bin, a01_04_05_map_bin, a01_04_06_map_bin, a01_04_07_map_bin, a01_04_08_map_bin, a01_04_09_map_bin, a01_04_10_map_bin, a01_04_11_map_bin, a01_04_12_map_bin, a01_04_13_map_bin, a01_04_14_map_bin, a01_04_15_map_bin,
    a01_05_00_map_bin, a01_05_01_map_bin, a01_05_02_map_bin, a01_05_03_map_bin, a01_05_04_map_bin, a01_05_05_map_bin, a01_05_06_map_bin, a01_05_07_map_bin, a01_05_08_map_bin, a01_05_09_map_bin, a01_05_10_map_bin, a01_05_11_map_bin, a01_05_12_map_bin, a01_05_13_map_bin, a01_05_14_map_bin, a01_05_15_map_bin,
    a01_06_00_map_bin, a01_06_01_map_bin, a01_06_02_map_bin, a01_06_03_map_bin, a01_06_04_map_bin, a01_06_05_map_bin, a01_06_06_map_bin, a01_06_07_map_bin, a01_06_08_map_bin, a01_06_09_map_bin, a01_06_10_map_bin, a01_06_11_map_bin, a01_06_12_map_bin, a01_06_13_map_bin, a01_06_14_map_bin, a01_06_15_map_bin,
    a01_07_00_map_bin, a01_07_01_map_bin, a01_07_02_map_bin, a01_07_03_map_bin, a01_07_04_map_bin, a01_07_05_map_bin, a01_07_06_map_bin, a01_07_07_map_bin, a01_07_08_map_bin, a01_07_09_map_bin, a01_07_10_map_bin, a01_07_11_map_bin, a01_07_12_map_bin, a01_07_13_map_bin, a01_07_14_map_bin, a01_07_15_map_bin,
    a01_08_00_map_bin, a01_08_01_map_bin, a01_08_02_map_bin, a01_08_03_map_bin, a01_08_04_map_bin, a01_08_05_map_bin, a01_08_06_map_bin, a01_08_07_map_bin, a01_08_08_map_bin, a01_08_09_map_bin, a01_08_10_map_bin, a01_08_11_map_bin, a01_08_12_map_bin, a01_08_13_map_bin, a01_08_14_map_bin, a01_08_15_map_bin,
    a01_09_00_map_bin, a01_09_01_map_bin, a01_09_02_map_bin, a01_09_03_map_bin, a01_09_04_map_bin, a01_09_05_map_bin, a01_09_06_map_bin, a01_09_07_map_bin, a01_09_08_map_bin, a01_09_09_map_bin, a01_09_10_map_bin, a01_09_11_map_bin, a01_09_12_map_bin, a01_09_13_map_bin, a01_09_14_map_bin, a01_09_15_map_bin,
    a01_10_00_map_bin, a01_10_01_map_bin, a01_10_02_map_bin, a01_10_03_map_bin, a01_10_04_map_bin, a01_10_05_map_bin, a01_10_06_map_bin, a01_10_07_map_bin, a01_10_08_map_bin, a01_10_09_map_bin, a01_10_10_map_bin, a01_10_11_map_bin, a01_10_12_map_bin, a01_10_13_map_bin, a01_10_14_map_bin, a01_10_15_map_bin,
    a01_11_00_map_bin, a01_11_01_map_bin, a01_11_02_map_bin, a01_11_03_map_bin, a01_11_04_map_bin, a01_11_05_map_bin, a01_11_06_map_bin, a01_11_07_map_bin, a01_11_08_map_bin, a01_11_09_map_bin, a01_11_10_map_bin, a01_11_11_map_bin, a01_11_12_map_bin, a01_11_13_map_bin, a01_11_14_map_bin, a01_11_15_map_bin,
    a01_12_00_map_bin, a01_12_01_map_bin, a01_12_02_map_bin, a01_12_03_map_bin, a01_12_04_map_bin, a01_12_05_map_bin, a01_12_06_map_bin, a01_12_07_map_bin, a01_12_08_map_bin, a01_12_09_map_bin, a01_12_10_map_bin, a01_12_11_map_bin, a01_12_12_map_bin, a01_12_13_map_bin, a01_12_14_map_bin, a01_12_15_map_bin,
    a01_13_00_map_bin, a01_13_01_map_bin, a01_13_02_map_bin, a01_13_03_map_bin, a01_13_04_map_bin, a01_13_05_map_bin, a01_13_06_map_bin, a01_13_07_map_bin, a01_13_08_map_bin, a01_13_09_map_bin, a01_13_10_map_bin, a01_13_11_map_bin, a01_13_12_map_bin, a01_13_13_map_bin, a01_13_14_map_bin, a01_13_15_map_bin,
    a01_14_00_map_bin, a01_14_01_map_bin, a01_14_02_map_bin, a01_14_03_map_bin, a01_14_04_map_bin, a01_14_05_map_bin, a01_14_06_map_bin, a01_14_07_map_bin, a01_14_08_map_bin, a01_14_09_map_bin, a01_14_10_map_bin, a01_14_11_map_bin, a01_14_12_map_bin, a01_14_13_map_bin, a01_14_14_map_bin, a01_14_15_map_bin,
    a01_15_00_map_bin, a01_15_01_map_bin, a01_15_02_map_bin, a01_15_03_map_bin, a01_15_04_map_bin, a01_15_05_map_bin, a01_15_06_map_bin, a01_15_07_map_bin, a01_15_08_map_bin, a01_15_09_map_bin, a01_15_10_map_bin, a01_15_11_map_bin, a01_15_12_map_bin, a01_15_13_map_bin, a01_15_14_map_bin, a01_15_15_map_bin
};

// Area 02 map bin banks (Temp)
unsigned char Area02MapBanks[256] = {
    a02_00_00_map_bin_bank, a02_00_01_map_bin_bank, a02_00_02_map_bin_bank, a02_00_03_map_bin_bank, a02_00_04_map_bin_bank, a02_00_05_map_bin_bank, a02_00_06_map_bin_bank, a02_00_07_map_bin_bank, a02_00_08_map_bin_bank, a02_00_09_map_bin_bank, a02_00_10_map_bin_bank, a02_00_11_map_bin_bank, a02_00_12_map_bin_bank, a02_00_13_map_bin_bank, a02_00_14_map_bin_bank, a02_00_15_map_bin_bank,
    a02_01_00_map_bin_bank, a02_01_01_map_bin_bank, a02_01_02_map_bin_bank, a02_01_03_map_bin_bank, a02_01_04_map_bin_bank, a02_01_05_map_bin_bank, a02_01_06_map_bin_bank, a02_01_07_map_bin_bank, a02_01_08_map_bin_bank, a02_01_09_map_bin_bank, a02_01_10_map_bin_bank, a02_01_11_map_bin_bank, a02_01_12_map_bin_bank, a02_01_13_map_bin_bank, a02_01_14_map_bin_bank, a02_01_15_map_bin_bank,
    a02_02_00_map_bin_bank, a02_02_01_map_bin_bank, a02_02_02_map_bin_bank, a02_02_03_map_bin_bank, a02_02_04_map_bin_bank, a02_02_05_map_bin_bank, a02_02_06_map_bin_bank, a02_02_07_map_bin_bank, a02_02_08_map_bin_bank, a02_02_09_map_bin_bank, a02_02_10_map_bin_bank, a02_02_11_map_bin_bank, a02_02_12_map_bin_bank, a02_02_13_map_bin_bank, a02_02_14_map_bin_bank, a02_02_15_map_bin_bank,
    a02_03_00_map_bin_bank, a02_03_01_map_bin_bank, a02_03_02_map_bin_bank, a02_03_03_map_bin_bank, a02_03_04_map_bin_bank, a02_03_05_map_bin_bank, a02_03_06_map_bin_bank, a02_03_07_map_bin_bank, a02_03_08_map_bin_bank, a02_03_09_map_bin_bank, a02_03_10_map_bin_bank, a02_03_11_map_bin_bank, a02_03_12_map_bin_bank, a02_03_13_map_bin_bank, a02_03_14_map_bin_bank, a02_03_15_map_bin_bank,
    a02_04_00_map_bin_bank, a02_04_01_map_bin_bank, a02_04_02_map_bin_bank, a02_04_03_map_bin_bank, a02_04_04_map_bin_bank, a02_04_05_map_bin_bank, a02_04_06_map_bin_bank, a02_04_07_map_bin_bank, a02_04_08_map_bin_bank, a02_04_09_map_bin_bank, a02_04_10_map_bin_bank, a02_04_11_map_bin_bank, a02_04_12_map_bin_bank, a02_04_13_map_bin_bank, a02_04_14_map_bin_bank, a02_04_15_map_bin_bank,
    a02_05_00_map_bin_bank, a02_05_01_map_bin_bank, a02_05_02_map_bin_bank, a02_05_03_map_bin_bank, a02_05_04_map_bin_bank, a02_05_05_map_bin_bank, a02_05_06_map_bin_bank, a02_05_07_map_bin_bank, a02_05_08_map_bin_bank, a02_05_09_map_bin_bank, a02_05_10_map_bin_bank, a02_05_11_map_bin_bank, a02_05_12_map_bin_bank, a02_05_13_map_bin_bank, a02_05_14_map_bin_bank, a02_05_15_map_bin_bank,
    a02_06_00_map_bin_bank, a02_06_01_map_bin_bank, a02_06_02_map_bin_bank, a02_06_03_map_bin_bank, a02_06_04_map_bin_bank, a02_06_05_map_bin_bank, a02_06_06_map_bin_bank, a02_06_07_map_bin_bank, a02_06_08_map_bin_bank, a02_06_09_map_bin_bank, a02_06_10_map_bin_bank, a02_06_11_map_bin_bank, a02_06_12_map_bin_bank, a02_06_13_map_bin_bank, a02_06_14_map_bin_bank, a02_06_15_map_bin_bank,
    a02_07_00_map_bin_bank, a02_07_01_map_bin_bank, a02_07_02_map_bin_bank, a02_07_03_map_bin_bank, a02_07_04_map_bin_bank, a02_07_05_map_bin_bank, a02_07_06_map_bin_bank, a02_07_07_map_bin_bank, a02_07_08_map_bin_bank, a02_07_09_map_bin_bank, a02_07_10_map_bin_bank, a02_07_11_map_bin_bank, a02_07_12_map_bin_bank, a02_07_13_map_bin_bank, a02_07_14_map_bin_bank, a02_07_15_map_bin_bank,
    a02_08_00_map_bin_bank, a02_08_01_map_bin_bank, a02_08_02_map_bin_bank, a02_08_03_map_bin_bank, a02_08_04_map_bin_bank, a02_08_05_map_bin_bank, a02_08_06_map_bin_bank, a02_08_07_map_bin_bank, a02_08_08_map_bin_bank, a02_08_09_map_bin_bank, a02_08_10_map_bin_bank, a02_08_11_map_bin_bank, a02_08_12_map_bin_bank, a02_08_13_map_bin_bank, a02_08_14_map_bin_bank, a02_08_15_map_bin_bank,
    a02_09_00_map_bin_bank, a02_09_01_map_bin_bank, a02_09_02_map_bin_bank, a02_09_03_map_bin_bank, a02_09_04_map_bin_bank, a02_09_05_map_bin_bank, a02_09_06_map_bin_bank, a02_09_07_map_bin_bank, a02_09_08_map_bin_bank, a02_09_09_map_bin_bank, a02_09_10_map_bin_bank, a02_09_11_map_bin_bank, a02_09_12_map_bin_bank, a02_09_13_map_bin_bank, a02_09_14_map_bin_bank, a02_09_15_map_bin_bank,
    a02_10_00_map_bin_bank, a02_10_01_map_bin_bank, a02_10_02_map_bin_bank, a02_10_03_map_bin_bank, a02_10_04_map_bin_bank, a02_10_05_map_bin_bank, a02_10_06_map_bin_bank, a02_10_07_map_bin_bank, a02_10_08_map_bin_bank, a02_10_09_map_bin_bank, a02_10_10_map_bin_bank, a02_10_11_map_bin_bank, a02_10_12_map_bin_bank, a02_10_13_map_bin_bank, a02_10_14_map_bin_bank, a02_10_15_map_bin_bank,
    a02_11_00_map_bin_bank, a02_11_01_map_bin_bank, a02_11_02_map_bin_bank, a02_11_03_map_bin_bank, a02_11_04_map_bin_bank, a02_11_05_map_bin_bank, a02_11_06_map_bin_bank, a02_11_07_map_bin_bank, a02_11_08_map_bin_bank, a02_11_09_map_bin_bank, a02_11_10_map_bin_bank, a02_11_11_map_bin_bank, a02_11_12_map_bin_bank, a02_11_13_map_bin_bank, a02_11_14_map_bin_bank, a02_11_15_map_bin_bank,
    a02_12_00_map_bin_bank, a02_12_01_map_bin_bank, a02_12_02_map_bin_bank, a02_12_03_map_bin_bank, a02_12_04_map_bin_bank, a02_12_05_map_bin_bank, a02_12_06_map_bin_bank, a02_12_07_map_bin_bank, a02_12_08_map_bin_bank, a02_12_09_map_bin_bank, a02_12_10_map_bin_bank, a02_12_11_map_bin_bank, a02_12_12_map_bin_bank, a02_12_13_map_bin_bank, a02_12_14_map_bin_bank, a02_12_15_map_bin_bank,
    a02_13_00_map_bin_bank, a02_13_01_map_bin_bank, a02_13_02_map_bin_bank, a02_13_03_map_bin_bank, a02_13_04_map_bin_bank, a02_13_05_map_bin_bank, a02_13_06_map_bin_bank, a02_13_07_map_bin_bank, a02_13_08_map_bin_bank, a02_13_09_map_bin_bank, a02_13_10_map_bin_bank, a02_13_11_map_bin_bank, a02_13_12_map_bin_bank, a02_13_13_map_bin_bank, a02_13_14_map_bin_bank, a02_13_15_map_bin_bank,
    a02_14_00_map_bin_bank, a02_14_01_map_bin_bank, a02_14_02_map_bin_bank, a02_14_03_map_bin_bank, a02_14_04_map_bin_bank, a02_14_05_map_bin_bank, a02_14_06_map_bin_bank, a02_14_07_map_bin_bank, a02_14_08_map_bin_bank, a02_14_09_map_bin_bank, a02_14_10_map_bin_bank, a02_14_11_map_bin_bank, a02_14_12_map_bin_bank, a02_14_13_map_bin_bank, a02_14_14_map_bin_bank, a02_14_15_map_bin_bank,
    a02_15_00_map_bin_bank, a02_15_01_map_bin_bank, a02_15_02_map_bin_bank, a02_15_03_map_bin_bank, a02_15_04_map_bin_bank, a02_15_05_map_bin_bank, a02_15_06_map_bin_bank, a02_15_07_map_bin_bank, a02_15_08_map_bin_bank, a02_15_09_map_bin_bank, a02_15_10_map_bin_bank, a02_15_11_map_bin_bank, a02_15_12_map_bin_bank, a02_15_13_map_bin_bank, a02_15_14_map_bin_bank, a02_15_15_map_bin_bank
};

// Area 02 map bin pointers (Temp)
const unsigned char* Area02MapBins[256] = {
    a02_00_00_map_bin, a02_00_01_map_bin, a02_00_02_map_bin, a02_00_03_map_bin, a02_00_04_map_bin, a02_00_05_map_bin, a02_00_06_map_bin, a02_00_07_map_bin, a02_00_08_map_bin, a02_00_09_map_bin, a02_00_10_map_bin, a02_00_11_map_bin, a02_00_12_map_bin, a02_00_13_map_bin, a02_00_14_map_bin, a02_00_15_map_bin,
    a02_01_00_map_bin, a02_01_01_map_bin, a02_01_02_map_bin, a02_01_03_map_bin, a02_01_04_map_bin, a02_01_05_map_bin, a02_01_06_map_bin, a02_01_07_map_bin, a02_01_08_map_bin, a02_01_09_map_bin, a02_01_10_map_bin, a02_01_11_map_bin, a02_01_12_map_bin, a02_01_13_map_bin, a02_01_14_map_bin, a02_01_15_map_bin,
    a02_02_00_map_bin, a02_02_01_map_bin, a02_02_02_map_bin, a02_02_03_map_bin, a02_02_04_map_bin, a02_02_05_map_bin, a02_02_06_map_bin, a02_02_07_map_bin, a02_02_08_map_bin, a02_02_09_map_bin, a02_02_10_map_bin, a02_02_11_map_bin, a02_02_12_map_bin, a02_02_13_map_bin, a02_02_14_map_bin, a02_02_15_map_bin,
    a02_03_00_map_bin, a02_03_01_map_bin, a02_03_02_map_bin, a02_03_03_map_bin, a02_03_04_map_bin, a02_03_05_map_bin, a02_03_06_map_bin, a02_03_07_map_bin, a02_03_08_map_bin, a02_03_09_map_bin, a02_03_10_map_bin, a02_03_11_map_bin, a02_03_12_map_bin, a02_03_13_map_bin, a02_03_14_map_bin, a02_03_15_map_bin,
    a02_04_00_map_bin, a02_04_01_map_bin, a02_04_02_map_bin, a02_04_03_map_bin, a02_04_04_map_bin, a02_04_05_map_bin, a02_04_06_map_bin, a02_04_07_map_bin, a02_04_08_map_bin, a02_04_09_map_bin, a02_04_10_map_bin, a02_04_11_map_bin, a02_04_12_map_bin, a02_04_13_map_bin, a02_04_14_map_bin, a02_04_15_map_bin,
    a02_05_00_map_bin, a02_05_01_map_bin, a02_05_02_map_bin, a02_05_03_map_bin, a02_05_04_map_bin, a02_05_05_map_bin, a02_05_06_map_bin, a02_05_07_map_bin, a02_05_08_map_bin, a02_05_09_map_bin, a02_05_10_map_bin, a02_05_11_map_bin, a02_05_12_map_bin, a02_05_13_map_bin, a02_05_14_map_bin, a02_05_15_map_bin,
    a02_06_00_map_bin, a02_06_01_map_bin, a02_06_02_map_bin, a02_06_03_map_bin, a02_06_04_map_bin, a02_06_05_map_bin, a02_06_06_map_bin, a02_06_07_map_bin, a02_06_08_map_bin, a02_06_09_map_bin, a02_06_10_map_bin, a02_06_11_map_bin, a02_06_12_map_bin, a02_06_13_map_bin, a02_06_14_map_bin, a02_06_15_map_bin,
    a02_07_00_map_bin, a02_07_01_map_bin, a02_07_02_map_bin, a02_07_03_map_bin, a02_07_04_map_bin, a02_07_05_map_bin, a02_07_06_map_bin, a02_07_07_map_bin, a02_07_08_map_bin, a02_07_09_map_bin, a02_07_10_map_bin, a02_07_11_map_bin, a02_07_12_map_bin, a02_07_13_map_bin, a02_07_14_map_bin, a02_07_15_map_bin,
    a02_08_00_map_bin, a02_08_01_map_bin, a02_08_02_map_bin, a02_08_03_map_bin, a02_08_04_map_bin, a02_08_05_map_bin, a02_08_06_map_bin, a02_08_07_map_bin, a02_08_08_map_bin, a02_08_09_map_bin, a02_08_10_map_bin, a02_08_11_map_bin, a02_08_12_map_bin, a02_08_13_map_bin, a02_08_14_map_bin, a02_08_15_map_bin,
    a02_09_00_map_bin, a02_09_01_map_bin, a02_09_02_map_bin, a02_09_03_map_bin, a02_09_04_map_bin, a02_09_05_map_bin, a02_09_06_map_bin, a02_09_07_map_bin, a02_09_08_map_bin, a02_09_09_map_bin, a02_09_10_map_bin, a02_09_11_map_bin, a02_09_12_map_bin, a02_09_13_map_bin, a02_09_14_map_bin, a02_09_15_map_bin,
    a02_10_00_map_bin, a02_10_01_map_bin, a02_10_02_map_bin, a02_10_03_map_bin, a02_10_04_map_bin, a02_10_05_map_bin, a02_10_06_map_bin, a02_10_07_map_bin, a02_10_08_map_bin, a02_10_09_map_bin, a02_10_10_map_bin, a02_10_11_map_bin, a02_10_12_map_bin, a02_10_13_map_bin, a02_10_14_map_bin, a02_10_15_map_bin,
    a02_11_00_map_bin, a02_11_01_map_bin, a02_11_02_map_bin, a02_11_03_map_bin, a02_11_04_map_bin, a02_11_05_map_bin, a02_11_06_map_bin, a02_11_07_map_bin, a02_11_08_map_bin, a02_11_09_map_bin, a02_11_10_map_bin, a02_11_11_map_bin, a02_11_12_map_bin, a02_11_13_map_bin, a02_11_14_map_bin, a02_11_15_map_bin,
    a02_12_00_map_bin, a02_12_01_map_bin, a02_12_02_map_bin, a02_12_03_map_bin, a02_12_04_map_bin, a02_12_05_map_bin, a02_12_06_map_bin, a02_12_07_map_bin, a02_12_08_map_bin, a02_12_09_map_bin, a02_12_10_map_bin, a02_12_11_map_bin, a02_12_12_map_bin, a02_12_13_map_bin, a02_12_14_map_bin, a02_12_15_map_bin,
    a02_13_00_map_bin, a02_13_01_map_bin, a02_13_02_map_bin, a02_13_03_map_bin, a02_13_04_map_bin, a02_13_05_map_bin, a02_13_06_map_bin, a02_13_07_map_bin, a02_13_08_map_bin, a02_13_09_map_bin, a02_13_10_map_bin, a02_13_11_map_bin, a02_13_12_map_bin, a02_13_13_map_bin, a02_13_14_map_bin, a02_13_15_map_bin,
    a02_14_00_map_bin, a02_14_01_map_bin, a02_14_02_map_bin, a02_14_03_map_bin, a02_14_04_map_bin, a02_14_05_map_bin, a02_14_06_map_bin, a02_14_07_map_bin, a02_14_08_map_bin, a02_14_09_map_bin, a02_14_10_map_bin, a02_14_11_map_bin, a02_14_12_map_bin, a02_14_13_map_bin, a02_14_14_map_bin, a02_14_15_map_bin,
    a02_15_00_map_bin, a02_15_01_map_bin, a02_15_02_map_bin, a02_15_03_map_bin, a02_15_04_map_bin, a02_15_05_map_bin, a02_15_06_map_bin, a02_15_07_map_bin, a02_15_08_map_bin, a02_15_09_map_bin, a02_15_10_map_bin, a02_15_11_map_bin, a02_15_12_map_bin, a02_15_13_map_bin, a02_15_14_map_bin, a02_15_15_map_bin
};

// Area 03 map bin banks (Temp)
unsigned char Area03MapBanks[256] = {
    a01_00_00_map_bin_bank, a01_01_00_map_bin_bank, a01_02_00_map_bin_bank, a01_03_00_map_bin_bank, a01_04_00_map_bin_bank, a01_05_00_map_bin_bank, a01_06_00_map_bin_bank, a01_07_00_map_bin_bank, a01_08_00_map_bin_bank, a01_09_00_map_bin_bank, a01_10_00_map_bin_bank, a01_11_00_map_bin_bank, a01_12_00_map_bin_bank, a01_13_00_map_bin_bank, a01_14_00_map_bin_bank, a01_15_00_map_bin_bank,
    a01_00_01_map_bin_bank, a01_01_01_map_bin_bank, a01_02_01_map_bin_bank, a01_03_01_map_bin_bank, a01_04_01_map_bin_bank, a01_05_01_map_bin_bank, a01_06_01_map_bin_bank, a01_07_01_map_bin_bank, a01_08_01_map_bin_bank, a01_09_01_map_bin_bank, a01_10_01_map_bin_bank, a01_11_01_map_bin_bank, a01_12_01_map_bin_bank, a01_13_01_map_bin_bank, a01_14_01_map_bin_bank, a01_15_01_map_bin_bank,
    a01_00_02_map_bin_bank, a01_01_02_map_bin_bank, a01_02_02_map_bin_bank, a01_03_02_map_bin_bank, a01_04_02_map_bin_bank, a01_05_02_map_bin_bank, a01_06_02_map_bin_bank, a01_07_02_map_bin_bank, a01_08_02_map_bin_bank, a01_09_02_map_bin_bank, a01_10_02_map_bin_bank, a01_11_02_map_bin_bank, a01_12_02_map_bin_bank, a01_13_02_map_bin_bank, a01_14_02_map_bin_bank, a01_15_02_map_bin_bank,
    a01_00_03_map_bin_bank, a01_01_03_map_bin_bank, a01_02_03_map_bin_bank, a01_03_03_map_bin_bank, a01_04_03_map_bin_bank, a01_05_03_map_bin_bank, a01_06_03_map_bin_bank, a01_07_03_map_bin_bank, a01_08_03_map_bin_bank, a01_09_03_map_bin_bank, a01_10_03_map_bin_bank, a01_11_03_map_bin_bank, a01_12_03_map_bin_bank, a01_13_03_map_bin_bank, a01_14_03_map_bin_bank, a01_15_03_map_bin_bank,
    a01_00_04_map_bin_bank, a01_01_04_map_bin_bank, a01_02_04_map_bin_bank, a01_03_04_map_bin_bank, a01_04_04_map_bin_bank, a01_05_04_map_bin_bank, a01_06_04_map_bin_bank, a01_07_04_map_bin_bank, a01_08_04_map_bin_bank, a01_09_04_map_bin_bank, a01_10_04_map_bin_bank, a01_11_04_map_bin_bank, a01_12_04_map_bin_bank, a01_13_04_map_bin_bank, a01_14_04_map_bin_bank, a01_15_04_map_bin_bank,
    a01_00_05_map_bin_bank, a01_01_05_map_bin_bank, a01_02_05_map_bin_bank, a01_03_05_map_bin_bank, a01_04_05_map_bin_bank, a01_05_05_map_bin_bank, a01_06_05_map_bin_bank, a01_07_05_map_bin_bank, a01_08_05_map_bin_bank, a01_09_05_map_bin_bank, a01_10_05_map_bin_bank, a01_11_05_map_bin_bank, a01_12_05_map_bin_bank, a01_13_05_map_bin_bank, a01_14_05_map_bin_bank, a01_15_05_map_bin_bank,
    a01_00_06_map_bin_bank, a01_01_06_map_bin_bank, a01_02_06_map_bin_bank, a01_03_06_map_bin_bank, a01_04_06_map_bin_bank, a01_05_06_map_bin_bank, a01_06_06_map_bin_bank, a01_07_06_map_bin_bank, a01_08_06_map_bin_bank, a01_09_06_map_bin_bank, a01_10_06_map_bin_bank, a01_11_06_map_bin_bank, a01_12_06_map_bin_bank, a01_13_06_map_bin_bank, a01_14_06_map_bin_bank, a01_15_06_map_bin_bank,
    a01_00_07_map_bin_bank, a01_01_07_map_bin_bank, a01_02_07_map_bin_bank, a01_03_07_map_bin_bank, a01_04_07_map_bin_bank, a01_05_07_map_bin_bank, a01_06_07_map_bin_bank, a01_07_07_map_bin_bank, a01_08_07_map_bin_bank, a01_09_07_map_bin_bank, a01_10_07_map_bin_bank, a01_11_07_map_bin_bank, a01_12_07_map_bin_bank, a01_13_07_map_bin_bank, a01_14_07_map_bin_bank, a01_15_07_map_bin_bank,
    a01_00_08_map_bin_bank, a01_01_08_map_bin_bank, a01_02_08_map_bin_bank, a01_03_08_map_bin_bank, a01_04_08_map_bin_bank, a01_05_08_map_bin_bank, a01_06_08_map_bin_bank, a01_07_08_map_bin_bank, a01_08_08_map_bin_bank, a01_09_08_map_bin_bank, a01_10_08_map_bin_bank, a01_11_08_map_bin_bank, a01_12_08_map_bin_bank, a01_13_08_map_bin_bank, a01_14_08_map_bin_bank, a01_15_08_map_bin_bank,
    a01_00_09_map_bin_bank, a01_01_09_map_bin_bank, a01_02_09_map_bin_bank, a01_03_09_map_bin_bank, a01_04_09_map_bin_bank, a01_05_09_map_bin_bank, a01_06_09_map_bin_bank, a01_07_09_map_bin_bank, a01_08_09_map_bin_bank, a01_09_09_map_bin_bank, a01_10_09_map_bin_bank, a01_11_09_map_bin_bank, a01_12_09_map_bin_bank, a01_13_09_map_bin_bank, a01_14_09_map_bin_bank, a01_15_09_map_bin_bank,
    a01_00_10_map_bin_bank, a01_01_10_map_bin_bank, a01_02_10_map_bin_bank, a01_03_10_map_bin_bank, a01_04_10_map_bin_bank, a01_05_10_map_bin_bank, a01_06_10_map_bin_bank, a01_07_10_map_bin_bank, a01_08_10_map_bin_bank, a01_09_10_map_bin_bank, a01_10_10_map_bin_bank, a01_11_10_map_bin_bank, a01_12_10_map_bin_bank, a01_13_10_map_bin_bank, a01_14_10_map_bin_bank, a01_15_10_map_bin_bank,
    a01_00_11_map_bin_bank, a01_01_11_map_bin_bank, a01_02_11_map_bin_bank, a01_03_11_map_bin_bank, a01_04_11_map_bin_bank, a01_05_11_map_bin_bank, a01_06_11_map_bin_bank, a01_07_11_map_bin_bank, a01_08_11_map_bin_bank, a01_09_11_map_bin_bank, a01_10_11_map_bin_bank, a01_11_11_map_bin_bank, a01_12_11_map_bin_bank, a01_13_11_map_bin_bank, a01_14_11_map_bin_bank, a01_15_11_map_bin_bank,
    a01_00_12_map_bin_bank, a01_01_12_map_bin_bank, a01_02_12_map_bin_bank, a01_03_12_map_bin_bank, a01_04_12_map_bin_bank, a01_05_12_map_bin_bank, a01_06_12_map_bin_bank, a01_07_12_map_bin_bank, a01_08_12_map_bin_bank, a01_09_12_map_bin_bank, a01_10_12_map_bin_bank, a01_11_12_map_bin_bank, a01_12_12_map_bin_bank, a01_13_12_map_bin_bank, a01_14_12_map_bin_bank, a01_15_12_map_bin_bank,
    a01_00_13_map_bin_bank, a01_01_13_map_bin_bank, a01_02_13_map_bin_bank, a01_03_13_map_bin_bank, a01_04_13_map_bin_bank, a01_05_13_map_bin_bank, a01_06_13_map_bin_bank, a01_07_13_map_bin_bank, a01_08_13_map_bin_bank, a01_09_13_map_bin_bank, a01_10_13_map_bin_bank, a01_11_13_map_bin_bank, a01_12_13_map_bin_bank, a01_13_13_map_bin_bank, a01_14_13_map_bin_bank, a01_15_13_map_bin_bank,
    a01_00_14_map_bin_bank, a01_01_14_map_bin_bank, a01_02_14_map_bin_bank, a01_03_14_map_bin_bank, a01_04_14_map_bin_bank, a01_05_14_map_bin_bank, a01_06_14_map_bin_bank, a01_07_14_map_bin_bank, a01_08_14_map_bin_bank, a01_09_14_map_bin_bank, a01_10_14_map_bin_bank, a01_11_14_map_bin_bank, a01_12_14_map_bin_bank, a01_13_14_map_bin_bank, a01_14_14_map_bin_bank, a01_15_14_map_bin_bank,
    a01_00_15_map_bin_bank, a01_01_15_map_bin_bank, a01_02_15_map_bin_bank, a01_03_15_map_bin_bank, a01_04_15_map_bin_bank, a01_05_15_map_bin_bank, a01_06_15_map_bin_bank, a01_07_15_map_bin_bank, a01_08_15_map_bin_bank, a01_09_15_map_bin_bank, a01_10_15_map_bin_bank, a01_11_15_map_bin_bank, a01_12_15_map_bin_bank, a01_13_15_map_bin_bank, a01_14_15_map_bin_bank, a01_15_15_map_bin_bank
};

// Area 03 map bin pointers (Temp)
const unsigned char* Area03MapBins[256] = {
    a01_00_00_map_bin, a01_01_00_map_bin, a01_02_00_map_bin, a01_03_00_map_bin, a01_04_00_map_bin, a01_05_00_map_bin, a01_06_00_map_bin, a01_07_00_map_bin, a01_08_00_map_bin, a01_09_00_map_bin, a01_10_00_map_bin, a01_11_00_map_bin, a01_12_00_map_bin, a01_13_00_map_bin, a01_14_00_map_bin, a01_15_00_map_bin,
    a01_00_01_map_bin, a01_01_01_map_bin, a01_02_01_map_bin, a01_03_01_map_bin, a01_04_01_map_bin, a01_05_01_map_bin, a01_06_01_map_bin, a01_07_01_map_bin, a01_08_01_map_bin, a01_09_01_map_bin, a01_10_01_map_bin, a01_11_01_map_bin, a01_12_01_map_bin, a01_13_01_map_bin, a01_14_01_map_bin, a01_15_01_map_bin,
    a01_00_02_map_bin, a01_01_02_map_bin, a01_02_02_map_bin, a01_03_02_map_bin, a01_04_02_map_bin, a01_05_02_map_bin, a01_06_02_map_bin, a01_07_02_map_bin, a01_08_02_map_bin, a01_09_02_map_bin, a01_10_02_map_bin, a01_11_02_map_bin, a01_12_02_map_bin, a01_13_02_map_bin, a01_14_02_map_bin, a01_15_02_map_bin,
    a01_00_03_map_bin, a01_01_03_map_bin, a01_02_03_map_bin, a01_03_03_map_bin, a01_04_03_map_bin, a01_05_03_map_bin, a01_06_03_map_bin, a01_07_03_map_bin, a01_08_03_map_bin, a01_09_03_map_bin, a01_10_03_map_bin, a01_11_03_map_bin, a01_12_03_map_bin, a01_13_03_map_bin, a01_14_03_map_bin, a01_15_03_map_bin,
    a01_00_04_map_bin, a01_01_04_map_bin, a01_02_04_map_bin, a01_03_04_map_bin, a01_04_04_map_bin, a01_05_04_map_bin, a01_06_04_map_bin, a01_07_04_map_bin, a01_08_04_map_bin, a01_09_04_map_bin, a01_10_04_map_bin, a01_11_04_map_bin, a01_12_04_map_bin, a01_13_04_map_bin, a01_14_04_map_bin, a01_15_04_map_bin,
    a01_00_05_map_bin, a01_01_05_map_bin, a01_02_05_map_bin, a01_03_05_map_bin, a01_04_05_map_bin, a01_05_05_map_bin, a01_06_05_map_bin, a01_07_05_map_bin, a01_08_05_map_bin, a01_09_05_map_bin, a01_10_05_map_bin, a01_11_05_map_bin, a01_12_05_map_bin, a01_13_05_map_bin, a01_14_05_map_bin, a01_15_05_map_bin,
    a01_00_06_map_bin, a01_01_06_map_bin, a01_02_06_map_bin, a01_03_06_map_bin, a01_04_06_map_bin, a01_05_06_map_bin, a01_06_06_map_bin, a01_07_06_map_bin, a01_08_06_map_bin, a01_09_06_map_bin, a01_10_06_map_bin, a01_11_06_map_bin, a01_12_06_map_bin, a01_13_06_map_bin, a01_14_06_map_bin, a01_15_06_map_bin,
    a01_00_07_map_bin, a01_01_07_map_bin, a01_02_07_map_bin, a01_03_07_map_bin, a01_04_07_map_bin, a01_05_07_map_bin, a01_06_07_map_bin, a01_07_07_map_bin, a01_08_07_map_bin, a01_09_07_map_bin, a01_10_07_map_bin, a01_11_07_map_bin, a01_12_07_map_bin, a01_13_07_map_bin, a01_14_07_map_bin, a01_15_07_map_bin,
    a01_00_08_map_bin, a01_01_08_map_bin, a01_02_08_map_bin, a01_03_08_map_bin, a01_04_08_map_bin, a01_05_08_map_bin, a01_06_08_map_bin, a01_07_08_map_bin, a01_08_08_map_bin, a01_09_08_map_bin, a01_10_08_map_bin, a01_11_08_map_bin, a01_12_08_map_bin, a01_13_08_map_bin, a01_14_08_map_bin, a01_15_08_map_bin,
    a01_00_09_map_bin, a01_01_09_map_bin, a01_02_09_map_bin, a01_03_09_map_bin, a01_04_09_map_bin, a01_05_09_map_bin, a01_06_09_map_bin, a01_07_09_map_bin, a01_08_09_map_bin, a01_09_09_map_bin, a01_10_09_map_bin, a01_11_09_map_bin, a01_12_09_map_bin, a01_13_09_map_bin, a01_14_09_map_bin, a01_15_09_map_bin,
    a01_00_10_map_bin, a01_01_10_map_bin, a01_02_10_map_bin, a01_03_10_map_bin, a01_04_10_map_bin, a01_05_10_map_bin, a01_06_10_map_bin, a01_07_10_map_bin, a01_08_10_map_bin, a01_09_10_map_bin, a01_10_10_map_bin, a01_11_10_map_bin, a01_12_10_map_bin, a01_13_10_map_bin, a01_14_10_map_bin, a01_15_10_map_bin,
    a01_00_11_map_bin, a01_01_11_map_bin, a01_02_11_map_bin, a01_03_11_map_bin, a01_04_11_map_bin, a01_05_11_map_bin, a01_06_11_map_bin, a01_07_11_map_bin, a01_08_11_map_bin, a01_09_11_map_bin, a01_10_11_map_bin, a01_11_11_map_bin, a01_12_11_map_bin, a01_13_11_map_bin, a01_14_11_map_bin, a01_15_11_map_bin,
    a01_00_12_map_bin, a01_01_12_map_bin, a01_02_12_map_bin, a01_03_12_map_bin, a01_04_12_map_bin, a01_05_12_map_bin, a01_06_12_map_bin, a01_07_12_map_bin, a01_08_12_map_bin, a01_09_12_map_bin, a01_10_12_map_bin, a01_11_12_map_bin, a01_12_12_map_bin, a01_13_12_map_bin, a01_14_12_map_bin, a01_15_12_map_bin,
    a01_00_13_map_bin, a01_01_13_map_bin, a01_02_13_map_bin, a01_03_13_map_bin, a01_04_13_map_bin, a01_05_13_map_bin, a01_06_13_map_bin, a01_07_13_map_bin, a01_08_13_map_bin, a01_09_13_map_bin, a01_10_13_map_bin, a01_11_13_map_bin, a01_12_13_map_bin, a01_13_13_map_bin, a01_14_13_map_bin, a01_15_13_map_bin,
    a01_00_14_map_bin, a01_01_14_map_bin, a01_02_14_map_bin, a01_03_14_map_bin, a01_04_14_map_bin, a01_05_14_map_bin, a01_06_14_map_bin, a01_07_14_map_bin, a01_08_14_map_bin, a01_09_14_map_bin, a01_10_14_map_bin, a01_11_14_map_bin, a01_12_14_map_bin, a01_13_14_map_bin, a01_14_14_map_bin, a01_15_14_map_bin,
    a01_00_15_map_bin, a01_01_15_map_bin, a01_02_15_map_bin, a01_03_15_map_bin, a01_04_15_map_bin, a01_05_15_map_bin, a01_06_15_map_bin, a01_07_15_map_bin, a01_08_15_map_bin, a01_09_15_map_bin, a01_10_15_map_bin, a01_11_15_map_bin, a01_12_15_map_bin, a01_13_15_map_bin, a01_14_15_map_bin, a01_15_15_map_bin
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// General /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Clears VRAM
void ClearVRAM(void) {
    SMS_displayOff();
    SMS_VRAMmemset(XYtoADDR(0, 0), 0, 16384);
    SMS_initSprites();
    SMS_copySpritestoSAT();
    SMS_displayOn();
}

// Resets commonly used variables for program flow
void ResetGlobalVariables(void) {
    SMS_setBGScrollX(0);
    SMS_setBGScrollY(0);
    SubState = 0;
    SubState2 = 0;
	Timer = 0;
    Timer2 = 0;
    FrameCounter = 0;
}

// Sets game state, resets global variables
void SetGameState(unsigned short game_state) {
    SMS_disableLineInterrupt();
    GameState = game_state;
    ResetGlobalVariables();
    ClearVRAM();
}

// Increments timer to the given threshold, then resets it
// Returns true, if timer hit threshold
bool IncrementTimer(unsigned short threshold) {
    Timer++;
    if (Timer >= threshold) {
        Timer = 0;
        return true;
    }
    return false;
}

// Increments timer to the given threshold, then resets it
// Returns true, if timer hit threshold
bool IncrementTimer2(unsigned short threshold) {
    Timer2++;
    if (Timer2 >= threshold) {
        Timer2 = 0;
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Graphics ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Updates map palette based on map type
void UpdatePalette(void) {
    switch (GameState) {
        case GAME_STATE_AREA_01:
            if (MapType == MAP_TYPE_A1_BASE) {
                return;
            } else if (MapType == MAP_TYPE_A1_BEACH || MapType == MAP_TYPE_A1_DESERT) {
                SMS_mapROMBank(a01_overworld_02_bg_pal_bin_bank);
                GG_loadBGPalette(a01_overworld_02_bg_pal_bin);
            } else if (MapType == MAP_TYPE_A1_CEMETARY || MapType == MAP_TYPE_A1_SHOPS) {
                SMS_mapROMBank(a01_overworld_04_bg_pal_bin_bank);
                GG_loadBGPalette(a01_overworld_04_bg_pal_bin);
            } else {
                // Purple village building maps
                if (MapId == 32 || MapId == 48 || MapId == 49 || MapId == 64 || MapId == 129 || MapId == 179) {
                    SMS_mapROMBank(a01_overworld_03_bg_pal_bin_bank);
                    GG_loadBGPalette(a01_overworld_03_bg_pal_bin);
                } else {
                    SMS_mapROMBank(a01_overworld_01_bg_pal_bin_bank);
                    GG_loadBGPalette(a01_overworld_01_bg_pal_bin);
                }
            }
        break;
        case GAME_STATE_AREA_02:
            switch (MapType) {
                case MAP_TYPE_A2_TAIL_CAVE_01:
                    SMS_mapROMBank(a02_tail_cave_01_bg_pal_bin_bank);
                    GG_loadBGPalette(a02_tail_cave_01_bg_pal_bin);
                break;
                case MAP_TYPE_A2_TAIL_CAVE_02:
                    SMS_mapROMBank(a02_tail_cave_02_bg_pal_bin_bank);
                    GG_loadBGPalette(a02_tail_cave_02_bg_pal_bin);
                break;
                case MAP_TYPE_A2_BOTTLE_GROTTO_01:
                    SMS_mapROMBank(a02_bottle_grotto_01_bg_pal_bin_bank);
                    GG_loadBGPalette(a02_bottle_grotto_01_bg_pal_bin);
                break;
                case MAP_TYPE_A2_BOTTLE_GROTTO_02:
                    SMS_mapROMBank(a02_bottle_grotto_02_bg_pal_bin_bank);
                    GG_loadBGPalette(a02_bottle_grotto_02_bg_pal_bin);
                break;
                case MAP_TYPE_A2_KEY_CAVERN_01:
                    SMS_mapROMBank(a02_key_cavern_01_bg_pal_bin_bank);
                    GG_loadBGPalette(a02_key_cavern_01_bg_pal_bin);
                break;
                case MAP_TYPE_A2_ANGLERS_TUNNEL_01:
                    SMS_mapROMBank(a02_anglers_tunnel_01_bg_pal_bin_bank);
                    GG_loadBGPalette(a02_anglers_tunnel_01_bg_pal_bin);
                break;
                case MAP_TYPE_A2_CATFISHS_MAW_01:
                    SMS_mapROMBank(a02_catfishs_maw_01_bg_pal_bin_bank);
                    GG_loadBGPalette(a02_catfishs_maw_01_bg_pal_bin);
                break;
                case MAP_TYPE_A2_FACE_SHRINE_01:
                    SMS_mapROMBank(a02_face_shrine_01_bg_pal_bin_bank);
                    GG_loadBGPalette(a02_face_shrine_01_bg_pal_bin);
                break;
                case MAP_TYPE_A2_CAVES_01:
                    SMS_mapROMBank(a02_caves_01_bg_pal_bin_bank);
                    GG_loadBGPalette(a02_caves_01_bg_pal_bin);
                break;
            }
        break;
        case GAME_STATE_AREA_03:
            if (MapType == MAP_TYPE_A2_TAIL_CAVE_01) {
                SMS_mapROMBank(a01_overworld_01_bg_pal_bin_bank);
                GG_loadBGPalette(a01_overworld_01_bg_pal_bin);
            }
        break;
    }
}

// Update game graphics
void UpdateGameStateGraphics(void) {
    switch (GameState) {
        case GAME_STATE_AREA_01:
            SMS_mapROMBank(a01_map_types_bin_bank);
            MapType = a01_map_types_bin[MapId];
            UpdatePalette();
            if (MapType != PrevMapType) {
                SubStateMax = 3;
                TimerMax = 10;
                PrevMapType = MapType;
                SMS_mapROMBank(a01_base_tiles_bin_bank);
                SMS_loadTiles(a01_base_tiles_bin, 256, 3584);
                switch (MapType) {
                    case MAP_TYPE_A1_ANGLERS_TUNNEL:
                        SMS_mapROMBank(a01_anglers_tunnel_tiles_bin_bank);
                        SMS_loadTiles(a01_anglers_tunnel_tiles_bin, 368, 1536);
                    break;
                    case MAP_TYPE_A1_ANIMAL_VILLAGE:
                        SMS_mapROMBank(a01_animal_village_tiles_bin_bank);
                        SMS_loadTiles(a01_animal_village_tiles_bin, 368, 1536);
                    break;
                    case MAP_TYPE_A1_BEACH:
                        SubState = 0;
                        SubStateMax = 8;
                        SMS_mapROMBank(a01_beach_tiles_bin_bank);
                        SMS_loadTiles(a01_beach_tiles_bin, 368, 1536);
                    break;
                    case MAP_TYPE_A1_BOTTLE_GROTTO:
                        SMS_mapROMBank(a01_bottle_grotto_tiles_bin_bank);
                        SMS_loadTiles(a01_bottle_grotto_tiles_bin, 368, 1536);
                    break;
                    case MAP_TYPE_A1_CATFISH_MAW:
                        SMS_mapROMBank(a01_catfishs_maw_tiles_bin_bank);
                        SMS_loadTiles(a01_catfishs_maw_tiles_bin, 368, 1536);
                    break;
                    case MAP_TYPE_A1_CEMETARY:
                        SMS_mapROMBank(a01_cemetary_tiles_bin_bank);
                        SMS_loadTiles(a01_cemetary_tiles_bin, 368, 1536);
                    break;
                    case MAP_TYPE_A1_DESERT:
                        SMS_mapROMBank(a01_desert_tiles_bin_bank);
                        SMS_loadTiles(a01_desert_tiles_bin, 368, 1536);
                    break;
                    case MAP_TYPE_A1_EAGLES_TOWER:
                        SMS_mapROMBank(a01_eagles_tower_tiles_bin_bank);
                        SMS_loadTiles(a01_eagles_tower_tiles_bin, 368, 1536);
                    break;
                    case MAP_TYPE_A1_EGG:
                        SMS_mapROMBank(a01_egg_tiles_bin_bank);
                        SMS_loadTiles(a01_egg_tiles_bin, 368, 1536);
                    break;
                    case MAP_TYPE_A1_FOREST:
                        SMS_mapROMBank(a01_forest_tiles_bin_bank);
                        SMS_loadTiles(a01_forest_tiles_bin, 368, 1536);
                    break;
                    case MAP_TYPE_A1_MABE_VILLAGE:
                        SMS_mapROMBank(a01_mabe_village_tiles_bin_bank);
                        SMS_loadTiles(a01_mabe_village_tiles_bin, 368, 1536);
                    break;
                    case MAP_TYPE_A1_MANSION:
                        SMS_mapROMBank(a01_mansion_tiles_bin_bank);
                        SMS_loadTiles(a01_mansion_tiles_bin, 368, 1536);
                    break;
                    case MAP_TYPE_A1_SHOPS:
                        SMS_mapROMBank(a01_shops_tiles_bin_bank);
                        SMS_loadTiles(a01_shops_tiles_bin, 368, 1536);
                    break;
                    case MAP_TYPE_A1_SWAMP:
                        SMS_mapROMBank(a01_swamp_tiles_bin_bank);
                        SMS_loadTiles(a01_swamp_tiles_bin, 368, 1536);
                    break;
                    case MAP_TYPE_A1_TAIL_CAVE:
                        SMS_mapROMBank(a01_tail_cave_tiles_bin_bank);
                        SMS_loadTiles(a01_tail_cave_tiles_bin, 368, 1536);
                    break;
                    case MAP_TYPE_A1_TURTLE_ROCK:
                        SMS_mapROMBank(a01_turtle_rock_tiles_bin_bank);
                        SMS_loadTiles(a01_turtle_rock_tiles_bin, 368, 1536);
                    break;
                }
            }
        break;
        case GAME_STATE_AREA_02:
            SMS_mapROMBank(a02_map_types_bin_bank);
            MapType = a02_map_types_bin[MapId];
            UpdatePalette();
            if (MapType != PrevMapType) {
                SubStateMax = 3;
                TimerMax = 10;
                SubStateMax2 = 8;
                TimerMax2 = 3;
                PrevMapType = MapType;
                switch (MapType) {
                    case MAP_TYPE_A2_TAIL_CAVE_01:
                        SMS_mapROMBank(a02_tail_cave_01_tiles_bin_bank);
                        SMS_loadTiles(a02_tail_cave_01_tiles_bin, 256, a02_tail_cave_01_tiles_bin_size);
                    break;
                    case MAP_TYPE_A2_TAIL_CAVE_02:
                        SMS_mapROMBank(a02_tail_cave_02_tiles_bin_bank);
                        SMS_loadTiles(a02_tail_cave_02_tiles_bin, 256, a02_tail_cave_02_tiles_bin_size);
                    break;
                    case MAP_TYPE_A2_BOTTLE_GROTTO_01:
                        SMS_mapROMBank(a02_bottle_grotto_01_tiles_bin_bank);
                        SMS_loadTiles(a02_bottle_grotto_01_tiles_bin, 256, a02_bottle_grotto_01_tiles_bin_size);
                    break;
                    case MAP_TYPE_A2_BOTTLE_GROTTO_02:
                        SMS_mapROMBank(a02_bottle_grotto_02_tiles_bin_bank);
                        SMS_loadTiles(a02_bottle_grotto_02_tiles_bin, 256, a02_bottle_grotto_02_tiles_bin_size);
                    break;
                    case MAP_TYPE_A2_KEY_CAVERN_01:
                        SMS_mapROMBank(a02_key_cavern_01_tiles_bin_bank);
                        SMS_loadTiles(a02_key_cavern_01_tiles_bin, 256, a02_key_cavern_01_tiles_bin_size);
                    break;
                    case MAP_TYPE_A2_ANGLERS_TUNNEL_01:
                        TimerMax2 = 15;
                        SubStateMax2 = 3;
                        SMS_mapROMBank(a02_anglers_tunnel_01_tiles_bin_bank);
                        SMS_loadTiles(a02_anglers_tunnel_01_tiles_bin, 256, a02_anglers_tunnel_01_tiles_bin_size);
                    break;
                    case MAP_TYPE_A2_CATFISHS_MAW_01:
                        TimerMax2 = 15;
                        SubStateMax2 = 3;
                        SMS_mapROMBank(a02_catfishs_maw_01_tiles_bin_bank);
                        SMS_loadTiles(a02_catfishs_maw_01_tiles_bin, 256, a02_catfishs_maw_01_tiles_bin_size);
                    break;
                    case MAP_TYPE_A2_FACE_SHRINE_01:
                        SMS_mapROMBank(a02_face_shrine_01_tiles_bin_bank);
                        SMS_loadTiles(a02_face_shrine_01_tiles_bin, 256, a02_face_shrine_01_tiles_bin_size);
                    break;
                    case MAP_TYPE_A2_CAVES_01:
                        SMS_mapROMBank(a02_caves_01_tiles_bin_bank);
                        SMS_loadTiles(a02_caves_01_tiles_bin, 256, a02_caves_01_tiles_bin_size);
                    break;
                }
            }
        break;
        case GAME_STATE_AREA_03:
            SMS_mapROMBank(a02_map_types_bin_bank);
            MapType = a02_map_types_bin[MapId];
            UpdatePalette();
            if (MapType != PrevMapType) {
                SubStateMax = 3;
                TimerMax = 10;
                PrevMapType = MapType;
                switch (MapType) {
                    case MAP_TYPE_A2_TAIL_CAVE_01:
                        SMS_mapROMBank(a02_tail_cave_01_tiles_bin_bank);
                        SMS_loadTiles(a02_tail_cave_01_tiles_bin, 256, a02_tail_cave_01_tiles_bin_size);
                    break;
                }
            }
        break;
    }
}

// Sets current area data
void SetArea(unsigned char mapId) {
    switch (GameState) {
        case GAME_STATE_AREA_01:
            AreaBank = Area01MapBanks[mapId];
            AreaBin = Area01MapBins[mapId];
        break;
        case GAME_STATE_AREA_02:
            AreaBank = Area02MapBanks[mapId];
            AreaBin = Area02MapBins[mapId];
        break;
        case GAME_STATE_AREA_03:
            AreaBank = Area03MapBanks[mapId];
            AreaBin = Area03MapBins[mapId];
        break;
    }
}

// Sets the tilemap indexes vertically orientated (Used for horizontal scrolling only)
void SetVerticalMap(void) {
    unsigned short index = 0;
    unsigned short tile = 0;
    unsigned char map_01;
    unsigned char map_02;

    // Set orientation of previous and current map, based on scroll direction
    if (ScrollDir == DIRECTION_RIGHT) {
        map_01 = PrevMapId;
        map_02 = MapId;
    } else {
        map_01 = MapId;
        map_02 = PrevMapId;
    }

    // Fill map array with vertically aligned tiles
    SetArea(map_01);
    SMS_mapROMBank(AreaBank);
    for (unsigned char col = 0; col < 20; col++) {
        for (unsigned char row = 0; row < 16; row++) {
            tile = (40 * row) + (col << 1);
            Map[index++] = AreaBin[tile];
            Map[index++] = AreaBin[tile + 1];
        }
    }
    SetArea(map_02);
    SMS_mapROMBank(AreaBank);
    for (unsigned char col = 0; col < 20; col++) {
        for (unsigned char row = 0; row < 16; row++) {
            tile = (40 * row) + (col << 1);
            Map[index++] = AreaBin[tile];
            Map[index++] = AreaBin[tile + 1];
        }
    }
    // Set back to current map id
    SetArea(MapId);
}

// Loads graphics for current game state
void LoadGameStateGraphics(void) {
    switch (GameState) {
        case GAME_STATE_INVENTORY:
            SMS_mapROMBank(inventory_bg_pal_bin_bank);
            GG_loadBGPalette(inventory_bg_pal_bin);
            SMS_mapROMBank(inventory_spr_pal_bin_bank);
            GG_loadSpritePalette(inventory_spr_pal_bin);
            SMS_mapROMBank(inventory_tiles_bin_bank);
            SMS_loadTiles(inventory_tiles_bin, 0, inventory_tiles_bin_size);
            SMS_mapROMBank(inventory_map_bin_bank);
            SMS_loadTileMapArea(6, 3, inventory_map_bin, 20, 18);
        break;
        case GAME_STATE_AREA_01:
            UpdateGameStateGraphics();
            SMS_mapROMBank(main_spr_pal_bin_bank);
            GG_loadSpritePalette(main_spr_pal_bin);
            SMS_mapROMBank(Area01MapBanks[MapId]);
            SMS_loadTileMapArea(6, 3, Area01MapBins[MapId], 20, 16);
            SMS_mapROMBank(hud_tiles_bin_bank);
            SMS_loadTiles(hud_tiles_bin, 0, hud_tiles_bin_size);
            SMS_mapROMBank(hud_map_bin_bank);
            SMS_loadTileMapArea(6, 19, hud_map_bin, 20, 2);
        break;
        case GAME_STATE_AREA_02:
            UpdateGameStateGraphics();
            SMS_mapROMBank(main_spr_pal_bin_bank);
            GG_loadSpritePalette(main_spr_pal_bin);
            SMS_mapROMBank(Area02MapBanks[MapId]);
            SMS_loadTileMapArea(6, 3, Area02MapBins[MapId], 20, 16);
            SMS_mapROMBank(hud_tiles_bin_bank);
            SMS_loadTiles(hud_tiles_bin, 0, hud_tiles_bin_size);
            SMS_mapROMBank(hud_map_bin_bank);
            SMS_loadTileMapArea(6, 19, hud_map_bin, 20, 2);
        break;
        case GAME_STATE_AREA_03:
            UpdateGameStateGraphics();
            SMS_mapROMBank(main_spr_pal_bin_bank);
            GG_loadSpritePalette(main_spr_pal_bin);
            SMS_mapROMBank(Area03MapBanks[MapId]);
            SMS_loadTileMapArea(6, 3, Area03MapBins[MapId], 20, 16);
            SMS_mapROMBank(hud_tiles_bin_bank);
            SMS_loadTiles(hud_tiles_bin, 0, hud_tiles_bin_size);
            SMS_mapROMBank(hud_map_bin_bank);
            SMS_loadTileMapArea(6, 19, hud_map_bin, 20, 2);
        break;
    }
}

// Updates game state environmental animations
void UpdateEnvironmentAnimations(void) {
    if (ScrollIndex != 0)
        return;

    if (SubState >= SubStateMax)
        SubState = 0;

    if (SubState2 >= SubStateMax2)
        SubState2 = 0;

    switch (GameState) {
        case GAME_STATE_AREA_01:
            if (IncrementTimer(TimerMax) == false)
                return;

            switch (MapType) {
                case MAP_TYPE_A1_BASE:
                    SMS_mapROMBank(animation_flower_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy32(8192, &animation_flower_tiles_bin[SubState << 5]);
                    SMS_mapROMBank(animation_gem_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy128(8256, &animation_gem_tiles_bin[SubState << 7]);
                    SubState++;
                break;
                case MAP_TYPE_A1_BOTTLE_GROTTO:
                    SMS_mapROMBank(animation_forest_flower_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy32(13248, &animation_forest_flower_tiles_bin[SubState << 5]);
                    SubState++;
                break;
                case MAP_TYPE_A1_BEACH:
                    SMS_mapROMBank(animation_shore_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy128(13184, &animation_shore_tiles_bin[SubState << 7]);
                    SubState++;
                break;
                case MAP_TYPE_A1_FOREST:
                    SMS_mapROMBank(animation_flower_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy32(8192, &animation_flower_tiles_bin[SubState << 5]);
                    SMS_mapROMBank(animation_forest_flower_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy32(13248, &animation_forest_flower_tiles_bin[SubState << 5]);
                    SubState++;
                break;
                case MAP_TYPE_A1_MABE_VILLAGE:
                    SMS_mapROMBank(animation_flower_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy32(8192, &animation_flower_tiles_bin[SubState << 5]);
                    SMS_mapROMBank(animation_rooster_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy64(11776, &animation_rooster_tiles_bin[SubState << 6]);
                    SubState++;
                break;
                case MAP_TYPE_A1_SWAMP:
                    SMS_mapROMBank(animation_swamp_flower_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy32(13248, &animation_swamp_flower_tiles_bin[SubState << 5]);
                    SubState++;
                break;
                case MAP_TYPE_A1_TURTLE_ROCK:
                    SMS_mapROMBank(animation_gem_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy128(8256, &animation_gem_tiles_bin[SubState << 7]);
                    SubState++;
                break;
            }
        break;
        case GAME_STATE_AREA_02:
            switch (MapType) {
                case MAP_TYPE_A2_ANGLERS_TUNNEL_01:
                    if (IncrementTimer(TimerMax) == true) {
                        SMS_mapROMBank(animation_torches_01_tiles_bin_bank);
                        UNSAFE_SMS_VRAMmemcpy64(8224, &animation_torches_01_tiles_bin[SubState << 7]);
                        SMS_mapROMBank(animation_torches_01_tiles_bin_bank);
                        UNSAFE_SMS_VRAMmemcpy32(8288, &animation_torches_01_tiles_bin[(SubState << 7) + 64]);
                        SubState++;
                    }
                    if (IncrementTimer2(TimerMax2) == true) {
                        SMS_mapROMBank(animation_torches_01_tiles_bin_bank);
                        UNSAFE_SMS_VRAMmemcpy32(8320, &animation_torches_01_tiles_bin[(SubState2 << 7) + 96]);
                        SMS_mapROMBank(animation_water_01_tiles_bin_bank);
                        UNSAFE_SMS_VRAMmemcpy128(9280, &animation_water_01_tiles_bin[SubState2 << 7]);
                        SubState2++;
                    }
                break;
                case MAP_TYPE_A2_KEY_CAVERN_01:
                    if (IncrementTimer2(TimerMax2) == true) {
                        SMS_mapROMBank(animation_belt_01_tiles_bin_bank);
                        UNSAFE_SMS_VRAMmemcpy64(9408, &animation_belt_01_tiles_bin[SubState2 << 6]);
                        SubState2++;
                    }
                    if (IncrementTimer(TimerMax) == true) {
                        SMS_mapROMBank(animation_torches_01_tiles_bin_bank);
                        UNSAFE_SMS_VRAMmemcpy128(8224, &animation_torches_01_tiles_bin[SubState << 7]);
                        SubState++;
                    }
                break;
                case MAP_TYPE_A2_CATFISHS_MAW_01:
                    if (IncrementTimer2(TimerMax2) == true) {
                        SMS_mapROMBank(animation_water_02_tiles_bin_bank);
                        UNSAFE_SMS_VRAMmemcpy64(9280, &animation_water_02_tiles_bin[SubState2 << 6]);
                        SubState2++;
                    }
                    if (IncrementTimer(TimerMax) == true) {
                        SMS_mapROMBank(animation_torches_01_tiles_bin_bank);
                        UNSAFE_SMS_VRAMmemcpy128(8224, &animation_torches_01_tiles_bin[SubState << 7]);
                        SubState++;
                    }
                break;
                case MAP_TYPE_A2_FACE_SHRINE_01:
                    if (IncrementTimer2(TimerMax2) == true) {
                        SMS_mapROMBank(animation_belt_02_tiles_bin_bank);
                        UNSAFE_SMS_VRAMmemcpy64(9280, &animation_belt_02_tiles_bin[SubState2 << 6]);
                        SubState2++;
                    }
                    if (IncrementTimer(TimerMax) == true) {
                        SMS_mapROMBank(animation_torches_02_tiles_bin_bank);
                        UNSAFE_SMS_VRAMmemcpy128(8224, &animation_torches_02_tiles_bin[SubState << 7]);
                        SubState++;
                    }
                break;
                case MAP_TYPE_A2_CAVES_01:
                    if (IncrementTimer(TimerMax) == true) {
                        SMS_mapROMBank(animation_torches_03_tiles_bin_bank);
                        UNSAFE_SMS_VRAMmemcpy128(8224, &animation_torches_03_tiles_bin[SubState << 7]);
                        SubState++;
                    }
                break;
                case MAP_TYPE_A2_BOTTLE_GROTTO_01:
                case MAP_TYPE_A2_TAIL_CAVE_01:
                    if (IncrementTimer(TimerMax) == true) {
                        SMS_mapROMBank(animation_torches_01_tiles_bin_bank);
                        UNSAFE_SMS_VRAMmemcpy128(8224, &animation_torches_01_tiles_bin[SubState << 7]);
                        SubState++;
                    }
                break;
            }
        break;
        case GAME_STATE_AREA_03:
            switch (MapType) {
                case MAP_TYPE_A2_TAIL_CAVE_01:
                    SMS_mapROMBank(animation_flower_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy32(8384, &animation_flower_tiles_bin[SubState << 5]);
                    SubState++;
                break;
            }
        break;
    }
}

// Updates a map columns or rows based on scroll direction
void UpdateMapScroll(void) {
    switch (ScrollDir) {
        case DIRECTION_RIGHT:
            ScrollIndex = ScrollIndex + 1;
            for (unsigned char i = 0; i < 20; i++) {
                SMS_loadTileMapColumn(i + 6, 3, &Map[(ScrollIndex + i - 6) << 5], 16);
            }
            if (ScrollIndex == 26) {
                ScrollIndex = 0;
            }
        break;
        case DIRECTION_UP:
            ScrollIndex = ScrollIndex + 1;
            SMS_mapROMBank(AreaBank);
            SMS_loadTileMapArea(6, 3, &AreaBin[640 - (40 * (ScrollIndex - 3))], 20, ScrollIndex - 3);
            if (19 - ScrollIndex != 0) {
                SMS_loadTileMapArea(6, ScrollIndex, Map, 20, 19 - ScrollIndex);
            }
            if (ScrollIndex == 19) {
                ScrollIndex = 0;
            }
        break;
        case DIRECTION_LEFT:
            ScrollIndex = ScrollIndex - 1;
            for (unsigned char i = 0; i < 20; i++) {
                SMS_loadTileMapColumn(i + 6, 3, &Map[(ScrollIndex + i) << 5], 16);
            }
            if (ScrollIndex == 26) {
                ScrollIndex = 0;
            }
        break;
        case DIRECTION_DOWN:
            ScrollIndex = ScrollIndex - 1;
            SMS_mapROMBank(AreaBank);
            SMS_loadTileMapArea(6, ScrollIndex, AreaBin, 20, 19 - ScrollIndex);
            if (ScrollIndex - 3 != 0) {
                SMS_loadTileMapArea(6, 3, &Map[640 - (40 * (ScrollIndex - 3))], 20, ScrollIndex - 3);
            }
            if (ScrollIndex == 3) {
                ScrollIndex = 0;
            }
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Polls current input state
void PollInput(void) {
	KeysPressed = SMS_getKeysPressed();
    KeysHeld = SMS_getKeysHeld();
}

// If left and right keys are being held together
bool IsHoldingLeftAndRight(void) {
	return KeysHeld & PORT_A_KEY_LEFT && KeysHeld & PORT_A_KEY_RIGHT;
}

// If up and down keys are being held together
bool IsHoldingUpAndDown(void) {
	return KeysHeld & PORT_A_KEY_DOWN && KeysHeld & PORT_A_KEY_UP;
}

// // Handles player input
// void HandlePlayerInput(void) {
// }

// Checks if player 1 start button is pressed, and sets the given next state, returns if key pressed
bool PollNextState(unsigned char game_state) {
    if (KeysPressed & PORT_A_KEY_1) {
        SetGameState(game_state);
        return true;
    }
    return false;
}

// Test map layout using directional keys
void TestScroll(void) {
    if (ScrollIndex != 0) {
        UpdateMapScroll();
        return;
    }

    if (KeysPressed == PORT_A_KEY_RIGHT) {
        ScrollDir = DIRECTION_RIGHT;
        ScrollIndex = 6;
        PrevMapId = MapId;
        MapId = MapId + 1;
        SetVerticalMap();
        UpdateGameStateGraphics();
    } else if (KeysPressed == PORT_A_KEY_UP && MapId > 15) {
        ScrollDir = DIRECTION_UP;
        ScrollIndex = 3;
        MapId = MapId - 16;
        SetArea(MapId);
        SMS_saveTileMapArea(6, 3, &Map, 20, 16);
        UpdateGameStateGraphics();
    } else if (KeysPressed == PORT_A_KEY_LEFT) {
        ScrollDir = DIRECTION_LEFT;
        ScrollIndex = 20;
        PrevMapId = MapId;
        MapId = MapId - 1;
        SetVerticalMap();
        UpdateGameStateGraphics();
    } else if (KeysPressed == PORT_A_KEY_DOWN && MapId < 240) {
        ScrollDir = DIRECTION_DOWN;
        ScrollIndex = 19;
        MapId = MapId + 16;
        SetArea(MapId);
        SMS_saveTileMapArea(6, 3, &Map, 20, 16);
        UpdateGameStateGraphics();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Game State Handling /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Line IRQ Handler
void LineIRQhandler (void) {
    SMS_disableLineInterrupt();
    SMS_displayOff();
    UpdateEnvironmentAnimations();
    SMS_displayOn();
    TestScroll();
}

// Handles main game loop event logic
void HandleGameEvent(unsigned char event) {
    if (event == EVENT_VBLANK_LOAD && PrevGameState == GameState)
        return;

    switch (GameState) {
        case GAME_STATE_INVENTORY:
            switch (event) {
                case EVENT_VBLANK_LOAD:
                    ResetGlobalVariables();
                    LoadGameStateGraphics();
                break;
            }
        break;
        case GAME_STATE_AREA_01:
        case GAME_STATE_AREA_02:
        case GAME_STATE_AREA_03:
            switch (event) {
                case EVENT_VBLANK_LOAD:
                    ResetGlobalVariables();
                    LoadGameStateGraphics();
                    SMS_setLineInterruptHandler(LineIRQhandler);
					SMS_setLineCounter(168);
                break;
                case EVENT_AFTER_VBLANK:
                    SMS_enableLineInterrupt();
                break;
            }
        break;
    }
    if (event == EVENT_VBLANK_LOAD) {
        PrevGameState = GameState;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main Loop ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main(void) {
	SetGameState(GAME_STATE_AREA_01);
    MapId = 0;
    for (;;) {
        PollInput();
		HandleGameEvent(EVENT_BEFORE_VBLANK);
		SMS_waitForVBlank();
        FrameCounter++;
		HandleGameEvent(EVENT_VBLANK_LOAD);
		HandleGameEvent(EVENT_AFTER_VBLANK);
        SMS_mapROMBank(AudioCurrentBank);
		PSGFrame();
	}
}

// SMS ROM Header information
SMS_EMBED_SEGA_ROM_HEADER(9999, 1);
SMS_EMBED_SDSC_HEADER_AUTO_DATE(1, 0, "Nintendo", "Links Awakening", "Adventure");