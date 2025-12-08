#include "raylib.h"
#include "raymath.h"
#include "ctime" // for time() in seeding
#include "vector"
#include "memory" // for unique_ptr
#include "algorithm" // for remove_if
#include "iostream"

#include "colors.h" // My colours
#include "entity.h"
#include "player.h"
#include "enemy.h"


// Function def to Draw a GUI
void DrawGUI();

// The game stuff
int main(void) {
	// Window Dimensions
	int windowWidth = 800;
	int windowHeight = 450;

	// Making the window
	InitWindow(windowWidth, windowHeight, "Test Game 1");

	// Setting the max FPS
	SetTargetFPS(60);
	
	// Seeding the RNG
	SetRandomSeed((unsigned int)time(0));
	
	// Initializing game objects
	auto playerPtr = std::make_unique<Player>();
	// A raw pointer to player for camera tracking
	Player* playerRawPtr = playerPtr.get();

	// making enemies
	std::vector<std::unique_ptr<Enemy>> entities;
	const int ENEMY_COUNT = 4;
	for (int i = 0; i < ENEMY_COUNT; ++i) {
		auto newEnemyPtr = std::make_unique<Enemy>();
		entities.push_back(std::move(newEnemyPtr));
	}
	
	// Setting the world camera
	Camera2D camera = {0};
	camera.zoom = 1.0f;
	camera.rotation = 0.0f;
	camera.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	camera.target = (Vector2){ 0.0f, 0.0f }; // Start camera at world origin

	// Main loop ---------------------------------------------------
	while(!WindowShouldClose()){

		playerRawPtr -> Update(entities);
		// Update all entities
		for (const auto& entity : entities) {entity -> Update(*playerRawPtr);}
		// removing entities if not alive 
		auto new_end = std::remove_if(entities.begin(), entities.end(), [](const auto& entityPtr) {
			return entityPtr->alive == false;
		});
		entities.erase(new_end, entities.end());
		// Setting camera centered to player
		camera.target = playerRawPtr -> position;
		
		// Drawing stuff ---------------------------------------------
		BeginDrawing();
		
		// White background
		ClearBackground(GRASS_GREEN);
		
		BeginMode2D(camera);

		playerRawPtr -> Draw();
		
		// Drawing entites
		for (const auto& entity : entities) {entity -> Draw();}
		
		EndMode2D();

		DrawGUI();
		
		EndDrawing();
		
	}
	// CLosing the window
	CloseWindow();

	return 0;
}


void DrawGUI() {
	DrawText("just an RPG game", 2, 20, 13, BLACK);
	DrawFPS(0,0);
}
