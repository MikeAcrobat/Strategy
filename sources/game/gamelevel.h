#pragma once

#include "entity.h"
#include <irrlicht.h>
#include "entitysystem.h"
#include "entityfactory.h"

class EventReceiver;

class Selector {
public:
	Selector(EntitySystem & enitites);

	void start_selection(irr::core::vector3df);
	void update_selection(irr::core::vector3df);
	void end_selection(irr::core::vector3df);

private:
	void update_selection_state();

	EntitySystem & enitites;
	irr::core::vector3df start;
	irr::core::vector3df end;
	bool selecting;
};

class Level {
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

	Selector m_selector;

	//GameObjectSystem m_objects_by_type;
};