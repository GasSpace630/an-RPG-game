#pragma once
#include "raylib.h"
#include <vector>

// Base class player and enemies
class Entity {
public:
	Vector2 position = {0.0f,0.0f};
	Color color = RED;
	float size = 10.0f;
	Rectangle rect = {0 , 0 , 10 , 10};

	float health = 100.0f;
	bool alive = true;
	
	virtual void Update() = 0;
	virtual void Draw() = 0;
	
	virtual ~Entity() = default;
};