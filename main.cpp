#include "raylib.h"
#include "ctime" // for time() in seeding
#include "vector"
#include "memory" // for unique_ptr
#include "iostream"

// Base class player and enemies
class Entity {
public:
	Vector2 position = {0.0f,0.0f};
	int size = 10;
	
	virtual void Update() = 0;
	virtual void Draw() = 0;
	
	virtual ~Entity() = default;
	};

// Player class for player stuff
class Player : public Entity {
public:
	int size = 30; // Radius of circle *Temporary*
	int speed = 6;
	Vector2 position = {0,0};
	Color color = BLUE;
	
	// WASD,Arrow key movement
	void movementSystem(){
		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {position.y -= speed;}
		if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {position.y += speed;}
		if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {position.x -= speed;}
		if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {position.x += speed;}
		}
	
	void Update() override {
		movementSystem();
		}
	// Drawing the player just a blue circle for now
	void Draw() override {
		DrawCircleV(position, size, color);
		DrawText("Player", (position.x-size), (position.y-size), 16 , BLACK);
		}
	};

// Enemys , stuff you can beat up to get stuff
class Enemy : public Entity {
private:
	bool IsMoving = false;
public:
	int size = 20;
	int speed = 2;
	Vector2 position = {100, 100};
	Color color = RED;
	
	int directionX = 0;
	int directionY = 0;
	float directionChangeDelay = 1.0f;
	float directionChangeTimer = 0.0f;
	void movementSystem() {
		directionChangeTimer += GetFrameTime();
		
		if (directionChangeTimer >= directionChangeDelay) {
			directionChangeTimer = 0.0f;
			directionX = GetRandomValue(-1,1);
			directionY = GetRandomValue(-1,1);
			}
		if (directionX != 0 || directionY != 0) {IsMoving = true;}
		else {IsMoving = false;}
		
		if (IsMoving) {
			position.x += (speed * directionX);
			position.y += (speed * directionY);
			}
		}
	// Update stuff
	void Update() override {
		movementSystem();
		}
	// Drawing a circle
	void Draw() {
		DrawCircleV(position, size, color);
		}
	};

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
	std::vector<std::unique_ptr<Entity>> entities;
	auto playerPtr = std::make_unique<Player>();
	auto enemy1Ptr = std::make_unique<Enemy>();
	
	// A ray pointer to player for camera tracking
	Player* playerRawPtr = playerPtr.get();
	// Adding them to entity list
	entities.push_back(std::move(playerPtr));
	entities.push_back(std::move(enemy1Ptr));
	
	// Setting the world camera
	Camera2D camera = {0};
	camera.zoom = 1.0f;
	camera.rotation = 0.0f;
	camera.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	camera.target = (Vector2){ 0.0f, 0.0f }; // Start camera at world origin

	// Main loop
	while(!WindowShouldClose()){
		// Update all entities
		for (const auto& entity : entities) {entity -> Update();}
		// Setting camera centered to player
		camera.target = playerRawPtr -> position;
		
		BeginDrawing();
		
		// White background
		ClearBackground(RAYWHITE);
		
		BeginMode2D(camera);
		
		// Drawing entites
		for (const auto& entity : entities) {entity -> Draw();}
		
		EndMode2D();
		
		// Creator Info and Stuff
		DrawText("just an RPG game", 2, 20, 13, BLACK);
		DrawFPS(0,0);
		
		EndDrawing();
		
		}
	// CLosing the window
	CloseWindow();

	return 0;
}
