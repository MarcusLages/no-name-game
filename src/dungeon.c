#include "screen.h"
#include "texture.h"

// 2D array of type Tile for the world level
Tile **world;

// A reference to the game's tilemap
Texture2D *textures;

//------------------------------------------
//* FUNCTION PROTOTYPES
void StartCamera();

void DungeonStartup() {
    // Sets the current screen 
    currentScreen = DUNGEON;

    // Loads the game's tilemap into heap as Texture2D
    Image img = LoadImage("resources/tilemap.png");
    textures = (Texture2D*) malloc(sizeof(Texture2D));
    textures[1] = LoadTextureFromImage(img);

    // Unloads the image as it is no longer needed
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
}

void DungeonUpdate() {
}

void DungeonRender() {
    Tile tile;

    // Rendering each tile by calling DrawTile
    for (int j = 0; j < WORLD_HEIGHT; j++) {
        for (int i = 0; i < WORLD_WIDTH; i++) {
            tile = world[i][j];
            DrawTile(tile.x * TILE_WIDTH, tile.y * TILE_HEIGHT, 5, 0);
        }
    }
}

void DungeonUnload() {
    for (int j = 0; j < WORLD_HEIGHT; j++) {
        free(world[j]);
        world[j] = NULL;
    }
    free(world);
    world = NULL;
}

/**
 * Starts the 2D camera by initializing it.
 */
void StartCamera() {
    // Initialize camera
    // TODO: make camera responsive
    camera.target = (Vector2) {1000, 1000};
    camera.offset = (Vector2) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

/**
 * Draws tiles at (x, y) from the tilemap from (x, y).
 * 
 * @param x_pos the x-coord to draw on
 * @param y_pos the y-coord to draw on
 * @param texture_tile_x the x-coord the tile in present on
 * @param texture_tile_y the y-coord the tile in present on
 */
void DrawTile(int x_pos, int y_pos, int texture_tile_x, int texture_tile_y) {
    Rectangle source = { 
        (float) (texture_tile_x * TILE_WIDTH), 
        (float) (texture_tile_y * TILE_HEIGHT), 
        (float) TILE_WIDTH, 
        (float) TILE_HEIGHT 
        };
    Rectangle dest = { 
        (float) x_pos, 
        (float) y_pos, 
        (float) TILE_WIDTH, 
        (float) TILE_HEIGHT
        };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(
        textures[1],
        source, 
        dest, 
        origin,
        0.0f, 
        WHITE);
}