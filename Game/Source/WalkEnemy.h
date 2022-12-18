#ifndef __WALKENEMY_H__
#define __WALKENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

struct transformEnemyPosition {
	float posX;
	float posY;
	bool turn;

};

enum moveState
{
	IDLE,
	CHASING,
	GETTINGHURT,
	DYING
};

class WalkEnemy : public Entity
{
public:

	WalkEnemy();
	
	virtual ~WalkEnemy();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void WalkEnemy::ChangePosition(int x, int y);


public:

	SDL_Texture* enemyTexture;
	const char* texturePath;
	const char* deathFxPath;
	int deathFxId;

	Animation* currentAnimation;
	/*Animation rightIdleAnimation;
	Animation rightHitAnimation;
	Animation rightRunAnimation;*/

	Animation leftIdleAnimation;
	Animation leftHitAnimation;
	Animation leftRunAnimation;

	Animation dissappearAnimation;

	int speed;

	PhysBody* pbody;

	int timerDeath;
	bool isDead;

	transformEnemyPosition teleport;

	int initialPosX;
	int initialPosY;

	int currentMoveState;

private:



};

#endif // __WALKENEMY_H__