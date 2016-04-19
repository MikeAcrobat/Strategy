#pragma once

#include <IEventReceiver.h>
#include <functional>
#include <string>
#include <vector>

class EventReceiver : public irr::IEventReceiver
{
public:
	virtual bool OnEvent(const irr::SEvent&);

	typedef std::function<void(const irr::SEvent&)> ReceiverCallback;

	void subscribe(const std::string &, ReceiverCallback);

private:

	struct Receiver
	{
		std::string tag;
		ReceiverCallback callback;
	};

	std::vector<Receiver> m_receivers;
};