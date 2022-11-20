#pragma once

#include <DwarfResources/Resource.h>

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>

namespace rf {
	class ShaderIncludeResource : public df::IResource {
	public:
		ShaderIncludeResource() = default;

		void SetCode(const df::StringView& code);
		auto GetCode() const -> const df::String&;

	private:
		df::String m_Code;
	};
}