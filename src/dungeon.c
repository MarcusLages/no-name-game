/**********************************************************************************************
 *
 **   dungeon.c is responsible for dealing and assemblying all functionalities related to the
 **   dungeon game screen.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.3
 *
 *    @include  <stdlib.h>, screen.h, tile.h, audio.h, enemy-list.h, player.h
 *
 **********************************************************************************************/

#include "../include/audio.h"
#include "../include/enemy-list.h"
#include "../include/player.h"
#include "../include/screen.h"
#include "../include/tile.h"
#include <stdlib.h>

//* ------------------------------------------
//* GLOBAL VARIABLES

/** Pointer for the framebuffer (white canvas) for displaying the map. */
RenderTexture2D* worldCanvas;

/** Linked list with indexes to all the possible collidable tiles. */
CollisionNode* collidableTiles;

/** A reference to the game's tilemap (texture.h) */
Texture2D* textures;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Starts the 2D camera by initializing it.
 */
static void StartCamera();

/**
 * Loads all of the textures required for the dungeon by populating the array of textures.
 */
static void LoadTextures();

/**
 * Allocates all the tiles into the world array and the collidableTiles list.
 */
static void InitializeTiles();

/**
 * @return True if player is dead.
 */
static bool IsPlayerDead();

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void DungeonStartup() {
    // Sets the current screen
    currentScreen = DUNGEON;

    // Allocate memory for the game's textures as Texture2D
    textures = (Texture2D*) malloc(MAX_TEXTURES * sizeof(Texture2D));

    // Allocate memory for the world framebuffer as RenderTexture2D
    worldCanvas = (RenderTexture2D*) malloc(sizeof(RenderTexture2D));

    // Assign initial null value value for the collidableTiles list
    collidableTiles = NULL;

    // Populating textures array with the texture images
    LoadTextures();

    // Allocating tiles of type Tile into 2D array
    InitializeTiles();

    StartCamera();
    SetupEnemies();
    PlayerStartup();

    if(!IsMusicStreamPlaying(songs[DUNGEON_SONG])) {
        StopMusicStream(songs[MENU_SONG]);
        PlayMusicStream(songs[DUNGEON_SONG]);
    }

    TraceLog(LOG_INFO, "DUNGEON.C (DungeonStartup): Dungeon loaded successfully.");
}

void DungeonUpdate() {
    // If player is dead, no need to check for anything
    // Instead, sends him to the final screen
    if(!IsPlayerDead() && enemies != NULL) {
        UpdateMusicStream(songs[DUNGEON_SONG]);
        PlayerUpdate();
        UpdateEnemies();

        // Update camera to follow the player
        camera.target = (Vector2){ (int) player.pos.x + 8, (int) player.pos.y + 16 };
    } else
        nextScreen = FINAL_SCREEN;
}

void DungeonRender() {
    // If player is dead, no need to check for anything
    if(!IsPlayerDead()) {
        // Render the world canvas
        // ? Note: height is negative because OpenGL orientation is inverted from Raylib
        DrawTextureRec(
            worldCanvas->texture,
            (Rectangle){ 0, 0, worldCanvas->texture.width, -worldCanvas->texture.height },
            (Vector2){ 0.0f, 0.0f }, WHITE);

        // Draw player on the screen
        RenderEnemies();
        PlayerRender();
    }
}

void DungeonUnload() {
    // Unloads the player sprites and animations.
    PlayerUnload();

    // Unloads the enemy sprites and animations.
    UnloadEnemies();

    // Unloads collidableTiles list
    FreeCollisionList(collidableTiles);
    collidableTiles = NULL;

    TraceLog(LOG_INFO, "DUNGEON.C (DungeonUnload): Collidable tiles list unloaded successfully.");

    // Unloads texture array
    for(int i = 0; i < MAX_TEXTURES; i++) {
        UnloadTexture(textures[i]);
    }
    free(textures);
    textures = NULL;

    TraceLog(LOG_INFO, "DUNGEON.C (DungeonUnload): Dungeon textures unloaded successfully.");

    // Unloads the worldCanvas framebuffer
    UnloadRenderTexture(*worldCanvas);
    worldCanvas = NULL;

    TraceLog(LOG_INFO, "DUNGEON.C (DungeonUnload): World dungeon tilemap framebuffer unloaded successfully.");
    TraceLog(LOG_INFO, "DUNGEON.C (DungeonUnload): Dungeon unloaded successfully.");
}

static void StartCamera() {
    camera.target   = (Vector2){ player.pos.x + 8, player.pos.y + 16 };
    camera.offset   = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    camera.rotation = 0.0f;
    camera.zoom     = 4.0f;
}

static void LoadTextures() {
    Image img = LoadImage("resources/img/player/player-idle2.png");
    textures[TILE_PLAYER_IDLE] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/img/player/player-movement2.png");
    textures[TILE_PLAYER_MOVE] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/img/player/player-attack.png");
    textures[TILE_PLAYER_ATTACK] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/img/enemies/pablo-idle.png");
    textures[TILE_ENEMY_PABLO_IDLE] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/img/enemies/pablo-movement.png");
    textures[TILE_ENEMY_PABLO_MOVE] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/img/enemies/pablo-diego-attack.png");
    textures[TILE_ENEMY_PABLO_ATTACK] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/img/enemies/diego-idle.png");
    textures[TILE_ENEMY_DIEGO_IDLE] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/img/enemies/diego-movement.png");
    textures[TILE_ENEMY_DIEGO_MOVE] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/img/enemies/pablo-diego-attack.png");
    textures[TILE_ENEMY_DIEGO_ATTACK] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/img/enemies/waffles-idle.png");
    textures[TILE_ENEMY_WAFFLES_IDLE] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/img/enemies/waffles-movement.png");
    textures[TILE_ENEMY_WAFFLES_MOVE] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/img/enemies/waffles-attack.png");
    textures[TILE_ENEMY_WAFFLES_ATTACK] = LoadTextureFromImage(img);
    UnloadImage(img);

    img                         = LoadImage("resources/img/heart-meter.png");
    textures[TILE_HEALTH_METER] = LoadTextureFromImage(img);
    UnloadImage(img);

    TraceLog(LOG_INFO, "DUNGEON.C (LoadTextures): All dungeon textures loaded.");
}

static void InitializeTiles() {
    tmx_map* mapTmx;
    mapTmx = TmxMapFrameBufStartup(worldCanvas, "resources/map/map.tmx");
    TmxMapFrameBufRender(worldCanvas, mapTmx);

    // Frees the map.tmx
    tmx_map_free(mapTmx);
    mapTmx = NULL;

    TraceLog(LOG_INFO, "DUNGEON.C (InitializeTiles): Tmx map unloaded.");
}

static bool IsPlayerDead() { return player.health <= 0; }