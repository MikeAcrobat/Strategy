#pragma once
#include "entity.h"
#include "entitysystem.h"

class Action {
public:
	virtual bool update(Entity::Ptr entity, float dt) = 0;

	typedef std::shared_ptr<Action> Ptr;
};

class Walk : public Action {
public:
	Walk(irr::core::vector3df target);
	virtual bool update(Entity::Ptr entity, float dt);
protected:
	irr::core::vector3df target;
};

class Collect : public Action {
public:
	Collect(Entity::Ptr resource);
	virtual bool update(Entity::Ptr entity, float dt);
protected:
	Entity::Weak resource;
	float cooldown;
};

class Deposit : public Action {
public:
	virtual bool update(Entity::Ptr entity, float dt);
};