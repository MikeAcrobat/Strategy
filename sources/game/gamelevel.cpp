#include "gamelevel.h"
#include "inputmanager.h"

#include "entityfactory.h"

#include "systems/gamesystem.h"

static Entity::Ptr ccc;

Level::Level(irr::IrrlichtDevice * mgr, EventReceiver * events) :
	m_device(mgr), m_events(events), m_factory(m_entity_system, m_device->getSceneManager())
{

	auto * smgr = m_device->getSceneManager();
	auto * video = m_device->getVideoDriver();

	video->getMaterial2D().setFlag(irr::video::EMF_LIGHTING, false);

	const float height = 200;

	m_camera = smgr->addCameraSceneNode();
	m_camera->setPosition(irr::core::vector3df(0, height, -60));
	m_camera->setTarget(irr::core::vector3df(0, 0, 0));
	m_camera->setFOV(45.f * irr::core::PI / 180.f);

	// add floor
	auto * floor = smgr->addCubeSceneNode(1.f);
	floor->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	floor->setMaterialTexture(0, video->getTexture("tex1.jpg"));
	floor->setScale(irr::core::vector3df(1000.f, 1.f, 1000.f));
	
	m_events->subscribe("top_camera", std::bind(&Level::handle_camera, this, std::placeholders::_1));
	m_events->subscribe("game_control", std::bind(&Level::handle_game, this, std::placeholders::_1));

	EntityFactory::register_components();
	EntityFactory::register_templates("templates.json");
}

void Level::update(float dt) {
	std::vector<Entity::Ptr> new_entities;
	{
		std::vector<Entity::Ptr> entities;
		m_entity_system.get_entities<MovementComponent, TransformComponent>(entities);

		MovementProcess process;
		process.update(entities, dt);
	}
	{
		std::vector<Entity::Ptr> entities;
		m_entity_system.get_entities<ProductionComponent>(entities);

		ProductionProcess process(new_entities, m_factory);
		process.update(entities, dt);
	}
	
	m_entities.insert(m_entities.end(), new_entities.begin(), new_entities.end());
}

void Level::handle_camera(const irr::SEvent & e) {
	if (e.EventType == irr::EET_MOUSE_INPUT_EVENT) {
		static int x = e.MouseInput.X;
		static int y = e.MouseInput.Y;

		if (e.MouseInput.Control) {
			int diff_x = x - e.MouseInput.X;
			int diff_y = e.MouseInput.Y - y;
			
			irr::core::vector3df position = m_camera->getPosition();
			position.X += diff_x;
			position.Z += diff_y;
			m_camera->setPosition(position);

			irr::core::vector3df target = m_camera->getTarget();
			target.X += diff_x;
			target.Z += diff_y;
			m_camera->setTarget(target);
		}

		if (e.MouseInput.Event == irr::EMIE_MOUSE_WHEEL) {
			irr::core::vector3df position = m_camera->getPosition();
			position.Y += e.MouseInput.Wheel * 10.f;
			m_camera->setPosition(position);
		}

		x = e.MouseInput.X;
		y = e.MouseInput.Y;
	}
}

void Level::handle_game(const irr::SEvent & e)
{
	//static GameObjectPtr building;

	if (e.EventType == irr::EET_KEY_INPUT_EVENT) {
		if (e.KeyInput.Key == irr::KEY_SPACE && !e.KeyInput.PressedDown) {
			place_object("command_center");
		}

		if (e.KeyInput.Key == irr::KEY_KEY_S && !e.KeyInput.PressedDown) {
			place_object("scv");
		}

		if (e.KeyInput.Key == irr::KEY_KEY_B && !e.KeyInput.PressedDown) {
			if (ccc) {
				ProductionComponent::Ptr pr = ccc->get_component<ProductionComponent>();
				if (pr && !pr->in_progress) {
					pr->in_progress = true;
					pr->produce_result = "scv";
					pr->time_remaning = 3.f;
					pr->production_point.set(100, 0, 100);
				}
			}
		}
	}

	if (e.EventType == irr::EET_MOUSE_INPUT_EVENT) {
		//if (building)
		{
			int x = e.MouseInput.X;
			int y = e.MouseInput.Y;

			m_device->getSceneManager();
		}
	}
}

void Level::place_object(const std::string & name) {
	Entity::Ptr entity = m_factory.create_object(name);
	if (entity) {
		TransformComponent::Ptr transform = entity->get_component<TransformComponent>();
		if (transform) {
			transform->position = m_camera->getTarget();
		}

		RenderComponent::Ptr render = entity->get_component<RenderComponent>();
		if (render) {
			render->node = m_device->getSceneManager()->addSphereSceneNode(40);
			render->node->setPosition(m_camera->getTarget());
		}

		ccc = entity;

		m_entities.push_back(entity);
	}
}
