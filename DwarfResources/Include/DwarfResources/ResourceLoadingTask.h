#pragma once

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Task.h>

#include <functional>

namespace df {
	class Resource2;
}

namespace df {
	class ResourceLoadingTask {
	public:
		Resource2* m_Resource = nullptr;

		Vector<std::function<void(const Resource2&)>> m_Callbacks;
	};
}