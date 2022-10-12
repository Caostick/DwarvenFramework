#include <DwarvenCore/Application/Module.h>

df::Module::~Module() {

}

bool df::Module::InitModule(Application& /*application*/) {
	return true;
}

void df::Module::ReleaseModule(Application& /*application*/) {

}

bool df::Module::LoadModule(Application& /*application*/) {
	return true;
}

void df::Module::UnloadModule(Application& /*application*/) {

}

bool df::Module::Update(Application& /*application*/) {
	return true;
}