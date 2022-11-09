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

#include <iostream>
using namespace std;

#define DEATH_TIME 40;

Player::Player() : Entity(EntityType::PLAYER)
{
	
		name.Create("Player");

		//Animation pushbacks

		for (int i = 0; i < 11; ++i)
		{
			rightIdleAnimation.PushBack({ 32 * i, 31, 32, 32 });
		}
		rightIdleAnimation.loop;
		rightIdleAnimation.speed = 0.3f;

		for (int i = 0; i < 11; ++i)
		{
			rightRunAnimation.PushBack({ 32 * i, 62, 31, 32 });
		}
		rightRunAnimation.loop;
		rightRunAnimation.speed = 0.3f;

		rightJumpAnimation.PushBack({ 352, 31, 32, 32 });

		for (int i = 0; i < 6; ++i)
		{
			rightDoubleJumpAnimation.PushBack({ 32 * i, 0, 32, 32 });
		}
		rightDoubleJumpAnimation.loop;
		rightDoubleJumpAnimation.speed = 0.3f;

		for (int i = 0; i < 11; ++i)
		{
			leftIdleAnimation.PushBack({ 409 - (32 * i), 130, 32, 32 });
		}
		leftIdleAnimation.loop;
		leftIdleAnimation.speed = 0.3f;

		for (int i = 0; i < 11; ++i)
		{
			leftRunAnimation.PushBack({ 410 - (32 * i), 161, 31, 32 });
		}
		leftRunAnimation.loop;
		leftRunAnimation.speed = 0.3f;

		leftJumpAnimation.PushBack({ 58, 133, 32, 32 });

		for (int i = 0; i < 6; ++i)
		{
			leftDoubleJumpAnimation.PushBack({ 409 - (i * 32), 99, 32, 32 });
		}
		leftDoubleJumpAnimation.loop;
		leftDoubleJumpAnimation.speed = 0.3f;

		for (int i = 0; i < 7; ++i)
		{
			dissappearAnimation.PushBack({(63 * i), 197, 63, 63});
		}
		dissappearAnimation.pingpong = true;
		dissappearAnimation.speed = 0.3f;
	
}

Player::~Player() {

}

bool Player::Awake() {

		//L02: DONE 1: Initialize Player parameters
		//pos = position;
		//texturePath = "Assets/Textures/player/idle1.png";

		//L02: DONE 5: Get Player parameters from XML
		position.x = parameters.attribute("x").as_int();
		position.y = parameters.attribute("y").as_int();
		speed = parameters.attribute("speed").as_int();
		texturePath = parameters.attribute("texturepath").as_string();
		jumpFxPath = parameters.attribute("jumpfxpath").as_string();
		deathFxPath = parameters.attribute("deathfxpath").as_string();
		level1SongPath = parameters.attribute("level1songpath").as_string();
		playerlives = parameters.attribute("lives").as_int();
		jumpspeed = parameters.attribute("jumpspeed").as_int();
	
	return true;
}

bool Player::Start() {

	//initilize textures
	
		playerTexture = app->tex->Load(texturePath);
		transformPosition teleport;
		//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
		deathFxId = app->audio->LoadFx(deathFxPath);
		jumpFxId = app->audio->LoadFx(jumpFxPath);
		app->audio->PlayMusic(level1SongPath, 0);
		currentAnimation = &rightIdleAnimation;

		timerJump = 0;
		jumpspeed = -5.5;
		jumpsavailable = 2;

		LastDir = 1;

		timerDeath = DEATH_TIME;
		
		// L07 DONE 5: Add physics to the player - initialize physics body
		pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 14, bodyType::DYNAMIC);
		/*PhysBody* groundSensor = app->physics->CreateRectangle(position.x + 16, position.y + 26, 10, 16, bodyType::DYNAMIC);
		groundSensor->ctype = ColliderType::JUMPSENSOR;*/

		// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
		pbody->listener = this;

		// L07 DONE 7: Assign collider type
		pbody->ctype = ColliderType::PLAYER;
		pbody->body->SetLinearVelocity(b2Vec2(0, -GRAVITY_Y));
	
		return true;
	
}

bool Player::Update()
{
	
	// L07 DONE 5: Add physics to the player - updated player position using physics

		currentAnimation->Update();


		printf("PositionX: %d PositionY: %d\n", position.x, position.y);
		// L07 DONE 5: Add physics to the player - updated player position using physics

		if (isDead)
			cout << "IS DEAD";

		b2Vec2 vel ;
		if (!app->scene->godMode)
		{
			vel = b2Vec2(0, -GRAVITY_Y);
		}
		else
		{
			vel = b2Vec2(0, 0);
		}

		if (playerlives <= 0) {
			app->scene->FadeToNewState(app->scene->GAME_OVER_SCREEN);
			
		}
		
		if (LastDir == 1) {
			currentAnimation = &rightIdleAnimation;
		}
		else {
			currentAnimation = &leftIdleAnimation;
		}

		if (timerJump > 0) {
			timerJump--;
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				vel.x = speed;
				LastDir = 1;
			}
			else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {


				vel.x = -speed;
				LastDir = 2;
			}

			if (jumpsavailable == 1) {
				if (LastDir == 1) {
					currentAnimation = &rightJumpAnimation;
				}
				if (LastDir == 2) {
					currentAnimation = &leftJumpAnimation;
				}
			}
			else if (jumpsavailable == 0) {
				if (LastDir == 1) {
					currentAnimation = &rightDoubleJumpAnimation;
				}
				if (LastDir == 2) {
					currentAnimation = &leftDoubleJumpAnimation;
				}
			}

			vel = b2Vec2(vel.x, jumpspeed);

		}

		//if (app->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
		//{
		//	currentAnimation = &dissappearAnimation;
		//}

		//Manage Death Timer
		if (isDead)
		{
			if (timerDeath >= 0)
			{
				cout << "IS DEAD ";
				currentAnimation = &dissappearAnimation;
				--timerDeath;
			}
			else
			{
				isDead = false;
				timerDeath = DEATH_TIME;
				ChangePosition(40, 270);
			}
		}
		
		//PLAYER MOVE INPUT
		if (!app->scene->godMode)
		{
			//L02: DONE 4: modify the position of the player using arrow keys and render the texture
			if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && jumpsavailable > 0) {
				//
				timerJump = 15;
				app->audio->PlayFx(jumpFxId);
				jumpsavailable--;
				/*vel =  b2Vec2(vel.x,jumpspeed);*/
			}

			else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				//
			}

			else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && timerJump == 0) {
				vel = b2Vec2(-speed, vel.y);
				currentAnimation = &leftRunAnimation;
				LastDir = 2;
			}
			else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && timerJump == 0) {
				currentAnimation = &rightRunAnimation;
				LastDir = 1;
				vel = b2Vec2(speed, vel.y);
			}
		}
		else
		{
			//L02: DONE 4: modify the position of the player using arrow keys and render the texture
			if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)) {
				vel = b2Vec2(vel.x, -speed);
				currentAnimation = &leftJumpAnimation;
			}

			else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				vel = b2Vec2(vel.x, speed);
				currentAnimation = &leftJumpAnimation;
			}

			else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && timerJump == 0) {
				vel = b2Vec2(-speed, vel.y);
				currentAnimation = &leftRunAnimation;
				LastDir = 2;
			}
			else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && timerJump == 0) {
				currentAnimation = &rightRunAnimation;
				LastDir = 1;
				vel = b2Vec2(speed, vel.y);
			}
		}

		//Set the velocity of the pbody of the player
		pbody->body->SetLinearVelocity(vel);

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

		//app->render->DrawTexture(texture, position.x, position.y);

		SDL_Rect rect = currentAnimation->GetCurrentFrame();

		//Death animation needs offset
		if (isDead)
		{
			app->render->DrawTexture(playerTexture, position.x - 16, position.y - 16, &rect);
		}
		else
		{
			app->render->DrawTexture(playerTexture, position.x, position.y, &rect);
		}
	
	return true;
}

bool Player::CleanUp()
{
	return true;
}



// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision
	
	switch (physB->ctype)
	{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			app->audio->PlayFx(deathFxId);
			break;
		case ColliderType::PLATFORM:
			ground = true;
			if (timerJump > 0) { 
 				timerJump = 0; }
			jumpsavailable = 2;
			
			LOG("Collision PLATFORM");
			
			break;
		case ColliderType::DEATH:
			LOG("Collision DEATH");
			if (!app->scene->godMode)
			{
				app->audio->PlayFx(deathFxId);
				playerlives--;
			}
			isDead = true;
			break;
		case ColliderType::JUMPSENSOR:
			LOG("TOUCHING GROUND");
			cout << "Touching Ground";
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;	
	}
}
void Player::ChangePosition(int x, int y)
{

	teleport.posX = x;
	teleport.posY = y;
	teleport.turn = true;

}
