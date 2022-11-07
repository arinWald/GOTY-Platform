#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

struct transformPosition {
	float posX;
	float posY;
	bool turn;

};

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void Player::ChangePosition(int x, int y);


public:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* playerTexture;
	const char* texturePath;
	const char* deathFxPath;
	const char* jumpFxPath;
	const char* level1SongPath;

	Animation* currentAnimation;
	Animation rightRunAnimation;
	Animation rightIdleAnimation;
	Animation rightJumpAnimation;
	Animation rightFallAnimation;
	Animation rightDoubleJumpAnimation;

	Animation leftRunAnimation;
	Animation leftIdleAnimation;
	Animation leftJumpAnimation;
	Animation leftFallAnimation;
	Animation leftDoubleJumpAnimation;

	Animation dissappearAnimation;

	int speed;
	int jumpspeed;
	bool ground;
	int jumpsavailable;
	int playerlives;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	PhysBody* groundSensor;

	int timerJump;
	int pickCoinFxId;

	int timerPocho;
	int deathFxId;

	int timerDeath;
	bool isDead;

	int jumpFxId;
	int LastDir;

	transformPosition teleport;

private:



};

#endif // __PLAYER_H__