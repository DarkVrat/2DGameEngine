#pragma once

#include "../Objects/Projectile.h"
#include "../Objects/Character.h"
#include "../Objects/Collider.h"
#include "../Objects/Object.h"
#include "../Objects/Trigger.h"

class CollisionController {
public:
	static void CheckCollision(Entity& entity1, Entity& entity2);
	static void CheckCollision(Entity& entity, Collider& collider);
	static void CheckCollision(Entity& entity, Object& object);
	static void CheckCollision(Entity& entity, Trigger& trigger);

private:
	static void Help(Entity& entity1, Entity& entity2);
	static void Help(Entity& entity, Character& character);
	static void Help(Entity& entity, Projectile& projectile);
	static void Help(Character& character1, Character& character2);
	static void Help(Character& character, Projectile& projectile);
	static void Help(Projectile& projectile1, Projectile& projectile2);

	static void Help(Entity& entity, Collider& collider);
	static void Help(Character& character, Collider& collider);
	static void Help(Projectile& projectile, Collider& collider);
	static void Help(Entity& entity, Object& object);
	static void Help(Character& character, Object& object);
	static void Help(Projectile& projectile, Object& object);
	static void Help(Entity& entity, Trigger& trigger);
	static void Help(Character& character, Trigger& trigger);
	static void Help(Projectile& projectile, Trigger& trigger);

};