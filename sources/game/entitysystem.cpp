#include "entitysystem.h"

std::multimap<Component::Type, Entity::Ptr> EntitySystem::m_component_store;