#include "raylib.h"
#include "raymath.h"
#include "ctime" // for time() in seeding
#include "vector"
#include "memory" // for unique_ptr
#include "algorithm" // for remove_if
#include "iostream"

#include "include/colors.h" // My colours

class Player;

// Base class player and enemies
class Entity {
public:
	Vector2 position = {0.0f,0.0f};
	Color color = RED;
	float size = 10.0f;
	Rectangle rect = {0 , 0 , 10 , 10};

	float health = 100.0f;
	bool alive = true;
	
	void Update(const Player& player);
	void Draw();
	
	virtual ~Entity() = default;
};

// Player class for player stuff
class Player : public Entity{
public:
	float size = 26.0f; // Radius of circle *Temporary*
	float speed = 4.0f;
	Vector2 position = {0,0};
	Rectangle rect = {position.x , position.y , size , size};
	Color color = PLAYER_BLUE;
	float strength = 10.0f;
	Vector2 direction = {0 , 0};

	// WASD,Arrow key movement
	void movementSystem() {
		direction = {0.0f , 0.0f};
		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {direction.y -= 1.0f;}
		if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {direction.y += 1.0f;}
		if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {direction.x -= 1.0f;}
		if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {direction.x += 1.0f;}
		
		// normalizing direction
		if (direction.x != 0.0f || direction.y != 0.0f) {
			direction = Vector2Normalize(direction);
			position = Vector2Add(position , Vector2Scale(direction , speed));
		}
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
	float size = 30.0f;
	float speed = 1.0f;
	Vector2 position = {100, 100};
	Color color = ENEMY_RED;
	Rectangle rect = {position.x , position.y , size , size};
	
	bool isCollision;
	
	int directionX = 0;
	int directionY = 0;
	Vector2 direction = {0.0f , 0.0f};
	float directionChangeDelay = GetRandomValue(1 , 3);
	float directionChangeTimer = 0.0f;
	
	// Random Movement
	void movementSystem() {
		directionChangeTimer += GetFrameTime();
		
		if (directionChangeTimer >= directionChangeDelay) {
			directionChangeTimer = 0.0f;
			direction.x = GetRandomValue(-1,1);
			direction.y = GetRandomValue(-1,1);
		}
		if (direction.x != 0 || direction.y != 0) {IsMoving = true;}
		else {IsMoving = false;}
		
		if (IsMoving) {
			direction = Vector2Normalize(direction);
			position = Vector2Add(position , Vector2Scale(direction , speed));
		}
	}
	
	void collisionSystem(const Player& player) {
		isCollision = CheckCollisionRecs(rect , player.rect);
		if (isCollision && IsKeyPressed(KEY_K)) {
			health -= player.strength;
			color = RAYWHITE;
		}else {
			color = ENEMY_RED;
		}
	}
	
	// Update stuff
	void Update(const Player& player) {
		movementSystem();
		collisionSystem(player);

		if (health <= 0) {alive = false;}
	}
	// Drawing
	void Draw() {
		rect.x = position.x;
		rect.y = position.y;
		DrawRectangleRec(rect , color);
		char textBuffer[64];
		snprintf(textBuffer , 64 , "Enemy : %.2f" , health);
		DrawText(textBuffer , (position.x), (position.y), 10 , BLACK);
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
		ClearBackground(GRASS_GREEN);
		
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
