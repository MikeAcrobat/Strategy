#pragma once

#include <vector>
#include "entity.h"
#include "entityfactory.h"

class Process {
public:
	virtual void update(std::vector<Entity::Ptr> &entities, float dt) = 0;
};

class UpdateActions : public Process {
public:
	virtual void update(std::vector<Entity::Ptr> &entities, float dt);
};

/*
class ProductionProcess : public Process {
public:
	ProductionProcess(std::vector<Entity::Ptr> & storage, EntityFactory & factory);
	virtual void update(std::vector<Entity::Ptr> &entities, float dt);
private:
	EntityFactory & m_factory;
	std::vector<Entity::Ptr> & m_storage;
};
*/