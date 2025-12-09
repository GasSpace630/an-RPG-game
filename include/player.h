#pragma once
#include "entity.h"
#include <memory>
#include <vector>

// Forward declaration of Enemy
class Enemy; 

// Player class for player stuff
class Player : public Entity{
public:
	float size = 26.0f; // size of the box (replace with sprite/texture)
	float speed = 4.0f;
	float strength = 10.0f;
	int kills = 0; // how much kill you have
	Vector2 direction = {0.0f, 0.0f};
	Rectangle rect = {0 , 0 , size , size};

	// WASD | Arraow key movement
	void movementSystem();

	// Use the forward declared Enemy here:
	void collisionSystem(std::vector<std::unique_ptr<Enemy>>& enemies);
	
	// Update functions
	void Update(std::vector<std::unique_ptr<Enemy>>& enemies);
	void Update() override;

	// Draw the player
	void Draw();
};