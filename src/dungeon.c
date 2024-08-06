/**********************************************************************************************
*
**   dungeon.c is responsible for dealing and assemblying all functionalities related to the
**   dungeon game screen.
*   
*    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
*    @version 0.1
*
*    @include screen.h, player.h
*
**********************************************************************************************/

#include "../include/screen.h"
#include "../include/player.h"
#include "../include/enemy.h"

//* ------------------------------------------
//* GLOBAL VARIABLES

/** 2D array of type Tile for the world level (texture.h) */ 
Tile** world;

/** Linked list with indexes to all the possible collidable tiles. */
CollisionNode * collidableTiles;

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

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void DungeonStartup() {
    // Sets the current screen 
    currentScreen = DUNGEON;

    // Allocate memory for the game's textures as Texture2D
    textures = (Texture2D*) malloc(MAX_TEXTURES * sizeof(Texture2D));

    // Allocate memory for the word as a 2D array of Tile
    world = (Tile**) malloc(WORLD_WIDTH * sizeof(Tile*));

    // Assign initial null value value for the collidableTiles list
    collidableTiles = NULL;

    // Populating textures array with the texture images
    LoadTextures();

    // Allocating tiles of type Tile into 2D array
    InitializeTiles();   

    StartCamera();
    EnemyStartup();
    PlayerStartup();
}

void DungeonUpdate() {
    PlayerMovement();
    EnemyMovement();
    EnemyAttack();
    PlayerAttack();
    
    // Update camera to follow the player
    camera.target = (Vector2) {(int) player.x + 8, (int) player.y + 16};
}

void DungeonRender() {
    Tile tile;

    // Rendering each tile by calling DrawTile
    for (int j = 0; j < WORLD_HEIGHT; j++) {
        for (int i = 0; i < WORLD_WIDTH; i++) {
            tile = world[j][i];
            if(j != 3)
                DrawTile(tile.x * TILE_WIDTH, tile.y * TILE_HEIGHT, 7, 2, TILE_MAP);
            else
                DrawTile(tile.x * TILE_WIDTH, tile.y * TILE_HEIGHT, 4, 2, TILE_MAP);
        }
    }

    // Draw player on the screen  
    EnemyRender();
    PlayerRender();
}

void DungeonUnload() {
    //Unloads the player sprites and animations.
    PlayerUnload();

    //Unloads the enemy sprites and animations.
    EnemyUnload();

    // Unloads all tiles
    for (int j = 0; j < WORLD_HEIGHT; j++) {
        free(world[j]);
        world[j] = NULL;
    }
    free(world);
    world = NULL;

    // Unloads collidableTiles list
    FreeCollisionList(collidableTiles);
    collidableTiles = NULL;

    // Unloads texture array
    for(int i = 0; i < MAX_TEXTURES; i++) {
        UnloadTexture(textures[i]);
    }
    free(textures);
    textures = NULL;
}

static void StartCamera() {
    camera.target = (Vector2) {player.x + 8, player.y + 16};
    camera.offset = (Vector2) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 4.0f;
}

static void LoadTextures() {
    Image img = LoadImage("resources/tilemap.png");
    textures[TILE_MAP] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/player-idle.png");    
    textures[TILE_PLAYER_IDLE] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/player-movement.png");
    textures[TILE_PLAYER_MOVE] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/player-attack.png");
    textures[TILE_PLAYER_ATTACK] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/enemy-1-idle.png");
    textures[TILE_ENEMY_IDLE] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/enemy-1-movement.png");
    textures[TILE_ENEMY_MOVE] = LoadTextureFromImage(img);
    UnloadImage(img);

    //! NOTE: player attack is used here temporailly. New asset must be found.
    img = LoadImage("resources/player-attack.png");
    textures[TILE_ENEMY_ATTACK] = LoadTextureFromImage(img);
    UnloadImage(img);
}

static void InitializeTiles() {
    for (int j = 0; j < WORLD_HEIGHT; j++) {
        Tile *tiles = (Tile*) malloc(WORLD_HEIGHT * sizeof(Tile));
        for (int i = 0; i < WORLD_WIDTH; i++) {
            Tile tile = {
                .x = i,
                .y = j,
                .isCollidable = (j != 3) ? false : true
            };
            tiles[i] =  tile;

            if(tile.isCollidable == true) {
                if(collidableTiles == NULL)
                    collidableTiles = CreateCollisionList(tile.x, tile.y, 0);
                else
                    AddCollisionNode(collidableTiles, tile.x, tile.y, 0);
            }
        }
        world[j] = tiles;
    }
}

void DrawTile(int xPos, int yPos, int textureTileX, int textureTileY, TextureFile tileTexture) {
    Rectangle source = { 
        (float) (textureTileX * TILE_WIDTH), 
        (float) (textureTileY * TILE_HEIGHT), 
        (float) TILE_WIDTH, 
        (float) (tileTexture > TILE_MAP ? ENTITY_TILE_HEIGHT : TILE_HEIGHT) 
        };
    Rectangle dest = { 
        (float) xPos, 
        (float) yPos, 
        (float) TILE_WIDTH, 
        (float) (tileTexture > TILE_MAP ? ENTITY_TILE_HEIGHT : TILE_HEIGHT) 
        };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(
        textures[tileTexture],
        source, 
        dest, 
        origin,
        0.0f, 
        WHITE);
}