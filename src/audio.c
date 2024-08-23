/***********************************************************************************************
 *
 **   audio.c is responsible for implementing general audio handling functions (sfx and audio).
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.2
 *
 *    @include
 *
 ***********************************************************************************************/

#include "../include/audio.h"
#include "raymath.h"
#include <stdlib.h>

//* ------------------------------------------
//* ENUMERATIONS

/**
 * Enum to check which type of audio file is an input.
 *
 * @param SoundType 0
 * @param MusicType 0
 */
typedef enum AudioType { SoundType = 0, MusicType } AudioType;

//* ------------------------------------------
//* GLOBAL VARIABLES

Sound* soundFX;
Music* songs;

//* ------------------------------------------
//* MODULAR VARIABLES

/** Variable that controls master volume. */
float masterVolume;

/** Variable that controls sound effects volume. */
float sfxVolume;

/** Variable that controls music volume. */
float musicVolume;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Function to load sounds into soundFX array.
 */
static void LoadSFX();

/**
 * Function to unload sounds from soundFX array.
 */
static void UnloadSFX();

/**
 * Function to load songs into songs array.
 */
static void LoadSongs();

/**
 * Function to unload songs from songs array.
 */
static void UnloadSongs();

// static void LoopThroughData();

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void InitializeAudio() {
    InitAudioDevice();

    LoadSFX();
    LoadSongs();

    SetAudioVolume(0.5f, 0.5f, 0.5f);
}

void UnloadAudio() {
    UnloadSFX();
    UnloadSongs();
    CloseAudioDevice();
}

void SetAudioVolume(float master, float sfx, float music) {
    if(!FloatEquals(master, masterVolume)) SetMasterVolume(master);

    if(!FloatEquals(sfx, sfxVolume)) {
        sfxVolume = sfx;
        for(int index = 0; index < MAX_SFX; index++) {
            SetSoundVolume(soundFX[index], sfxVolume);
        }
    }

    if(!FloatEquals(music, musicVolume)) {
        musicVolume = music;
        for(int index = 0; index < MAX_SONGS; index++) {
            SetSoundVolume(soundFX[index], musicVolume);
        }
    }
}

static void LoadSFX() {
    // ALlocate memory
    soundFX = (Sound*) malloc(MAX_SFX * sizeof(Sound));
    if(soundFX == NULL) {
        TraceLog(LOG_FATAL, "AUDIO.C (LoadSFX, line: %d): Memory allocation for soundFX failure.", __LINE__);
    }

    soundFX[CLICK_SFX]        = LoadSound("resources/sounds/clickButton.wav");
    soundFX[HOVER_SFX]        = LoadSound("resources/sounds/hoverButton.wav");
    soundFX[OPEN_MENU_SFX]    = LoadSound("resources/sounds/openMenu.wav");
    soundFX[PLAYER_SLASH_SFX] = LoadSound("resources/sounds/playerSlash.wav");
    soundFX[SLASH_HIT_SFX]   = LoadSound("resources/sounds/playerSlashHit.wav");
    soundFX[STEP_SFX]        = LoadSound("resources/sounds/step.wav");
    soundFX[PLAYER_DEAD_SFX] = LoadSound("resources/sounds/playerDead.wav");
    soundFX[ENEMY_DEAD_SFX]  = LoadSound("resources/sounds/enemyDead.wav");

    TraceLog(LOG_INFO, "AUDIO.C (LoadSFX): All SFX loaded successfully.");
}

static void UnloadSFX() {
    for(int sfxIndex = 0; sfxIndex < MAX_SFX; sfxIndex++)
        UnloadSound(soundFX[sfxIndex]);

    free(soundFX);
    soundFX = NULL;

    TraceLog(LOG_INFO, "AUDIO.C (UnloadSFX): All SFX unloaded successfully.");
}

// TODO: Add songs and the uncomment
static void LoadSongs() {
    // ALlocate memory
    songs = (Music*) malloc(MAX_SFX * sizeof(Music));
    if(songs == NULL) {
        TraceLog(LOG_FATAL, "AUDIO.C (LoadSongs, line: %d): Memory allocation for songs failure.", __LINE__);
    }

    // songs[MENU_SONG]    = LoadMusicStream("resources/music/menu-song.mp3");
    // songs[DUNGEON_SONG] = LoadMusicStream("resources/music/dungeon-song.mp3");

    TraceLog(LOG_INFO, "AUDIO.C (LoadSongs): All songs loaded successfully.");
}

// TODO: Add songs and uncomment
static void UnloadSongs() {
    // for(int songIndex = 0; songIndex < MAX_SONGS; songIndex++)
    //     UnloadMusicStream(songs[songIndex]);

    free(songs);
    songs = NULL;

    TraceLog(LOG_INFO, "AUDIO.C (UnloadSongs): All songs unloaded successfully.");
}

// ? Ignore it, it's my try on using function pointers
// static void LoopThroughArray(AudioType type, void* array, void (*callback)(void*)) {
//     int max;

//     switch(type) {
//         case SoundType: max = MAX_SFX; break;
//         case MusicType: max = MAX_SONGS; break;
//     }

//     for(int index; index < max; index++) {
//         callback( (void*) array );
//     }
// }