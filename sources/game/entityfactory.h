#pragma once

#include "entity.h"
#include <map>
#include <memory>
#include <string>
#include "entitysystem.h"

#include "json/json-forwards.h"

namespace irr
{
	namespace scene
	{
		class ISceneManager;
	}
}

class EntityFactory
{
public:
	EntityFactory(EntitySystem & entity_system, irr::scene::ISceneManager * scene_manager);

	static void register_components();
	static bool register_templates(const std::string & filename);

	Entity::Ptr create_object(const std::string & name);

private:
	irr::scene::ISceneManager * m_scene_manager;
	EntitySystem & m_entity_system;
};