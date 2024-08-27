/***********************************************************************************************
 *
 **   audio.h is responsible for handling, storing, loading and unloading general audio
 **   (sfx and music).
 *
 *    @authors Marcus Vinicius Santos Lages and Samarjit Bhogal
 *    @version 0.2
 *
 *    @include raylib.h
 *
 ***********************************************************************************************/

#ifndef AUDIO_H_
#define AUDIO_H_

#include "raylib.h"

//* ------------------------------------------
//* DEFINITIONS

/** How many sound effects are loaded in the game.  */
#define MAX_SFX 8

/** How many songs are loaded in the game. */
#define MAX_SONGS 2

//* ------------------------------------------
//* ENUMERATIONS

/**
 * Enum for accessing the sound effects loaded in the game through
 * the soundFX array.
 *
 * @param CLICK_SFX         0
 * @param HOVER_SFX         1
 * @param OPEN_MENU_SFX     2
 * @param PLAYER_SLASH_SFX  3
 * @param SLASH_HIT_SFX     4
 * @param STEP_SFX          5
 * @param PLAYER_DEAD_SFX   6
 * @param ENEMY_DEAD_SFX    7
 */
typedef enum SoundFile {
    CLICK_SFX = 0,
    HOVER_SFX,
    OPEN_MENU_SFX,
    PLAYER_SLASH_SFX,
    SLASH_HIT_SFX,
    STEP_SFX,
    PLAYER_DEAD_SFX,
    ENEMY_DEAD_SFX
} SoundFile;

/**
 * Enum for accessing the songs loaded in the game through
 * the songs array.
 *
 * @param MENU_SONG     0
 * @param DUNGEON_SONG  1
 */
typedef enum MusicFile { MENU_SONG = 0, DUNGEON_SONG } MusicFile;

//* ------------------------------------------
//* GLOBAL VARIABLES

/** Array of all the SFX used in the game.
 *
 * @note Access the sounds through the SoundFile enum.
 */
extern Sound* soundFX;

/** Array of all the songs used in the game.
 *
 * @note Access the sounds through the MusicFile enum.
 */
extern Music* songs;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Loads all the necessary songs and sounds into the soundFX
 * and songs arrays and starts all necessary parameters for audio.
 *
 * @attention No need to use InitAudioDevice() before calling this function.
 */
void InitializeAudio();

/**
 * Unloads all the necessary songs and sounds from the soundFX
 * and songs arrays.
 *
 * @attention No need to use CloseAudioDevice() before calling this function.
 */
void UnloadAudio();

/**
 * Sets the volume for masterVolume, sfxVolumme and musicVolume and all its
 * related sounds/music.
 *
 * @param master    New volume master volume value.
 * @param sfx       New volume sfx volume value.
 * @param music     New volume music volume value.
 */
void SetAudioVolume(float master, float sfx, float music);

/**
 * Increment the value of the value of each kind of volume by each parameter.
 * 
 * @param masterIncrement Volume increment for master volume
 * @param sfxIncrement Volume increment for sfx volume
 * @param musicIncrement Volume increment for music volume
 */
// void IncrementVolume(float masterIncrement, float sfxIncrement, float musicIncrement);


/**
 * Returns the value of the audio volume of all sound effects.
 * 
 * @return Sfx volume as float
 */
float GetSfxVolume();

/**
 * Returns the value of the audio volume of all songs.
 * 
 * @return Music/songs volume as float
 */
float GetMusicVolume();

#endif // AUDIO_H_