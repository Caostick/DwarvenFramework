#pragma once

#include <DwarvenCore/Vector.h>

namespace df {
	class InputDevice {
	public:
		virtual auto GetCursorX() const -> double = 0;
		virtual auto GetCursorY() const -> double = 0;

		virtual auto GetAxis(int axis) const -> double = 0;
		virtual void Update() = 0;

		virtual auto GetEvents() const -> const df::Vector<int>&;
		void PushEvent(int eventId);
		void ResetEvents();

	private:
		df::Vector<int> m_Events;
	};
}