#include <DwarfRender/ShaderGraph/Types.h>

auto rf::sg::ToString(EVariableType type) -> df::String {
	switch (type) {
    case EVariableType::Unknown:
        break;
    case EVariableType::Float:
        return "float";
    case EVariableType::Vec2:
        return "vec2";
    case EVariableType::Vec3:
        return "vec3";
    case EVariableType::Vec4:
        return "vec4";
    case EVariableType::Mat3:
        return "mat3";
    case EVariableType::Mat4:
        return "mat4";
    default:
        break;
	}

    return "";
}