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


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	//Animation pushbacks
	
	for (int i = 0; i < 11; ++i)
	{
		rightIdleAnimation.PushBack({ 32*i, 32, 32, 32});
	}
	rightIdleAnimation.loop;
	rightIdleAnimation.speed = 0.3f;

	for (int i = 0; i < 11; ++i)
	{
		rightRunAnimation.PushBack({ 32 * i, 64, 32, 32 });
	}
	rightRunAnimation.loop;
	rightRunAnimation.speed = 0.3f;

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

	return true;
}

bool Player::Start() {

	//initilize textures
	playerTexture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 14, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this; 

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;
	pbody->body->SetLinearVelocity(b2Vec2(0, -GRAVITY_Y));

	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/coinPickup.ogg");
	jumpFxId = app->audio->LoadFx("Assets/Audio/Fx/jump.wav");

	currentAnimation = &rightIdleAnimation;

	int timerPocho = 0;
	jumpspeed = -5.5;
	jumpsavailable = 2;
	
	return true;
}

bool Player::Update()
{
	
	// L07 DONE 5: Add physics to the player - updated player position using physics

	currentAnimation->Update();

	
	printf("PositionX: %d PositionY: %d\n", position.x, position.y);
	// L07 DONE 5: Add physics to the player - updated player position using physics

	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);

	if (timerPocho > 0) {
		timerPocho--;
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			vel.x = speed;
		}
		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			vel.x = -speed;
		}
		
			vel = b2Vec2(vel.x, jumpspeed);
		
	}
	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	 if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && jumpsavailable >0) {
		//
		 timerPocho = 15;
		 app->audio->PlayFx(jumpFxId);
		 jumpsavailable--;
		/*vel =  b2Vec2(vel.x,jumpspeed);*/
	}
	 
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}

	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel =  b2Vec2(-speed, vel.y);
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		currentAnimation = &rightRunAnimation;
		vel = b2Vec2(speed, vel.y);
	}
	else currentAnimation = &rightIdleAnimation;


	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//app->render->DrawTexture(texture, position.x, position.y);

	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	app->render->DrawTexture(playerTexture, position.x, position.y, &rect);


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
			app->audio->PlayFx(pickCoinFxId);
			break;
		case ColliderType::PLATFORM:
			ground = true;
			jumpsavailable = 2;
			LOG("Collision PLATFORM");
			
			break;
		case ColliderType::DEATH:
			LOG("Collision DEATH");
			app->audio->PlayFx(pickCoinFxId);
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		
			
				
	}
	



}
