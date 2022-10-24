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

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Item::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	width = 32;
	height = 32;

	// L07 TODO 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + width / 4 , position.y + height / 4, width / 4, bodyType::DYNAMIC);

	return true;
}

bool Item::Update()
{
	b2Transform transform = pbody->body->GetTransform();
	b2Vec2 pos = transform.p;

	position.x = METERS_TO_PIXELS(pos.x) - width / 2;
	position.y = METERS_TO_PIXELS(pos.y) - height / 2;

	// L07 TODO 4: Add a physics to an item - update the position of the object from the physics.  
	app->render->DrawTexture(texture, position.x , position.y);

	return true;
}

bool Item::CleanUp()
{
	return true;
}