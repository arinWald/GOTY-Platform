#include "WalkEnemy.h"
#include "Player.h"
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
#include <cmath>

#include <iostream>
using namespace std;

#define DEATH_TIME 18;

WalkEnemy::WalkEnemy() : Entity(EntityType::WALKENEMY)
{
	
	name.Create("Enemy");

	//Animation pushbacks

	for (int i = 0; i < 14; ++i)
	{
		leftIdleAnimation.PushBack({ 32 * i, 40, 32, 32 });
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
		leftRunAnimation.PushBack({ 32*i, 74, 32, 32 });
	}
	leftRunAnimation.loop;
	leftRunAnimation.speed = 0.3f;

	for (int i = 0; i < 7; ++i)
	{
		disappearAnimation.PushBack({(64 * i), 214, 64, 64});
	}
	disappearAnimation.pingpong = false;
	disappearAnimation.speed = 0.3f;
	
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

	timerDeath = DEATH_TIME;
		
	pbody = app->physics->CreateRectangle(position.x, position.y, 20, 20, bodyType::DYNAMIC);
	/*pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 14, bodyType::DYNAMIC);*/
	pbody->body->SetGravityScale(1.5f);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;
	pbody->ctype = ColliderType::WALKENEMY;
	pbody->body->SetLinearVelocity(b2Vec2(0, -GRAVITY_Y));

	headBody = app->physics->CreateRectangleSensor(METERS_TO_PIXELS(pbody->body->GetTransform().p.x), METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 12, 5, 2, bodyType::STATIC);
	headBody->ctype = ColliderType::ENEMYHEAD;
	headBody->body->SetFixedRotation(true);

	currentMoveState = IDLE;
	direction = LEFT;
	
	return true;
	
}

bool WalkEnemy::Update()
{
	currentAnimation->Update();

	distanceFromPlayer = sqrt(pow(abs(app->scene->player->pbody->body->GetPosition().x - pbody->body->GetPosition().x), 2) +
		pow(abs(app->scene->player->pbody->body->GetPosition().y - pbody->body->GetPosition().y), 2));

	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);

	//TESTING ANIMATIONS
	/*if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
	{
		currentAnimation = &leftRunAnimation;
	}
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		currentAnimation = &leftHitAnimation;
	}
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		currentAnimation = &leftIdleAnimation;
	}
	if (app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
	{
		currentAnimation = &disappearAnimation;
	}*/

	// DEBUG SWITCH TO CHANGE MANUALLY MOVE STATES
	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		switch (debug_changeStateNum)
		{
		case IDLE:
			currentMoveState = CHASING;
			break;
		case CHASING:
			currentMoveState = IDLE;
			break;
		case GETTINGHURT:
			currentMoveState = DYING;
			break;
		case DYING:
			currentMoveState = IDLE;
			break;
		}
		debug_changeStateNum++;

		// Switch these two lines of code to switch from --> idle & chasing to --> all 4 possible states
		if (debug_changeStateNum > 1) debug_changeStateNum = 0;
		//if (debug_changeStateNum > 3) debug_changeStateNum = 0;

	}

	//Jump Test
	if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		pbody->body->ApplyLinearImpulse({ 0, -1 }, pbody->body->GetLocalCenter(), true);
	}
	float position_x = pbody->body->GetPosition().x;
	//All code related to each state
	switch (currentMoveState)
	{
	case IDLE:
		currentAnimation = &leftIdleAnimation;

		break;
	case CHASING:
		currentAnimation = &leftRunAnimation;

		switch (direction)
		{
		case RIGHT:
			if (objective.x + PIXEL_TO_METERS(app->map->mapData.tileWidth / 2) > pbody->body->GetPosition().x)
			{
				vel.x = speed;
				direction = Direction::RIGHT;
				currentAnimation = &leftRunAnimation;
			}
			break;
		case LEFT:
			if (objective.x + PIXEL_TO_METERS(app->map->mapData.tileWidth / 2) <= pbody->body->GetPosition().x)
			{
				vel.x = -speed;
				direction = Direction::LEFT;
				currentAnimation = &leftRunAnimation;
			}
			break;
		}

		if (app->physics->debug)//ray that is the PATH of the terrestre enemy
		{
			app->render->DrawLine(METERS_TO_PIXELS(pbody->body->GetPosition().x),
				METERS_TO_PIXELS(pbody->body->GetPosition().y),
				METERS_TO_PIXELS(app->scene->player->pbody->body->GetPosition().x),
				METERS_TO_PIXELS(app->scene->player->pbody->body->GetPosition().y),
				0, 255, 0); //green
		}

		break;
	case GETTINGHURT:
		currentAnimation = &leftHitAnimation;

		break;
	case DYING:
		currentAnimation = &disappearAnimation;
		isDead = true;
		cout << "DEAD " << endl;
		break;
	default:
		break;
	}

	if (pbody->body->GetPosition().x > app->scene->player->pbody->body->GetPosition().x)
	{
		direction = LEFT;
	}
	else direction = RIGHT;

	if (distanceFromPlayer <= 4)
	{
		ChangeMoveState(CHASING);
	}
	if (distanceFromPlayer > 4)
	{
		ChangeMoveState(IDLE);
	}

	cout << vel.x << endl;
	//cout << distanceFromPlayer << endl;

	//if (app->scene->gameplayState == app->scene->GameplayState::PLAYING)
	//{
	//	vel = b2Vec2(-speed, vel.y);
	//}

	pbody->body->SetLinearVelocity({ vel.x, pbody->body->GetLinearVelocity().y });

	//Manage Death Timer
	if (isDead)
	{
		if (timerDeath >= 0)
		{
			//cout << "IS DEAD ";
			currentAnimation = &disappearAnimation;
			--timerDeath;
		}
		else
		{
			app->entityManager->DestroyEntity(app->scene->walkEnemy);
			app->physics->world->DestroyBody(pbody->body);
			app->physics->world->DestroyBody(headBody->body);
			//timerDeath = DEATH_TIME;
		}
	}

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	headBodyPos.x = pbody->body->GetTransform().p.x;
	headBodyPos.y = pbody->body->GetTransform().p.y - PIXEL_TO_METERS(12);
	headBody->body->SetTransform({ headBodyPos.x, headBodyPos.y }, 0);

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
		//COllision with spikes
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
		case ColliderType::PLATFORMLIMIT:
			cout << "PLATFORM LIMIT HIT" << endl;

			break;
	}
}
void WalkEnemy::ChangePosition(int x, int y)
{

	teleport.posX = x;
	teleport.posY = y;
	teleport.turn = true;

}

void WalkEnemy::ChangeMoveState(MoveState newState)
{
	if (currentMoveState == newState) return;

	switch (newState)
	{
	case IDLE:
		currentMoveState = IDLE;
		break;
	case CHASING:
		currentMoveState = CHASING;
		break;
	case GETTINGHURT:
		currentMoveState = GETTINGHURT;
		break;
	case DYING:
		currentMoveState = DYING;
		break;
	}
}