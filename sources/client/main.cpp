#include "irrlicht.h"
#include "gamelevel.h"
#include "inputmanager.h"

int main(int argc, char * argv[])
{
	using namespace irr;
	
	EventReceiver receiver;

	IrrlichtDevice * device = createDevice(video::EDT_OPENGL, irr::core::dimension2du(1024, 768));
	device->setEventReceiver(&receiver);

	video::IVideoDriver * driver = device->getVideoDriver();
	scene::ISceneManager * smgr = device->getSceneManager();

	Level level(device, &receiver);

	irr::ITimer * timer = device->getTimer();

	int prev_time = timer->getRealTime();
	int delta = 1000 / 60;
	int accumulator = 0;

	while (device->run())
	{
		int now = timer->getRealTime();
		int frame_time = now - prev_time;

		accumulator += frame_time;

		while (accumulator >= delta) {
			level.update(delta / 1000.f);
			accumulator -= delta;
		}

		prev_time = now;

		driver->beginScene(true, true, video::SColor(255, 100, 101, 140));

		smgr->drawAll();

		driver->endScene();
	}

	device->drop();

	return 0;
}