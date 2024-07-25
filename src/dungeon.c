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

void DungeonStartup() {
    // Sets the current screen 
    currentScreen = DUNGEON;

    // Loads the game's tilemap into heap as Texture2D
    textures = (Texture2D*) malloc(MAX_TEXTURES * sizeof(Texture2D));

    Image img = LoadImage("resources/tilemap.png");
    textures[TILE_MAP] = LoadTextureFromImage(img);
    img = LoadImage("resources/player-idle.png");
    textures[TILE_PLAYER_IDLE] = LoadTextureFromImage(img);
    img = LoadImage("resources/player-movement.png");
    textures[TILE_PLAYER_MOVE] = LoadTextureFromImage(img);
    img = LoadImage("resources/enemy-tilemap.png");
    textures[TILE_ENEMY] = LoadTextureFromImage(img);
    UnloadImage(img);

    world = (Tile**) malloc(WORLD_WIDTH * sizeof(Tile*));

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
    // SpriteUpdate();
    PlayerMovement();
    
    // Update camera to follow the player
    camera.target = (Vector2) {player.x + 8, player.y + 16};
}

void DungeonRender() {
    Tile tile;

    // Rendering each tile by calling DrawTile
    for (int j = 0; j < WORLD_HEIGHT; j++) {
        for (int i = 0; i < WORLD_WIDTH; i++) {
            tile = world[i][j];
            DrawTile(tile.x * TILE_WIDTH, tile.y * TILE_HEIGHT, 4, 0, TILE_MAP);
        }
    }

    // Draw player on the screen
    DrawTile(player.x, player.y, 0, 0, TILE_PLAYER_IDLE);
}

void DungeonUnload() {
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

void DrawTile(int x_pos, int y_pos, int texture_tile_x, int texture_tile_y, TextureFile tileTexture) {
    Rectangle source = { 
        (float) (texture_tile_x * TILE_WIDTH), 
        (float) (texture_tile_y * TILE_HEIGHT), 
        (float) TILE_WIDTH, 
        (float) (tileTexture > TILE_MAP ? ENTITY_TILE_HEIGHT : TILE_HEIGHT) 
        };
    Rectangle dest = { 
        (float) x_pos, 
        (float) y_pos, 
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