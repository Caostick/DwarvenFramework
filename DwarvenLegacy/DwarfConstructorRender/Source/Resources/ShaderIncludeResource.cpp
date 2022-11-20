#include <DwarfConstructorRender/Resources/ShaderIncludeResource.h>

void rf::ShaderIncludeResource::SetCode(const df::StringView& code) {
	m_Code = df::String(code);
}

auto rf::ShaderIncludeResource::GetCode() const -> const df::String& {
	return m_Code;
}