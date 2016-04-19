#include "entity.h"

Component::Ptr Entity::get_component(Component::Type type) const {
	if (m_components.find(type) != m_components.end()) {
		return m_components.at(type);
	}

	return nullptr;
}

void EntityHelpers::set_position(Entity::Ptr entity, const irr::core::vector3df & position) {
	if (!entity) return;

	TransformComponent::Ptr transform = entity->get_component<TransformComponent>();
	if (transform) {
		transform->position = position;
	}

	RenderComponent::Ptr render = entity->get_component<RenderComponent>();
	if (render && render->node) {
		render->node->setPosition(position);
	}
}

void EntityHelpers::initialize_components(Entity::Ptr entity, irr::scene::ISceneManager * manager)
{
	if (!entity) return;

	RenderComponent::Ptr render = entity->get_component<RenderComponent>();
	if (render) {
		irr::scene::IAnimatedMesh * mesh = manager->getMesh(render->mesh_name.c_str());
		if (mesh) {
			render->node = manager->addMeshSceneNode(mesh);
			render->node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		}
	}
}
