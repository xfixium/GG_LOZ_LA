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
#define MAP_TYPE_NONE 1
#define MAP_TYPE_TURTLE_ROCK 2
#define MAP_TYPE_SWAMP 3
#define MAP_TYPE_VILLAGE 4
#define MAP_TYPE_FOREST 5
#define MAP_TYPE_CEMETARY 6
#define MAP_TYPE_TAIL_CAVE 7
#define MAP_TYPE_BOTTLE_GROTTO 8
#define MAP_TYPE_BEACH 9

// Game states
#define GAME_STATE_TITLE 1
#define GAME_STATE_INVENTORY 2
#define GAME_STATE_OVERWORLD 3
#define GAME_STATE_DUNGEON 4
#define GAME_STATE_INSIDE 5

// Global variables
unsigned char Map[1280];         // Current and previous map data
unsigned char ScrollIndex;       // Map scroll index
unsigned char ScrollDir;         // Map scroll direction
unsigned char MapId;             // Current map id
unsigned char PrevMapId;         // Previous map id
unsigned char MapType;           // Current map type
unsigned char PrevMapType;       // Previous map type
unsigned char GameState;         // Overall program flow
unsigned char PrevGameState;     // Previous loaded state
unsigned char SubState;          // A game state's sub state, for managed flow of a game state
unsigned char SubStateMax;       // Substate threshold
unsigned char FrameCounter;      // Frame counter, used for various things
unsigned short Timer;            // General timer
unsigned short TimerMax;         // Timer threshold
unsigned char AudioCurrentBank;  // Current audio bank
unsigned int KeysPressed;        // Keys pressed
unsigned int KeysHeld;           // Keys held down

// Overworld map bin banks (Temp)
unsigned char OverworldMapBanks[256] = {
    overworld_00_00_map_bin_bank, overworld_01_00_map_bin_bank, overworld_02_00_map_bin_bank, overworld_03_00_map_bin_bank, overworld_04_00_map_bin_bank, overworld_05_00_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_01_map_bin_bank, overworld_01_01_map_bin_bank, overworld_02_01_map_bin_bank, overworld_03_01_map_bin_bank, overworld_04_01_map_bin_bank, overworld_05_01_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_02_map_bin_bank, overworld_01_02_map_bin_bank, overworld_02_02_map_bin_bank, overworld_03_02_map_bin_bank, overworld_04_02_map_bin_bank, overworld_05_02_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_03_map_bin_bank, overworld_01_03_map_bin_bank, overworld_02_03_map_bin_bank, overworld_03_03_map_bin_bank, overworld_04_03_map_bin_bank, overworld_05_03_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_04_map_bin_bank, overworld_01_04_map_bin_bank, overworld_02_04_map_bin_bank, overworld_03_04_map_bin_bank, overworld_04_04_map_bin_bank, overworld_05_04_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_05_map_bin_bank, overworld_01_05_map_bin_bank, overworld_02_05_map_bin_bank, overworld_03_05_map_bin_bank, overworld_04_05_map_bin_bank, overworld_05_05_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_06_map_bin_bank, overworld_01_06_map_bin_bank, overworld_02_06_map_bin_bank, overworld_03_06_map_bin_bank, overworld_04_06_map_bin_bank, overworld_05_06_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_07_map_bin_bank, overworld_01_07_map_bin_bank, overworld_02_07_map_bin_bank, overworld_03_07_map_bin_bank, overworld_04_07_map_bin_bank, overworld_05_07_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_08_map_bin_bank, overworld_01_08_map_bin_bank, overworld_02_08_map_bin_bank, overworld_03_08_map_bin_bank, overworld_04_08_map_bin_bank, overworld_05_08_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_09_map_bin_bank, overworld_01_09_map_bin_bank, overworld_02_09_map_bin_bank, overworld_03_09_map_bin_bank, overworld_04_09_map_bin_bank, overworld_05_09_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_10_map_bin_bank, overworld_01_10_map_bin_bank, overworld_02_10_map_bin_bank, overworld_03_10_map_bin_bank, overworld_04_10_map_bin_bank, overworld_05_10_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_11_map_bin_bank, overworld_01_11_map_bin_bank, overworld_02_11_map_bin_bank, overworld_03_11_map_bin_bank, overworld_04_11_map_bin_bank, overworld_05_11_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_12_map_bin_bank, overworld_01_12_map_bin_bank, overworld_02_12_map_bin_bank, overworld_03_12_map_bin_bank, overworld_04_12_map_bin_bank, overworld_05_12_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_13_map_bin_bank, overworld_01_13_map_bin_bank, overworld_02_13_map_bin_bank, overworld_03_13_map_bin_bank, overworld_04_13_map_bin_bank, overworld_05_13_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_14_map_bin_bank, overworld_01_14_map_bin_bank, overworld_02_14_map_bin_bank, overworld_03_14_map_bin_bank, overworld_04_14_map_bin_bank, overworld_05_14_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_15_map_bin_bank, overworld_01_15_map_bin_bank, overworld_02_15_map_bin_bank, overworld_03_15_map_bin_bank, overworld_04_15_map_bin_bank, overworld_05_15_map_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Overworld map bin pointers (Temp)
const unsigned char* OverworldMapBins[256] = {
    overworld_00_00_map_bin, overworld_01_00_map_bin, overworld_02_00_map_bin, overworld_03_00_map_bin, overworld_04_00_map_bin, overworld_05_00_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_01_map_bin, overworld_01_01_map_bin, overworld_02_01_map_bin, overworld_03_01_map_bin, overworld_04_01_map_bin, overworld_05_01_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_02_map_bin, overworld_01_02_map_bin, overworld_02_02_map_bin, overworld_03_02_map_bin, overworld_04_02_map_bin, overworld_05_02_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_03_map_bin, overworld_01_03_map_bin, overworld_02_03_map_bin, overworld_03_03_map_bin, overworld_04_03_map_bin, overworld_05_03_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_04_map_bin, overworld_01_04_map_bin, overworld_02_04_map_bin, overworld_03_04_map_bin, overworld_04_04_map_bin, overworld_05_04_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_05_map_bin, overworld_01_05_map_bin, overworld_02_05_map_bin, overworld_03_05_map_bin, overworld_04_05_map_bin, overworld_05_05_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_06_map_bin, overworld_01_06_map_bin, overworld_02_06_map_bin, overworld_03_06_map_bin, overworld_04_06_map_bin, overworld_05_06_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_07_map_bin, overworld_01_07_map_bin, overworld_02_07_map_bin, overworld_03_07_map_bin, overworld_04_07_map_bin, overworld_05_07_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_08_map_bin, overworld_01_08_map_bin, overworld_02_08_map_bin, overworld_03_08_map_bin, overworld_04_08_map_bin, overworld_05_08_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_09_map_bin, overworld_01_09_map_bin, overworld_02_09_map_bin, overworld_03_09_map_bin, overworld_04_09_map_bin, overworld_05_09_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_10_map_bin, overworld_01_10_map_bin, overworld_02_10_map_bin, overworld_03_10_map_bin, overworld_04_10_map_bin, overworld_05_10_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_11_map_bin, overworld_01_11_map_bin, overworld_02_11_map_bin, overworld_03_11_map_bin, overworld_04_11_map_bin, overworld_05_11_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_12_map_bin, overworld_01_12_map_bin, overworld_02_12_map_bin, overworld_03_12_map_bin, overworld_04_12_map_bin, overworld_05_12_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_13_map_bin, overworld_01_13_map_bin, overworld_02_13_map_bin, overworld_03_13_map_bin, overworld_04_13_map_bin, overworld_05_13_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_14_map_bin, overworld_01_14_map_bin, overworld_02_14_map_bin, overworld_03_14_map_bin, overworld_04_14_map_bin, overworld_05_14_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_00_15_map_bin, overworld_01_15_map_bin, overworld_02_15_map_bin, overworld_03_15_map_bin, overworld_04_15_map_bin, overworld_05_15_map_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Overworld tileset bin banks (Temp)
unsigned char OverworldTilesetBanks[256] = {
    overworld_turtle_rock_tiles_bin_bank, overworld_turtle_rock_tiles_bin_bank, overworld_turtle_rock_tiles_bin_bank, overworld_turtle_rock_tiles_bin_bank, overworld_turtle_rock_tiles_bin_bank, overworld_turtle_rock_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_turtle_rock_tiles_bin_bank, overworld_turtle_rock_tiles_bin_bank, overworld_turtle_rock_tiles_bin_bank, overworld_turtle_rock_tiles_bin_bank, overworld_turtle_rock_tiles_bin_bank, overworld_turtle_rock_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_swamp_tiles_bin_bank, overworld_swamp_tiles_bin_bank, overworld_swamp_tiles_bin_bank, overworld_swamp_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_forest_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_swamp_tiles_bin_bank, overworld_swamp_tiles_bin_bank, overworld_swamp_tiles_bin_bank, overworld_swamp_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_forest_tiles_bin_bank, overworld_forest_tiles_bin_bank, overworld_forest_tiles_bin_bank, overworld_forest_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_forest_tiles_bin_bank, overworld_forest_tiles_bin_bank, overworld_forest_tiles_bin_bank, overworld_forest_tiles_bin_bank, overworld_cemetary_tiles_bin_bank, overworld_cemetary_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_forest_tiles_bin_bank, overworld_forest_tiles_bin_bank, overworld_forest_tiles_bin_bank, overworld_forest_tiles_bin_bank, overworld_cemetary_tiles_bin_bank, overworld_cemetary_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_forest_tiles_bin_bank, overworld_forest_tiles_bin_bank, overworld_forest_tiles_bin_bank, overworld_forest_tiles_bin_bank, overworld_cemetary_tiles_bin_bank, overworld_cemetary_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_forest_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_bottle_grotto_tiles_bin_bank, overworld_bottle_grotto_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_bottle_grotto_tiles_bin_bank, overworld_bottle_grotto_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_beach_tiles_bin_bank, overworld_village_tiles_bin_bank, overworld_tail_cave_tiles_bin_bank, overworld_tail_cave_tiles_bin_bank, overworld_bottle_grotto_tiles_bin_bank, overworld_bottle_grotto_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_beach_tiles_bin_bank, overworld_beach_tiles_bin_bank, overworld_beach_tiles_bin_bank, overworld_beach_tiles_bin_bank, overworld_beach_tiles_bin_bank, overworld_beach_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_beach_tiles_bin_bank, overworld_beach_tiles_bin_bank, overworld_beach_tiles_bin_bank, overworld_beach_tiles_bin_bank, overworld_beach_tiles_bin_bank, overworld_beach_tiles_bin_bank, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Overworld tileset bin pointers (Temp)
const unsigned char* OverworldTilesetBins[256] = {
    overworld_turtle_rock_tiles_bin, overworld_turtle_rock_tiles_bin, overworld_turtle_rock_tiles_bin, overworld_turtle_rock_tiles_bin, overworld_turtle_rock_tiles_bin, overworld_turtle_rock_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_turtle_rock_tiles_bin, overworld_turtle_rock_tiles_bin, overworld_turtle_rock_tiles_bin, overworld_turtle_rock_tiles_bin, overworld_turtle_rock_tiles_bin, overworld_turtle_rock_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_swamp_tiles_bin, overworld_swamp_tiles_bin, overworld_swamp_tiles_bin, overworld_swamp_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_forest_tiles_bin, overworld_village_tiles_bin, overworld_swamp_tiles_bin, overworld_swamp_tiles_bin, overworld_swamp_tiles_bin, overworld_swamp_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_forest_tiles_bin, overworld_forest_tiles_bin, overworld_forest_tiles_bin, overworld_forest_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_forest_tiles_bin, overworld_forest_tiles_bin, overworld_forest_tiles_bin, overworld_forest_tiles_bin, overworld_cemetary_tiles_bin, overworld_cemetary_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_forest_tiles_bin, overworld_forest_tiles_bin, overworld_forest_tiles_bin, overworld_forest_tiles_bin, overworld_cemetary_tiles_bin, overworld_cemetary_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_forest_tiles_bin, overworld_forest_tiles_bin, overworld_forest_tiles_bin, overworld_forest_tiles_bin, overworld_cemetary_tiles_bin, overworld_cemetary_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_forest_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_bottle_grotto_tiles_bin, overworld_bottle_grotto_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_village_tiles_bin, overworld_bottle_grotto_tiles_bin, overworld_bottle_grotto_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_beach_tiles_bin, overworld_village_tiles_bin, overworld_tail_cave_tiles_bin, overworld_tail_cave_tiles_bin, overworld_bottle_grotto_tiles_bin, overworld_bottle_grotto_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_beach_tiles_bin, overworld_beach_tiles_bin, overworld_beach_tiles_bin, overworld_beach_tiles_bin, overworld_beach_tiles_bin, overworld_beach_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    overworld_beach_tiles_bin, overworld_beach_tiles_bin, overworld_beach_tiles_bin, overworld_beach_tiles_bin, overworld_beach_tiles_bin, overworld_beach_tiles_bin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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
	FrameCounter = 0;
	Timer = 0;
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Graphics ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Updates map palette based on map type
void UpdatePalette(void) {
    switch (GameState)
    {
        case GAME_STATE_OVERWORLD:
            if (MapType == MAP_TYPE_NONE) {
                return;
            } else if (MapType == MAP_TYPE_BEACH) {
                SMS_mapROMBank(overworld_02_bg_pal_bin_bank);
                GG_loadBGPalette(overworld_02_bg_pal_bin);
            } else if (MapType == MAP_TYPE_CEMETARY) {
                SMS_mapROMBank(overworld_04_bg_pal_bin_bank);
                GG_loadBGPalette(overworld_04_bg_pal_bin);
            } else {
                // Purple village building maps
                if (MapId == 32 || MapId == 48 || MapId == 49 || MapId == 64 || MapId == 129 || MapId == 179) {
                    SMS_mapROMBank(overworld_03_bg_pal_bin_bank);
                    GG_loadBGPalette(overworld_03_bg_pal_bin);
                } else {
                    SMS_mapROMBank(overworld_01_bg_pal_bin_bank);
                    GG_loadBGPalette(overworld_01_bg_pal_bin);
                }
            }
        break;
    }
}

// Update game graphics
void UpdateGameStateGraphics(void) {
    switch (GameState) {
        case GAME_STATE_OVERWORLD:
            SMS_mapROMBank(overworld_map_types_bin_bank);
            MapType = overworld_map_types_bin[MapId];
            UpdatePalette();
            if (MapType != PrevMapType) {
                PrevMapType = MapType;
                switch (MapType) {
                    case MAP_TYPE_TURTLE_ROCK:
                        SubStateMax = 3;
                        TimerMax = 10;
                    break;
                    case MAP_TYPE_SWAMP:
                        SubStateMax = 3;
                        TimerMax = 10;
                    break;
                    case MAP_TYPE_FOREST:
                        SubStateMax = 3;
                        TimerMax = 10;
                    break;
                    case MAP_TYPE_NONE:
                    case MAP_TYPE_BOTTLE_GROTTO:
                    case MAP_TYPE_VILLAGE:
                        SubStateMax = 3;
                        TimerMax = 10;
                    break;
                    case MAP_TYPE_BEACH:
                        SubState = 0;
                        SubStateMax = 8;
                        TimerMax = 10;
                    break;
                }
                if (MapType != MAP_TYPE_NONE) {
                    SMS_mapROMBank(OverworldTilesetBanks[MapId]);
                    SMS_loadTiles(OverworldTilesetBins[MapId], 256, 5120);
                }
            }
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
    SMS_mapROMBank(OverworldMapBanks[map_01]);
    for (unsigned char col = 0; col < 20; col++) {
        for (unsigned char row = 0; row < 16; row++) {
            tile = (40 * row) + (col << 1);
            Map[index++] = OverworldMapBins[map_01][tile];
            Map[index++] = OverworldMapBins[map_01][tile + 1];
        }
    }
    SMS_mapROMBank(OverworldMapBanks[map_02]);
    for (unsigned char col = 0; col < 20; col++) {
        for (unsigned char row = 0; row < 16; row++) {
            tile = (40 * row) + (col << 1);
            Map[index++] = OverworldMapBins[map_02][tile];
            Map[index++] = OverworldMapBins[map_02][tile + 1];
        }
    }
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
        case GAME_STATE_OVERWORLD:
            UpdateGameStateGraphics();
            SMS_mapROMBank(main_spr_pal_bin_bank);
            GG_loadSpritePalette(main_spr_pal_bin);
            SMS_mapROMBank(OverworldMapBanks[MapId]);
            SMS_loadTileMapArea(6, 3, OverworldMapBins[MapId], 20, 16);
            SMS_mapROMBank(hud_tiles_bin_bank);
            SMS_loadTiles(hud_tiles_bin, 0, hud_tiles_bin_size);
            SMS_mapROMBank(hud_map_bin_bank);
            SMS_loadTileMapArea(6, 19, hud_map_bin, 20, 2);
        break;
    }
}

// Updates game state environmental animations
void UpdateEnvironmentAnimations(void) {
    if (IncrementTimer(TimerMax) == false || ScrollIndex != 0)
        return;

    if (SubState >= SubStateMax)
        SubState = 0;

    switch (GameState) {
        case GAME_STATE_OVERWORLD:
            switch (MapType)
            {
                case MAP_TYPE_NONE:
                    SMS_mapROMBank(animation_flower_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy32(8384, &animation_flower_tiles_bin[SubState << 5]);
                    SubState++;
                break;
                case MAP_TYPE_TURTLE_ROCK:
                    SMS_mapROMBank(animation_big_gem_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy32(12384, &animation_big_gem_tiles_bin[SubState << 5]);
                    SMS_mapROMBank(animation_small_gem_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy32(12896, &animation_small_gem_tiles_bin[SubState << 5]);
                    SubState++;
                break;
                case MAP_TYPE_VILLAGE:
                    SMS_mapROMBank(animation_flower_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy32(8384, &animation_flower_tiles_bin[SubState << 5]);
                    SMS_mapROMBank(animation_rooster_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy64(8704, &animation_rooster_tiles_bin[SubState << 6]);
                    SubState++;
                break;
                case MAP_TYPE_FOREST:
                    SMS_mapROMBank(animation_flower_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy32(8384, &animation_flower_tiles_bin[SubState << 5]);
                    SMS_mapROMBank(animation_forest_flower_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy32(10176, &animation_forest_flower_tiles_bin[SubState << 5]);
                    SubState++;
                break;
                case MAP_TYPE_SWAMP:
                    SMS_mapROMBank(animation_swamp_flower_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy32(10176, &animation_swamp_flower_tiles_bin[SubState << 5]);
                    SubState++;
                break;
                case MAP_TYPE_BOTTLE_GROTTO:
                    SMS_mapROMBank(animation_forest_flower_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy32(10176, &animation_forest_flower_tiles_bin[SubState << 5]);
                    SubState++;
                break;
                case MAP_TYPE_BEACH:
                    SMS_mapROMBank(animation_shore_tiles_bin_bank);
                    UNSAFE_SMS_VRAMmemcpy128(10112, &animation_shore_tiles_bin[SubState << 7]);
                    SubState++;
                break;
            }
        break;
    }
}

// Updates a map columns or rows based on scroll direction
void UpdateMapScroll(unsigned char dir) {
    switch (dir)
    {
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
            SMS_mapROMBank(OverworldMapBanks[MapId]);
            SMS_loadTileMapArea(6, 3, &OverworldMapBins[MapId][640 - (40 * (ScrollIndex - 3))], 20, ScrollIndex - 3);
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
            SMS_mapROMBank(OverworldMapBanks[MapId]);
            SMS_loadTileMapArea(6, ScrollIndex, OverworldMapBins[MapId], 20, 19 - ScrollIndex);
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
        UpdateMapScroll(ScrollDir);
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
        SMS_saveTileMapArea(6, 3, &Map, 20, 16);
        UpdateGameStateGraphics();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Game State Handling /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
        case GAME_STATE_OVERWORLD:
            switch (event) {
                case EVENT_VBLANK_LOAD:
                    ResetGlobalVariables();
                    LoadGameStateGraphics();
                break;
                case EVENT_AFTER_VBLANK:
                    UpdateEnvironmentAnimations();
                    TestScroll();
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
	SetGameState(GAME_STATE_OVERWORLD);
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