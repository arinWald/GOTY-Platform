#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Animation.h"
#include "Player.h"
#include "Scene.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	id = parameters.attribute("id").as_int();
	isActive = parameters.attribute("isActive").as_bool();

	if (id == 1)
	{
		for (int i = 0; i < 17; ++i)
		{
			fruitAnimation.PushBack({ 32 * i, 0, 32, 32 });
		}
		fruitAnimation.speed = 0.5f;
		fruitAnimation.loop = true;

		nothingAnimation.PushBack({ 32, 32, 32, 32 });
	}


	return true;
}

bool Item::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	currentAnimation = &fruitAnimation;
	
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircleSensor(position.x + 16, position.y + 16, 16, bodyType::STATIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ITEM;

	pbody->listener = this;
	
	disappear = false;

	return true;
}

bool Item::Update()
{
	// Removed these thing below
	//// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	//position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	//position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	if (disappear)
	{
		currentAnimation = &nothingAnimation;
		pbody->body->SetActive(false);
		pbody->body->SetTransform({ -10, -10 }, 0);
		isActive = false;
	}

	currentAnimation->Update();

	if (id == 1)
	{
		app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());
	}
	else if(id == 2)
	{
		if (disappear)
		{
			pbody->body->SetActive(false);
			pbody->body->SetTransform({ -10, -10 }, 0);
			isActive = false;
		}
		else
		{
			app->render->DrawTexture(texture, position.x, position.y, NULL);
		}

	}


	return true;
}

bool Item::CleanUp()
{
	return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physB == app->scene->player->pbody)
	{
		disappear = true;
		if (id == 1)
		{
			app->scene->player->playerScore += 10;
		}
		else if (id == 2)
		{
			if (app->scene->player->playerlives < 3)
			{
				app->scene->player->playerlives += 1;
			}			
			app->scene->player->playerScore += 100;
		}
	}
}

bool Item::LoadState(pugi::xml_node& data)
{
	isActive = data.child("item").attribute("isActive").as_bool();

	return true;
}

bool Item::SaveState(pugi::xml_node& data)
{
	pugi::xml_node itemNode = data.append_child("item");

	itemNode.append_attribute("isActive") = isActive;

	return true;
}