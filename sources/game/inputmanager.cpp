#include "inputmanager.h"
#include <algorithm>

bool EventReceiver::OnEvent(const irr::SEvent & event) {
	for (const auto & r : m_receivers) {
		r.callback(event);
	}
	return false;
}

void EventReceiver::subscribe(const std::string & tag, ReceiverCallback callback) {
	if (!callback) {
		return;
	}

	bool duplicate = std::any_of(m_receivers.begin(), m_receivers.end(), [&](const Receiver & r) {
		return r.tag == tag;
	});

	if (!duplicate) {
		Receiver r;
		r.tag = tag;
		r.callback = callback;
		m_receivers.push_back(r);
	}
}
