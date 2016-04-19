#pragma once

#include "entity.h"
#include <irrlicht.h>
#include "entitysystem.h"
#include "entityfactory.h"

class EventReceiver;

class Level
{
public:

	Level(irr::IrrlichtDevice *, EventReceiver *);

	void update(float dt);

private:
	void handle_camera(const irr::SEvent &);
	void handle_game(const irr::SEvent &);

	void place_object(const std::string & name);

	EventReceiver * m_events;

	irr::IrrlichtDevice * m_device;
	irr::scene::ICameraSceneNode * m_camera;

	std::vector<Entity::Ptr> m_entities;

	EntitySystem m_entity_system;
	EntityFactory m_factory;

	//GameObjectSystem m_objects_by_type;
};