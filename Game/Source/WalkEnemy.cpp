#include "WalkEnemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Map.h"
#include "Animation.h"
#include "EntityManager.h"
#include "Window.h"

#include <iostream>
using namespace std;

#define DEATH_TIME 40;

WalkEnemy::WalkEnemy() : Entity(EntityType::WALKENEMY)
{
	
		name.Create("Enemy");

		//Animation pushbacks

		for (int i = 0; i < 14; ++i)
		{
			leftIdleAnimation.PushBack({ 32 * i, 35, 32, 32 });
		}
		leftIdleAnimation.loop;
		leftIdleAnimation.speed = 0.3f;

		for (int i = 0; i < 5; ++i)
		{
			leftHitAnimation.PushBack({ 32 * i, 1, 32, 32 });
		}
		leftHitAnimation.speed = 0.3f;

		for (int i = 0; i < 16; ++i)
		{
			leftRunAnimation.PushBack({ 32*i, 68, 32, 32 });
		}
		leftRunAnimation.loop;
		leftRunAnimation.speed = 0.3f;

		/*for (int i = 0; i < 7; ++i)
		{
			dissappearAnimation.PushBack({(63 * i), 197, 63, 63});
		}
		dissappearAnimation.pingpong = true;
		dissappearAnimation.speed = 0.3f;*/
	
}

WalkEnemy::~WalkEnemy() {

}

bool WalkEnemy::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	speed = parameters.attribute("speed").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	deathFxPath = parameters.attribute("deathfxpath").as_string();
	
	return true;
}

bool WalkEnemy::Start() {

	//initilize textures
	enemyTexture = app->tex->Load(texturePath);
	transformEnemyPosition teleport;
	deathFxId = app->audio->LoadFx(deathFxPath);
	
	currentAnimation = &leftIdleAnimation;

	isDead = false;


	initialPosX = 40;
	initialPosY = 270;

	timerDeath = DEATH_TIME;
		
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 14, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::WALKENEMY;
	pbody->body->SetLinearVelocity(b2Vec2(0, -GRAVITY_Y));

	currentMoveState = IDLE;
	
	return true;
	
}

bool WalkEnemy::Update()
{
	
	currentAnimation->Update();

	//TESTING ANIMATIONS
	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		currentAnimation = &leftRunAnimation;
	}
	if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	{
		currentAnimation = &leftHitAnimation;
	}
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		currentAnimation = &leftIdleAnimation;
	}

	//Manage Death Timer
	if (isDead)
	{
		if (timerDeath >= 0)
		{
			//cout << "IS DEAD ";
			currentAnimation = &dissappearAnimation;
			--timerDeath;
		}
		else
		{
			isDead = false;
			timerDeath = DEATH_TIME;
		}
	}

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//PLAYER TELEPORT
	if (teleport.turn == true)
	{
		b2Vec2 resetPos = b2Vec2(PIXEL_TO_METERS(teleport.posX), PIXEL_TO_METERS(teleport.posY));
		pbody->body->SetTransform(resetPos, 0);

		teleport.turn = false;
	}

	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	//Death animation needs offset
	if (isDead)
	{
		app->render->DrawTexture(enemyTexture, position.x - 16, position.y - 16, &rect);
	}
	else
	{
		app->render->DrawTexture(enemyTexture, position.x, position.y, &rect);
		
	}

	return true;
}

bool WalkEnemy::CleanUp()
{
	return true;
}

void WalkEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
		case ColliderType::ITEM:
			break;
		case ColliderType::PLATFORM:
			break;
		case ColliderType::DEATH:
			LOG("Collision ENEMY DEATH");
			isDead = true;
			break;
		case ColliderType::GROUNDSENSOR:
			break;
		case ColliderType::WINSENSOR:
			break;
		case ColliderType::UNKNOWN:
			//LOG("Collision UNKNOWN");
			break;	
	}
}
void WalkEnemy::ChangePosition(int x, int y)
{

	teleport.posX = x;
	teleport.posY = y;
	teleport.turn = true;

}
