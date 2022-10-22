#include <DwarfConstructorRender/ShaderParameter.h>

const char* rf::ShaderParameter::Name = "";

const float rf::FloatParameter::DefaultValue = 0.0f;
const Vec2 rf::Vec2Parameter::DefaultValue = Vec2();
const Vec3 rf::Vec3Parameter::DefaultValue = Vec3();
const Vec4 rf::Vec4Parameter::DefaultValue = Vec4();
const Mat3 rf::Mat3Parameter::DefaultValue = Mat3::Identity();
const Mat4 rf::Mat4Parameter::DefaultValue = Mat4::Identity();

const bool rf::BufferParameter::Multiframed = false;