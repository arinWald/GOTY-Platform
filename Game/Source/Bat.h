#ifndef __Bat_H__
#define __Bat_H__


#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "DynArray.h"

struct SDL_Texture;


class Bat : public Entity
{
	enum class State
	{
		IDLE,
		FLYING,
		DYING
	};
public:


	Bat();

	virtual ~Bat();


	bool Awake();

	bool Start();

	bool Update();

	bool Draw();

	bool CleanUp();


	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void Bat::ChangePosition(int x, int y);

	void Reset();


	SDL_Texture* BatTexture;
	const char* texturePath;

	Animation idleAnimation;
	Animation flyingLeftAnimation;
	Animation flyingRightAnimation;
	Animation questionMarkAnimation;
	Animation deathAnimation;
	Animation* currentAnimation;

	State state;
	PhysBody* batbody;
	iPoint lastPlayerPosition;
	DynArray<iPoint> path;
	int pathIndex = 0;
	int speed;
	bool hasPath = false;
	fPoint initialPosition;


private:


	


};

#endif