#include "screen.h"
#include "entities.h"

// 2D array of type Tile for the world level
Tile **world;

// A reference to the game's tilemap
Texture2D *textures;

//------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Starts the 2D camera by initializing it.
 */
void StartCamera();

/**
 * Loads all of the textures required for the dungeon by populating the array of textures.
 */
void LoadTextures();

void DungeonStartup() {
    // Sets the current screen 
    currentScreen = DUNGEON;

    // Allocate memory for the game's textures as Texture2D
    textures = (Texture2D*) malloc(MAX_TEXTURES * sizeof(Texture2D));

    // Allocate memory for the word as a 2D array of Tile
    world = (Tile**) malloc(WORLD_WIDTH * sizeof(Tile*));

    // Populating textures array with the texture images
    LoadTextures();

    // Allocating tiles of type Tile into 2D array
    for (int j = 0; j < WORLD_HEIGHT; j++) {
        Tile *tiles = (Tile*) malloc(WORLD_HEIGHT * sizeof(Tile));
        for (int i = 0; i < WORLD_WIDTH; i++) {
            Tile tile = {
                .x = i,
                .y = j
            };
            tiles[i] =  tile;
        }
        world[j] = tiles;
    }   

    StartCamera();
    PlayerStartup();
}

void DungeonUpdate() {
    PlayerMovement();
    PlayerAttack();
    
    // Update camera to follow the player
    camera.target = (Vector2) {player.x + 8, player.y + 16};
}

void DungeonRender() {
    Tile tile;

    // Rendering each tile by calling DrawTile
    for (int j = 0; j < WORLD_HEIGHT; j++) {
        for (int i = 0; i < WORLD_WIDTH; i++) {
            tile = world[i][j];
            DrawTile(tile.x * TILE_WIDTH, tile.y * TILE_HEIGHT, 7, 2, TILE_MAP);
        }
    }

    // Draw player on the screen  
    PlayerRender();
}

void DungeonUnload() {
    //Unloads the player sprites and animations.
    PlayerUnload();

    // Unloads all tiles
    for (int j = 0; j < WORLD_HEIGHT; j++) {
        free(world[j]);
        world[j] = NULL;
    }
    free(world);
    world = NULL;

    // Unloads texture array
    for(int i = 0; i < MAX_TEXTURES; i++) {
        UnloadTexture(textures[i]);
    }
    free(textures);
    textures = NULL;
}

void StartCamera() {
    camera.target = (Vector2) {player.x + 8, player.y + 16};
    camera.offset = (Vector2) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 4.0f;
}

void LoadTextures() {
    Image img = LoadImage("resources/tilemap.png");
    textures[TILE_MAP] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/player-idle.png");    
    textures[TILE_PLAYER_IDLE] = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("resources/player-movement.png");
    textures[TILE_PLAYER_MOVE] = LoadTextureFromImage(img);
    UnloadImage(img);

    // temp testing for non-loop animation. This should be an attack animation
    img = LoadImage("resources/player-attack.png");
    textures[TILE_PLAYER_ATTACK] = LoadTextureFromImage(img);
    UnloadImage(img);

    // NOTE: This tile map will be remove we must render each individual sprite. See /resources.
    img = LoadImage("resources/enemy-tilemap.png");
    textures[TILE_ENEMY] = LoadTextureFromImage(img);
    UnloadImage(img);
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