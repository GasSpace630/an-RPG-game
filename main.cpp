#include "raylib.h"
#include "ctime" // for time() in seeding
#include "vector"
#include "memory" // for unique_ptr
#include "algorithm" // for remove_if
#include "iostream"

class Player;

// Base class player and enemies
class Entity {
public:
	Vector2 position = {0.0f,0.0f};
	Color color = RED;
	int size = 10;
	Rectangle rect = {10 , 10 , 0 , 0};
	bool alive = true;
	
	virtual void Update(const Player& player) = 0;
	virtual void Draw() = 0;
	
	virtual ~Entity() = default;
	};

// Player class for player stuff
class Player {
public:
	int size = 30; // Radius of circle *Temporary*
	int speed = 6;
	Vector2 position = {0,0};
	Rectangle rect = {0,0,30,30};
	Color color = LIME;
	
	// WASD,Arrow key movement
	void movementSystem() {
		// change to click to move
		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {position.y -= speed;}
		if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {position.y += speed;}
		if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {position.x -= speed;}
		if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {position.x += speed;}
		}
	
	void Update() {
		movementSystem();
		}
	// Drawing the player just a blue circle for now
	void Draw() {
		rect.x = position.x;
		rect.y = position.y;
		DrawRectangleRec(rect, color);
		DrawText("Player", (position.x), (position.y), 16 , BLACK);
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
	
	bool isCollision;
	
	int directionX = 0;
	int directionY = 0;
	float directionChangeDelay = GetRandomValue(2 , 3);
	float directionChangeTimer = 0.0f;
	
	// Random Movement
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
	
	void collisionSystem(const Player& player) {
		isCollision = CheckCollisionCircleRec(position, size, player.rect);
		if (isCollision) {alive = false;}
		}
	
	// Update stuff
	void Update(const Player& player) override {
		movementSystem();
		collisionSystem(player);
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

		playerRawPtr -> Update();
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
		ClearBackground(GREEN);
		
		BeginMode2D(camera);

		playerRawPtr -> Draw();
		
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


/*
 * I dont understand how i am removing enemies yet!
 * */
