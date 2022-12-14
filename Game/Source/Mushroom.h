#ifndef __SMALL_ENEMY_1__
#define __SMALL_ENEMY_1__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "SDL_Timer.h"
#include "Animation.h"

struct SDL_Texture;

class SmallEnemy1 : public Entity
{
public:

	SmallEnemy1();
	virtual ~SmallEnemy1();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void LoadInfo(iPoint pos, int state);

	void SaveInfo();

	int GetState();


private:

	//void ChaseMovement();
	void ChaseMovement2();

	//void SentryMovement();
	void SentryMovement2();

	//void ReturnMovement();
	void ReturnMovement2();




public:

	Animation* currentAnimationEnemy = nullptr;

	int limiteIzqX;
	int limiteDerX;
	int scalarLimites;
	int map;

	bool walkDir;
	bool attackE;
	bool animAtk;
	int speedinX;

	Animation walkRAnimEnemy;
	Animation walkLAnimEnemy;
	Animation angryRAnimEnemy;
	Animation angryLAnimEnemy;
	Animation attackRAnimEnemy;
	Animation attackLAnimEnemy;

	//bool destroy;

	enum estadoSEnemy1 {
		STOP = 0,
		SENTRY,
		CHASE,
		DEATH,
		RETURN,
		NONE,
	};
	int estadoSE1;

	enum direccionEnemy {
		IZQUIERDA,
		DERECHA,
	};
	int direccionE;

private:

	SDL_Texture* texture;
	const char* texturePath;
	SDL_Timer attackAnimTimer;
	//DONE 4: Add a physics
//	PhysBody* pbody;

	iPoint lastPos;
	int pathIterator;

	iPoint initialPosition, leftBorder, rightBorder;
	int range;
	bool startPath;
	float nextFootStep;
	float amountToMoveInX;
	float destination;
	bool firstPath;
	bool achievedRightBorder;
	bool achievedLeftBorder;
	bool debug;
	int playerTileX;
	int playerTileY;
	int limitToChaseX;
	int limitToChaseY;
	bool attackAnimation;

	struct dataFromSave
	{
		int posX;
		int posY;
		bool startPath;
		float nextFootStep;
		float amountToMoveInX;
		float destination;
		bool firstPath;
		bool achievedRightBorder;
		bool achievedLeftBorder;
		bool attackAnimation;
		int estado;
		Animation* animation = nullptr;

	} newData;

	bool changedDataFromSave;

	int framesStopped;
	int lastPosinX;
	int lastPosinY;

	float speedX;
	float speedLimit;

	bool auxBug;
	int intBug;
};

#endif // !__SMALL_ENEMY_1__

