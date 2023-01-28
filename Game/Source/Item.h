#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;
	Animation fruitAnimation;
	Animation nothingAnimation;
	Animation* currentAnimation;

	int id;
	bool isActive;

	bool disappear;
	
	//DONE 4: Add a physics to an item
	PhysBody* pbody;
};

#endif // __ITEM_H__