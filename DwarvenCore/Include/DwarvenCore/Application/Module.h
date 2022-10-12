#pragma once

namespace df {
	class Application;
}

namespace df {
	class Module {
	public:
		virtual ~Module();
		virtual bool InitModule(Application& application);
		virtual void ReleaseModule(Application& application);

		virtual bool LoadModule(Application& application);
		virtual void UnloadModule(Application& application);

		virtual bool Update(Application& application);
	};
}