#include "CollisionController.h"

#include <cmath>
#include <glm/glm.hpp>

//--------------Проверка и вызов функций----------//
void CollisionController::CheckCollision(Entity& entity1, Entity& entity2) {
    if (Character* character1 = dynamic_cast<Character*>(&entity1)) {
        if (Character* character2 = dynamic_cast<Character*>(&entity2)) { Help(*character1, *character2); }
        else if (Projectile* proj2 = dynamic_cast<Projectile*>(&entity2)) { Help(*character1, *proj2); }
        else { Help(entity1, *character1); }
    }

    else if (Projectile* proj1 = dynamic_cast<Projectile*>(&entity1)) {
        if (Character* character2 = dynamic_cast<Character*>(&entity2)) { Help(*character2, *proj1); }
        else if (Projectile* proj2 = dynamic_cast<Projectile*>(&entity2)) { Help(*proj1, *proj2); }
        else { Help(entity1, *proj1); }
    }

    else {
        if (Character* character2 = dynamic_cast<Character*>(&entity2)) { Help(entity1, *character2); }
        else if (Projectile* proj2 = dynamic_cast<Projectile*>(&entity2)) { Help(entity1, *proj2); }
        else { Help(entity1, entity2); }
    }
}

void CollisionController::CheckCollision(Entity& entity, Collider& collider){
    if (Character* character = dynamic_cast<Character*>(&entity)) { Help(*character, collider); }
    else if (Projectile* proj = dynamic_cast<Projectile*>(&entity)) { Help(*proj, collider); }
    else { Help(entity, collider); }
}

void CollisionController::CheckCollision(Entity& entity, Object& object) {
    if (Character* character = dynamic_cast<Character*>(&entity)) { Help(*character, object); }
    else if (Projectile* proj = dynamic_cast<Projectile*>(&entity)) { Help(*proj, object); }
    else { Help(entity, object); }
}

void CollisionController::CheckCollision(Entity& entity, Trigger& trigger) {
    if (Character* character = dynamic_cast<Character*>(&entity)) { Help(*character, trigger); }
    else if (Projectile* proj = dynamic_cast<Projectile*>(&entity)) { Help(*proj, trigger); }
    else { Help(entity, trigger); }
}
//--------------Проверка и вызов функций----------//

//--------------Функции для коллизий----------//
void CollisionController::Help(Entity& entity1, Entity& entity2){
    EPAResult result = entity1.CheckCollision(entity2);
    if (result.hasCollision) {
        float ration = entity1.m_entityData.Mass / (entity1.m_entityData.Mass + entity2.m_entityData.Mass);

        entity1.m_position -= result.penetrationVector * (1 - ration);
        entity2.m_position += result.penetrationVector * ration;

        glm::vec2 normal = glm::normalize(result.penetrationVector);
        float resultDot = glm::dot(entity2.m_CurrentSpeed - entity1.m_CurrentSpeed, normal);

        if (resultDot <= 0) {
            float e = (entity1.m_entityData.Elasticity + entity2.m_entityData.Elasticity) / 2.f;

            glm::vec2 impulse = -(1 + e) * resultDot * normal / (1 / entity1.m_entityData.Mass + 1 / entity2.m_entityData.Mass);
            entity1.AddImpulse(-impulse);
            entity2.AddImpulse(impulse);
        }
    }
}

void CollisionController::Help(Entity& entity, Character& character){
    Help(entity, static_cast<Entity&>(character));
}

void CollisionController::Help(Entity& entity, Projectile& projectile){
    if (!projectile.m_DataProjectire.m_physics || projectile.BounceIsOver()) {
        projectile.breakageCollision(entity);
    }
    else {
        Help(entity, static_cast<Entity&>(projectile));
    }
}

void CollisionController::Help(Character& character1, Character& character2){
    Help(static_cast<Entity&>(character1), static_cast<Entity&>(character2));
}

void CollisionController::Help(Character& character, Projectile& projectile){
    if (projectile.m_team == projectile.m_team) return;

    EPAResult result = projectile.CheckCollision(character);
    if (result.hasCollision && projectile.m_DataProjectire.m_collisionLengthForBreakage < glm::length(result.penetrationVector)) {
        character.addDamage(2.f);
    }
}

void CollisionController::Help(Projectile& projectile1, Projectile& projectile2){
    if (projectile1.m_DataProjectire.m_physics && projectile2.m_DataProjectire.m_physics) {
        Help(static_cast<Entity&>(projectile1), static_cast<Entity&>(projectile2));
    }
}

void CollisionController::Help(Entity& entity, Collider& collider){
    EPAResult result = entity.CheckCollision(collider);
    if (result.hasCollision) {
        entity.m_position -= result.penetrationVector;
    }
}

void CollisionController::Help(Character& character, Collider& collider){
    Help(static_cast<Entity&>(character), collider);
}

void CollisionController::Help(Projectile& projectile, Collider& collider){
    if (!projectile.m_DataProjectire.m_physics || projectile.BounceIsOver()) {
        projectile.breakageCollision(collider);
    }
    else {
        projectile.physicsCollision(collider);
    }
}

void CollisionController::Help(Entity& entity, Object& object){
    EPAResult result = entity.CheckCollision(object);
    if (result.hasCollision) {
        entity.m_position -= result.penetrationVector;

        float Speed = glm::length(entity.m_CurrentSpeed);
        if (Speed > 0.0001f) {
            glm::vec2 reflect = glm::reflect(glm::normalize(entity.m_CurrentSpeed), glm::normalize(result.penetrationVector));
            float e = (entity.m_entityData.Elasticity + object.Data().Elasticity) / 2.f;
            entity.m_CurrentSpeed = Speed * (1.f - e) * reflect;
        }
    }
}

void CollisionController::Help(Character& character, Object& object){
    Help(static_cast<Entity&>(character), object);
}

void CollisionController::Help(Projectile& projectile, Object& object){
    if (!projectile.m_DataProjectire.m_physics || projectile.BounceIsOver()) {
        projectile.breakageCollision(object);
    }
    else {
        projectile.physicsCollision(object);
    }
}

void CollisionController::Help(Entity& entity, Trigger& trigger){
    if (entity.CheckCollision(trigger, ONLY_COLLISION).hasCollision) {
        trigger.run(entity);
    }
}

void CollisionController::Help(Character& character, Trigger& trigger){
    Help(static_cast<Entity&>(character), trigger);
}

void CollisionController::Help(Projectile& projectile, Trigger& trigger){
}
//--------------Функции для коллизий----------//
