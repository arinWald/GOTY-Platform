//#include "Mushroom.h"
//#include "App.h"
//#include "Textures.h"
//#include "Audio.h"
//#include "Input.h"
//#include "Render.h"
//#include "Scene.h"
//#include "Log.h"
//#include "Point.h"
//#include "Physics.h"
//#include "EntityManager.h" 
//#include "Scene.h"
//#include "Window.h"
//
//#include "Box2D/Box2D/Box2D.h"
//
//#include "Map.h"
//#include "Pathfinding.h"
//#include "Defs.h"
//#include <cmath>
//#include <iostream>
//
//
//Mushroom::Mushroom() : Entity(EntityType::MUSHROOM)
//{
//	name.Create("Mushroom");
//}
//
//Mushroom::~Mushroom() {}
//
//bool Mushroom::Awake() {
//
//	position.x = parameters.attribute("x").as_int();
//	position.y = parameters.attribute("y").as_int();
//	scalarLimites = parameters.attribute("scalarLimites").as_int();
//	speedinX = parameters.attribute("speedX").as_int();
//
//	texturePath = parameters.attribute("texturepath").as_string();
//
//	walkDir = parameters.attribute("walkDir").as_bool();
//	attackE = parameters.attribute("attackE").as_bool();
//	animAtk = parameters.attribute("animAtk").as_bool();
//	//destroy = parameters.attribute("destroy").as_bool();
//	map = parameters.attribute("map").as_int();
//
//	return true;
//}
//
//bool Mushroom::Start() {
//
//
//
//	estadoSE1 = SENTRY;
//	direccionE = DERECHA;
//
//	limiteIzqX = position.x - scalarLimites;
//	limiteDerX = position.x + scalarLimites;
//
//	walkRAnimEnemy.PushBack({ 718,1297,24,21 });
//	walkRAnimEnemy.PushBack({ 676,1297,24,21 });
//	walkRAnimEnemy.PushBack({ 630,1297,24,21 });
//	walkRAnimEnemy.loop = true;
//	walkRAnimEnemy.speed = 0.125f;
//
//	walkLAnimEnemy.PushBack({ 776,1297,24,21 });
//	walkLAnimEnemy.PushBack({ 818,1297,24,21 });
//	walkLAnimEnemy.PushBack({ 864,1297,24,21 });
//	walkRAnimEnemy.loop = true;
//	walkLAnimEnemy.speed = 0.125f;
//
//	angryRAnimEnemy.PushBack({ 940,1331,24,21 });
//	angryRAnimEnemy.PushBack({ 777,1332,24,21 });
//	angryRAnimEnemy.PushBack({ 817,1332,24,21 });
//	angryRAnimEnemy.loop = false;
//	angryRAnimEnemy.speed = 0.05f;
//
//	angryLAnimEnemy.PushBack({ 940,1331,24,21 });
//	angryLAnimEnemy.PushBack({ 717,1332,24,21 });
//	angryLAnimEnemy.PushBack({ 676,1332,24,21 });
//	angryLAnimEnemy.loop = false;
//	angryLAnimEnemy.speed = 0.05f;
//
//	attackRAnimEnemy.PushBack({ 863,1327,24,26 });
//	attackRAnimEnemy.PushBack({ 905,1327,24,26 });
//	attackRAnimEnemy.loop = true;
//	attackRAnimEnemy.speed = 0.2f;
//
//	attackLAnimEnemy.PushBack({ 630,1327,24,26 });
//	attackLAnimEnemy.PushBack({ 588,1327,24,26 });
//	attackLAnimEnemy.loop = true;
//	attackLAnimEnemy.speed = 0.2f;
//
//
//	//initilize textures
//	texture = app->tex->Load(texturePath);
//
//	// L07 DONE 4: Add a physics  - initialize the physics body
//	pbody = app->physics->CreateCircle(position.x + 12, position.y + 12, 10, bodyType::DYNAMIC);
//
//	// L07 DONE 7: Assign collider type
//	pbody->ctype = ColliderType::ENEMY;
//
//	pbody->listener = this;
//
//	pbody->body->SetGravityScale(2.0f);
//	pbody->body->ResetMassData();
//
//	startPath = true;
//	nextFootStep = 0.0f;
//	amountToMoveInX = 0.0f;
//	initialPosition = { position.x / 64, position.y / 64 };
//	range = 5;
//	leftBorder = { position.x / 64, (position.y / 64) };
//	rightBorder = { initialPosition.x + range, (position.y / 64) };
//	firstPath = true;
//	achievedRightBorder = false;
//	achievedLeftBorder = true;
//	destination = 0.0f;
//	debug = false;
//	playerTileX = 0;
//	playerTileY = 0;
//	currentAnimationEnemy = &walkRAnimEnemy;
//	limitToChaseX = 0;
//	limitToChaseY = 0;
//	attackAnimation = false;
//
//	changedDataFromSave = false;
//	newData.startPath = startPath;
//	newData.nextFootStep = nextFootStep;
//	newData.amountToMoveInX = amountToMoveInX;
//	newData.destination = destination;
//	newData.firstPath = firstPath;
//	newData.achievedRightBorder = achievedRightBorder;
//	newData.achievedLeftBorder = achievedLeftBorder;
//	newData.attackAnimation = attackAnimation;
//	newData.animation = currentAnimationEnemy;
//
//	framesStopped = 0;
//	lastPosinX = 0;
//	lastPosinY = 0;
//
//	speedX = 3.0f;
//	speedLimit = 8.0f;
//
//	auxBug = false;
//	intBug = 0;
//
//	return true;
//}
//
////void Mushroom::ChaseMovement()
////{
////
////	if (!attackAnimation)
////	{
////		attackAnimTimer.Start(0.30);
////		attackAnimation = true;
////	}
////	
////	if (attackAnimTimer.Test() == EJECUTANDO)
////	{
////		currentAnimationEnemy = &angryRAnimEnemy;
////		return;
////	}
////		
////
////	iPoint playerPos = { app->scene->player->position.x / 32, app->scene->player->position.y / 32 };
////	iPoint myPos = { (int)std::round(nextFootStep / 64) , position.y / 64 };
////
////	if (changedDataFromSave)
////	{
////		myPos = { (int)std::round(newData.posX / 64) , newData.posY / 64 };
////		changedDataFromSave = false;
////	}
////
////	int ret = app->pathfinding->CreatePath(myPos, playerPos,"terrestre");
////
////	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
////
////	if (debug)
////	{
////		app->pathfinding->DrawLastPath();
////	}
////
////	if (path->At(1) != nullptr)
////	{
////		int aux = position.x;
////		destination = path->At(1)->x * 64;
////		nextFootStep = custom_lerp(position.x, destination, 0.075f);
////		amountToMoveInX = nextFootStep - aux;
////		startPath = false;
////	}
////
////	if (position.x > destination)
////	{
////		currentAnimationEnemy = &attackLAnimEnemy;
////	}
////	else if (position.x < destination)
////	{
////		currentAnimationEnemy = &attackRAnimEnemy;
////	}
////
////
////	b2Vec2 movePos = b2Vec2(PIXEL_TO_METERS(nextFootStep), PIXEL_TO_METERS(position.y));
////	pbody->body->SetTransform(movePos, 0);
////
////	nextFootStep += amountToMoveInX;
////
////	if (ret == -1)
////	{
//// 		estadoSE1 = RETURN;
////	}
////
////
////	//if (lastPosinX == position.x && lastPosinY == position.y)
////	//{
////	//	framesStopped++;
////	//	std::cout <<"framesStopped - " << framesStopped << std::endl;
////	//}
////
////	//lastPosinX = position.x;
////	//lastPosinY = position.y;
////
////	//if (framesStopped > 60)
////	//{
////	//	b2Vec2 vel = b2Vec2(0, -3.1);
////	//	//pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
////	//	pbody->body->ApplyLinearImpulse(vel, pbody->body->GetLocalCenter(), true);
////	//	framesStopped = 0;
////	//}
////
////}
//
//void Mushroom::ReturnMovement2()
//{
//	int ret = 0;
//	b2Vec2 vel = b2Vec2(0, 0);
//
//	if (position.y / 64 != leftBorder.y)
//	{
//		leftBorder = { position.x / 64, (position.y / 64) };
//		rightBorder = { leftBorder.x + range, (position.y / 64) };
//	}
//
//	attackAnimation = false;
//	iPoint myPos = { position.x / 64, position.y / 64 };
//
//
//	if (changedDataFromSave)
//	{
//		myPos = { (int)std::round(newData.posX / 64) , newData.posY / 64 };
//		changedDataFromSave = false;
//	}
//
//	ret = app->pathfinding->CreatePath(myPos, leftBorder, "terrestre");
//
//	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
//
//	if (debug)
//	{
//		app->pathfinding->DrawLastPath();
//	}
//
//	if (ret != -1)
//	{
//		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
//
//		if (debug)
//		{
//			app->pathfinding->DrawLastPath();
//		}
//
//		if (path->At(1) != nullptr)
//		{
//			if (path->At(1)->x * 64 > position.x)
//			{
//				vel = b2Vec2(speedX, 2.0f);
//			}
//			else if (path->At(1)->x * 64 < position.x)
//			{
//				vel = b2Vec2(-speedX, 2.0f);
//			}
//		}
//
//		pbody->body->SetLinearVelocity(vel);
//		if (path->At(1) != NULL)
//			destination = path->At(1)->x * 64;
//
//	}
//
//	if (position.x > destination)
//	{
//		currentAnimationEnemy = &walkLAnimEnemy;
//	}
//	else if (position.x < destination)
//	{
//		currentAnimationEnemy = &walkRAnimEnemy;
//	}
//
//}
//
////void Mushroom::ReturnMovement()
////{
////
////	if (position.y / 64 != leftBorder.y)
////	{
////		leftBorder = { position.x / 64, (position.y / 64) };
////		rightBorder = { leftBorder.x + range, (position.y / 64) };
////	}
////
////	attackAnimation = false;
////	if (startPath)
////	{
////		if (firstPath)
////		{
////			iPoint myPos = {position.x / 64, position.y / 64};
////			app->pathfinding->CreatePath(myPos, leftBorder, "terrestre");
////			firstPath = false;
////		}
////		else
////		{
////			iPoint myPos = { (int)std::round(nextFootStep / 64) , position.y / 64 };
////
////			if (changedDataFromSave)
////			{
////				myPos = { (int)std::round(newData.posX / 64) , newData.posY / 64 };
////				changedDataFromSave = false;
////			}
////
////
////			app->pathfinding->CreatePath(myPos, leftBorder, "terrestre");
////		}
////	}
////
////	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
////
////	if (debug)
////	{
////		app->pathfinding->DrawLastPath();
////	}
////
////	if (startPath)
////	{
////
////		if (path->At(1) != nullptr)
////		{
////			int aux = position.x;
////			destination = path->At(1)->x * 64;
////			nextFootStep = custom_lerp(position.x, destination, 0.05f);
////			amountToMoveInX = nextFootStep - aux;
////			startPath = false;
////		}
////
////	}
////
////	if (position.x > destination)
////	{
////		currentAnimationEnemy = &walkLAnimEnemy;
////	}
////	else if (position.x < destination)
////	{
////		currentAnimationEnemy = &walkRAnimEnemy;
////	}
////
////	b2Vec2 movePos = b2Vec2(PIXEL_TO_METERS(nextFootStep), PIXEL_TO_METERS(position.y));
////	pbody->body->SetTransform(movePos, 0);
////
////	nextFootStep += amountToMoveInX;
////
////	if (position.x > destination)
////	{
////		startPath = true;
////	}
////	else if (position.x < destination)
////	{
////		startPath = true;
////	}
////}
//
////void Mushroom::SentryMovement()
////{
////	int ret = 0;
////	if (position.y / 64 != leftBorder.y)
////	{
////		leftBorder = { position.x / 64, (position.y / 64) };
////		rightBorder = { leftBorder.x + range, (position.y / 64) };
////	}
////
////	if (startPath)
////	{
////		if (firstPath)
////		{
////			ret = app->pathfinding->CreatePath(leftBorder, rightBorder, "terrestre");
////			if (ret == -1) //AKAAAAAAAAAAAAAAAAAA
////			{
////				while (ret == -1)
////				{
////					leftBorder.x = leftBorder.x - 1;
////					rightBorder.x = rightBorder.x - 1;
////					ret = app->pathfinding->CreatePath(leftBorder, rightBorder, "terrestre");
////				}
////			}
////			firstPath = false;
////		}
////		else
////		{
////			if (achievedLeftBorder)
////			{
////				iPoint myPos = { (int)std::round(nextFootStep / 64) , position.y / 64 };
////				ret = app->pathfinding->CreatePath(myPos, rightBorder, "terrestre");
////				currentAnimationEnemy = &walkRAnimEnemy;
////
////				if (changedDataFromSave)
////				{
////					myPos = { (int)std::round(newData.posX / 64) , newData.posY / 64 };
////					ret = app->pathfinding->CreatePath(myPos, rightBorder, "terrestre");
////					changedDataFromSave = false;
////				}
////
////				if (ret == -1) //AKAAAAAAAAAAAAAAAAAA
////				{
////					while (ret == -1)
////					{
////						rightBorder.x = rightBorder.x - 1;
////						ret = app->pathfinding->CreatePath(myPos, rightBorder, "terrestre");
////					}
////				}
////
////			}
////			else if (achievedRightBorder)
////			{
////				iPoint myPos = { (int)std::round(nextFootStep / 64) , position.y / 64 };
////				ret = app->pathfinding->CreatePath(myPos, leftBorder, "terrestre");
////				currentAnimationEnemy = &walkLAnimEnemy;
////
////				if (changedDataFromSave)
////				{
////					myPos = { (int)std::round(newData.posX / 64) , newData.posY / 64 };
////					ret =  app->pathfinding->CreatePath(myPos, leftBorder, "terrestre");
////					changedDataFromSave = false;
////				}
////
////				while (ret == -1)
////				{
////					leftBorder.x = leftBorder.x - 1;
////					ret = app->pathfinding->CreatePath(myPos, leftBorder, "terrestre");
////				}
////
////			}
////		}
////	}
////
////	//if (ret == -1)
////	//{
////	//	int a = 4;
////	//}
////
////	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
////
////	if (debug)
////	{
////		app->pathfinding->DrawLastPath();
////	}
////	
////
////	if (startPath)
////	{
////		
////		if (path->At(1) != nullptr)
////		{
////			int aux = position.x;
////			destination = path->At(1)->x * 64;
////			nextFootStep = custom_lerp(position.x, destination, 0.05f);
////			amountToMoveInX = nextFootStep - aux;
////			startPath = false;
////		}
////		
////	}
////
////	b2Vec2 movePos = b2Vec2(PIXEL_TO_METERS(nextFootStep), PIXEL_TO_METERS(position.y));
////	pbody->body->SetTransform(movePos, 0);
////
////	nextFootStep += amountToMoveInX;
////
////	if (nextFootStep > destination && achievedLeftBorder == true)
////	{
////		startPath = true;
////	} 
////	else if (nextFootStep < destination && achievedRightBorder == true)
////	{
////		startPath = true;
////	}
////	
////	if (position.x / 64 > rightBorder.x-1)
////	{
////		achievedRightBorder = true;
////		achievedLeftBorder = false;
////	}
////	else if (position.x / 64 < leftBorder.x)
////	{
////		achievedLeftBorder = true;
////		achievedRightBorder = false;
////	}
////}
//
//void Mushroom::ChaseMovement2()
//{
//
//	int ret = 0;
//	iPoint playerPos = { app->scene->player->position.x / 32, app->scene->player->position.y / 32 };
//	iPoint myPos = { (int)std::round(position.x / 64) , (int)std::round(position.y / 64) };
//
//	b2Vec2 vel = b2Vec2(0, 0);
//
//	if (!attackAnimation)
//	{
//		attackAnimTimer.Start(0.30f);
//		attackAnimation = true;
//	}
//
//	if (attackAnimTimer.Test() == EJECUTANDO)
//	{
//		currentAnimationEnemy = &angryRAnimEnemy;
//		return;
//	}
//
//
//	if (changedDataFromSave)
//	{
//		myPos = { (int)std::round(newData.posX / 64) , newData.posY / 64 };
//		changedDataFromSave = false;
//	}
//
//	ret = app->pathfinding->CreatePath(myPos, playerPos, "terrestre");
//
//	if (ret != -1)
//	{
//		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
//
//
//
//		if (debug)
//		{
//			app->pathfinding->DrawLastPath();
//		}
//
//		if (path->At(1) != nullptr)
//		{
//
//			if (path->At(1)->x * 32 > app->scene->player->position.x)
//			{
//				vel = b2Vec2(-speedX, 0);
//			}
//			else if (path->At(1)->x * 32 < app->scene->player->position.x)
//			{
//				vel = b2Vec2(speedX, 0);
//			}
//
//			pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
//			//std::cout << "linear velocity - " << pbody->body->GetLinearVelocity().x << std::endl;
//
//			if (pbody->body->GetLinearVelocity().x > speedLimit)
//			{
//				b2Vec2 vel = pbody->body->GetLinearVelocity();
//				vel.x = speedLimit;
//				pbody->body->SetLinearVelocity(vel);
//			}
//			else if (pbody->body->GetLinearVelocity().x < -speedLimit)
//			{
//				b2Vec2 vel = pbody->body->GetLinearVelocity();
//				vel.x = -speedLimit;
//				pbody->body->SetLinearVelocity(vel);
//			}
//
//			//pbody->body->SetLinearVelocity(vel);
//
//			destination = path->At(1)->x * 64;
//			startPath = false;
//		}
//
//
//		if (position.x > destination)
//		{
//			currentAnimationEnemy = &attackLAnimEnemy;
//		}
//		else if (position.x < destination)
//		{
//			currentAnimationEnemy = &attackRAnimEnemy;
//		}
//
//	}
//
//	if (lastPosinX == position.x && lastPosinY == position.y)
//	{
//		framesStopped++;
//
//	}
//
//	lastPosinX = position.x;
//	lastPosinY = position.y;
//
//	if (framesStopped > 60)
//	{
//		b2Vec2 vel = b2Vec2(0, -2);
//		//pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
//		pbody->body->ApplyLinearImpulse(vel, pbody->body->GetLocalCenter(), true);
//		std::cout << "impulso linear vertical aplicado " << std::endl;
//		framesStopped = 0;
//	}
//
//
//
//
//}
//
//void Mushroom::SentryMovement2()
//{
//	int ret = 0;
//	b2Vec2 vel = b2Vec2(0, 0);
//	iPoint myPos = { 0,0 };
//
//	if (position.y / 64 != leftBorder.y)
//	{
//		leftBorder = { position.x / 64, (position.y / 64) };
//		rightBorder = { leftBorder.x + range, (position.y / 64) };
//	}
//
//	if (startPath)
//	{
//		if (firstPath)
//		{
//			ret = app->pathfinding->CreatePath(leftBorder, rightBorder, "terrestre");
//			firstPath = false;
//			if (ret == -1)
//			{
//				while (ret == -1)
//				{
//					leftBorder.x = leftBorder.x - 1;
//					rightBorder.x = rightBorder.x - 1;
//					ret = app->pathfinding->CreatePath(leftBorder, rightBorder, "terrestre");
//				}
//			}
//		}
//		else
//		{
//			if (achievedLeftBorder)
//			{
//				myPos = { position.x / 64 , position.y / 64 };
//				ret = app->pathfinding->CreatePath(myPos, rightBorder, "terrestre");
//				currentAnimationEnemy = &walkLAnimEnemy;
//
//				if (changedDataFromSave)
//				{
//					myPos = { (int)std::round(newData.posX / 64) , newData.posY / 64 };
//					ret = app->pathfinding->CreatePath(myPos, rightBorder, "terrestre");
//					changedDataFromSave = false;
//				}
//
//				if (ret == -1)
//				{
//					while (ret == -1)
//					{
//						rightBorder.x = rightBorder.x - 1;
//						ret = app->pathfinding->CreatePath(myPos, rightBorder, "terrestre");
//					}
//				}
//
//			}
//			else if (achievedRightBorder)
//			{
//				myPos = { position.x / 64 , position.y / 64 };
//				ret = app->pathfinding->CreatePath(myPos, leftBorder, "terrestre");
//				currentAnimationEnemy = &walkRAnimEnemy;
//
//				if (changedDataFromSave)
//				{
//					myPos = { (int)std::round(newData.posX / 64) , newData.posY / 64 };
//					ret = app->pathfinding->CreatePath(myPos, leftBorder, "terrestre");
//					changedDataFromSave = false;
//				}
//
//				if (ret == -1)
//				{
//					while (ret == -1)
//					{
//						leftBorder.x = leftBorder.x - 1;
//						ret = app->pathfinding->CreatePath(myPos, leftBorder, "terrestre");
//					}
//				}
//
//			}
//		}
//	}
//
//	if (ret != -1)
//	{
//		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
//
//		if (debug)
//		{
//			app->pathfinding->DrawLastPath();
//		}
//
//		if (path->At(1) != nullptr)
//		{
//			if (path->At(1)->x * 64 > position.x)
//			{
//				vel = b2Vec2(speedX, 5.0f);
//			}
//			else if (path->At(1)->x * 64 < position.x)
//			{
//				vel = b2Vec2(-speedX, 5.0f);
//			}
//		}
//
//		pbody->body->SetLinearVelocity(vel);
//		if (path->At(1) != NULL)
//			destination = path->At(1)->x * 64;
//
//	}
//
//	if (pbody->body->GetLinearVelocity().x > speedLimit)
//	{
//		b2Vec2 vel = pbody->body->GetLinearVelocity();
//		vel.x = speedLimit;
//		pbody->body->SetLinearVelocity(vel);
//	}
//	else if (pbody->body->GetLinearVelocity().x < -speedLimit)
//	{
//		b2Vec2 vel = pbody->body->GetLinearVelocity();
//		vel.x = -speedLimit;
//		pbody->body->SetLinearVelocity(vel);
//	}
//
//	if (position.x > destination)
//	{
//		currentAnimationEnemy = &walkLAnimEnemy;
//	}
//	else if (position.x < destination)
//	{
//		currentAnimationEnemy = &walkRAnimEnemy;
//	}
//
//	if (achievedLeftBorder)
//	{
//		int limitToChange = std::abs(position.x / 64 - rightBorder.x);
//
//		if (limitToChange == 0)
//		{
//			achievedRightBorder = true;
//			achievedLeftBorder = false;
//		}
//	}
//	else if (achievedRightBorder)
//	{
//		int limitToChange = std::abs(position.x / 64 - leftBorder.x);
//		if (limitToChange == 0)
//		{
//			achievedLeftBorder = true;
//			achievedRightBorder = false;
//		}
//	}
//
//	if (lastPosinX == position.x && lastPosinY == position.y)
//	{
//		framesStopped++;
//
//	}
//
//	lastPosinX = position.x;
//	lastPosinY = position.y;
//
//	if (framesStopped > 60)
//	{
//		b2Vec2 vel = b2Vec2(0, -2);
//		//pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
//		pbody->body->ApplyLinearImpulse(vel, pbody->body->GetLocalCenter(), true);
//		std::cout << "impulso linear vertical aplicado " << std::endl;
//		framesStopped = 0;
//	}
//
//}
//
//bool Mushroom::Update()
//{
//
//	// L07 DONE 4: Add a physics  - update the position of the object from the physics.  
//	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
//	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);
//
//	//
//	//iPoint playerPos = { app->scene->player->position.x / 32, app->scene->player->position.y / 32 };
//	//iPoint myPos = { position.x / 64 , position.y / 64 };
//	//iPoint aux = { myPos.x + 3, myPos.y };
//
//	switch (estadoSE1) {
//	case STOP:
//		break;
//	case SENTRY:
//		SentryMovement2();
//
//		/*	playerTileX = app->scene->player->position.x / 32;
//			playerTileY = app->scene->player->position.y / 32;
//			if (playerTileX > leftBorder.x &&
//				playerTileX < rightBorder.x &&
//				playerTileY > (position.y / 64) - 2 &&
//				playerTileY <(position.y / 64) + 2)
//			{
//				estadoSE1 = CHASE;
//			}*/
//
//		playerTileX = app->scene->player->position.x / 32;
//		playerTileY = app->scene->player->position.y / 32;
//		limitToChaseX = std::abs(playerTileX - (position.x / 64));
//		limitToChaseY = std::abs(playerTileY - (position.y / 64));
//
//		if (limitToChaseX < 5 && limitToChaseY < 4)
//		{
//			estadoSE1 = CHASE;
//		}
//
//
//		break;
//	case CHASE:
//		ChaseMovement2();
//
//		playerTileX = app->scene->player->position.x / 32;
//		limitToChaseX = std::abs(playerTileX - (position.x / 64));
//
//
//		if (limitToChaseX > 4)
//		{
//			estadoSE1 = RETURN;
//			startPath = true;
//			firstPath = true;
//			achievedRightBorder = false;
//			achievedLeftBorder = true;
//
//		}
//
//		break;
//
//	case RETURN:
//		ReturnMovement2();
//
//		if (position.x / 64 == leftBorder.x)
//		{
//			estadoSE1 = SENTRY;
//			startPath = true;
//			firstPath = true;
//			achievedRightBorder = false;
//			achievedLeftBorder = true;
//		}
//
//		playerTileX = app->scene->player->position.x / 32;
//		limitToChaseX = std::abs(playerTileX - (position.x / 64));
//		limitToChaseY = std::abs(playerTileY - (position.y / 64));
//
//		if (limitToChaseX < 5 && limitToChaseY < 4)
//		{
//			estadoSE1 = CHASE;
//		}
//
//		break;
//
//	default:
//		break;
//	}
//
//	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
//	{
//		if (!debug)
//		{
//			debug = true;
//		}
//		else
//		{
//			debug = false;
//		}
//	}
//
//	currentAnimationEnemy->Update();
//	SDL_Rect rect = currentAnimationEnemy->GetCurrentFrame();
//	app->render->DrawTexture(texture, position.x / app->win->GetScale() - 10, position.y / app->win->GetScale() - 7, &rect);
//
//	if (position.y > 1856 ||
//		position.x < 0 ||
//		position.x > 15936 ||
//		position.y < 0)
//	{
//		destroy = true;
//	}
//
//	if (destroy)
//	{
//		pbody->body->GetWorld()->DestroyBody(pbody->body);
//		CleanUp();
//		destroy = false;
//	}
//
//	return true;
//}
//
//
//bool Mushroom::CleanUp()
//{
//	//memoryleak
//
//	app->tex->UnLoad(texture);
//	active = false;
//
//	return true;
//}
//
//void Mushroom::OnCollision(PhysBody* physA, PhysBody* physB)
//{
//
//	switch (physB->ctype)
//	{
//	case ColliderType::PLAYER:
//
//		destroy = true;
//
//		break;
//	}
//}
//
//void Mushroom::LoadInfo(iPoint pos, int state)
//{
//
//	newData.posX = pos.x;
//	newData.posY = pos.y;
//	newData.estado = state;
//	estadoSE1 = state;
//	changedDataFromSave = true;
//
//
//	b2Vec2 movePos = b2Vec2(PIXEL_TO_METERS(newData.posX), PIXEL_TO_METERS(newData.posY));
//	pbody->body->SetTransform(movePos, 0);
//
//	startPath = newData.startPath;
//	nextFootStep = newData.nextFootStep;
//	amountToMoveInX = newData.amountToMoveInX;
//	destination = newData.destination;
//	firstPath = newData.firstPath;
//	achievedRightBorder = newData.achievedRightBorder;
//	achievedLeftBorder = newData.achievedLeftBorder;
//	attackAnimation = newData.attackAnimation;
//
//	if (newData.animation != NULL)
//		currentAnimationEnemy = newData.animation;
//
//}
//
//void Mushroom::SaveInfo()
//{
//
//	newData.startPath = startPath;
//	newData.nextFootStep = nextFootStep;
//	newData.amountToMoveInX = amountToMoveInX;
//	newData.destination = destination;
//	newData.firstPath = firstPath;
//	newData.achievedRightBorder = achievedRightBorder;
//	newData.achievedLeftBorder = achievedLeftBorder;
//	newData.attackAnimation = attackAnimation;
//	newData.animation = currentAnimationEnemy;
//}
//
//int Mushroom::GetState()
//{
//	return estadoSE1;
//}