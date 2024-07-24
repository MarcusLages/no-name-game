#include "screen.h"
#include "texture.h"

Tile **world;
Texture2D *textures;

void DungeonStartup() {
    currentScreen = DUNGEON;

    Image img = LoadImage("resources/tilemap.png");
    textures = (Texture2D*) malloc(sizeof(Texture2D));
    textures[1] = LoadTextureFromImage(img);
    UnloadImage(img);

    world = (Tile**) malloc(WORLD_WIDTH * sizeof(Tile*));

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
}

void DungeonUpdate() {}

void DungeonRender() {
    Tile tile;

    for (int j = 0; j < WORLD_HEIGHT; j++) {
        for (int i = 0; i < WORLD_WIDTH; i++) {
            tile = world[i][j];
            DrawTile(tile.x * TILE_WIDTH, tile.y * TILE_HEIGHT, 4, 0);
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