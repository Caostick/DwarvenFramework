#include "DwarfInput/InputDevice.h"

auto df::InputDevice::GetEvents() const -> const df::Vector<int>& {
	return m_Events;
}

void df::InputDevice::PushEvent(int eventId) {
	m_Events.push_back(eventId);
}

void df::InputDevice::ResetEvents() {
	m_Events.clear();
}