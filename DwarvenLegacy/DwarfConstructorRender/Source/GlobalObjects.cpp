#include <DwarfConstructorRender/GlobalObjects.h>

#include "RenderCore.h"

#include <DwarvenCore/Math/Math.h>
#include <DwarvenCore/Math/TangentFrame.h>
//#include <DwarvenCore/DebugName.h>

namespace {
	rf::RasterizationStateId g_RasterizationStates[int(rf::ERasterizationState::COUNT)];
	rf::BlendStateId g_BlendStates[int(rf::EBlendState::COUNT)];
	rf::DepthStateId g_DepthStates[int(rf::EDepthStencilState::COUNT)];
	rf::TextureId g_Textures[int(rf::ETexture::COUNT)];
	rf::SamplerId g_Samplers[int(rf::ESamplerState::COUNT)];

	void CreateRasterizationStates(rf::RenderCore& renderCore) {
		for (auto i = 0; i< int(rf::ERasterizationState::COUNT); ++i) {
			g_RasterizationStates[i] = renderCore.RequestRasterizationState(rf::ERasterizationState(i));
		}
	}

	void CreateBlendStates(rf::RenderCore& renderCore) {
		for (auto i = 0; i< int(rf::EBlendState::COUNT); ++i) {
			g_BlendStates[i] = renderCore.RequestBlendState(rf::EBlendState(i));
		}
	}

	void CreateDepthStates(rf::RenderCore& renderCore) {
		for (auto i = 0; i< int(rf::EDepthStencilState::COUNT); ++i) {
			g_DepthStates[i] = renderCore.RequestDepthState(rf::EDepthStencilState(i));
		}
	}

	void CreateSamplerStates(rf::RenderCore& renderCore) {
		rf::SamplerState samplerState;
		samplerState.m_AddressMode = rf::EAddressMode::Repeat;
		samplerState.m_MipmapMode = rf::EMipmapMode::Linear;

		samplerState.m_Filter = rf::EFilter::Nearest;
		g_Samplers[int(rf::ESamplerState::Nearest)] = renderCore.CreateSampler(samplerState);

		samplerState.m_Filter = rf::EFilter::Linear;
		g_Samplers[int(rf::ESamplerState::Linear)] = renderCore.CreateSampler(samplerState);
	}

	void DestroySamplerStates(rf::RenderCore& renderCore) {
		for (int i = 0; i< int(rf::ESamplerState::COUNT); ++i) {
			renderCore.DestroySampler(g_Samplers[i]);
			g_Samplers[i] = rf::SamplerId();
		}
	}

	auto CreateNullTexture(rf::RenderCore& renderCore) -> rf::TextureId {
		uint8 data[64];
		for (int i = 0; i < 16; ++i) {
			data[i * 4 + 0] = 255;
			data[i * 4 + 1] = 0;
			data[i * 4 + 2] = 255;
			data[i * 4 + 3] = 255;
		}

		rf::TextureId textureId = renderCore.CreateTexture("Null Texture", 4, 4, rf::EImageUsageFlag::Texture, rf::ETextureFormat::R8G8B8A8_UNorm);
		renderCore.SetTextureData(textureId, data, 64);

		return textureId;
	}

	auto CreateWhiteTexture(rf::RenderCore& renderCore) -> rf::TextureId {
		uint8 data[64];
		for (int i = 0; i < 16; ++i) {
			data[i * 4 + 0] = 255;
			data[i * 4 + 1] = 255;
			data[i * 4 + 2] = 255;
			data[i * 4 + 3] = 255;
		}

		rf::TextureId textureId = renderCore.CreateTexture("White Texture", 4, 4, rf::EImageUsageFlag::Texture, rf::ETextureFormat::R8G8B8A8_UNorm);
		renderCore.SetTextureData(textureId, data, 64);

		return textureId;
	}

	auto CreateBlackTexture(rf::RenderCore& renderCore) -> rf::TextureId {
		uint8 data[64];
		for (int i = 0; i < 16; ++i) {
			data[i * 4 + 0] = 0;
			data[i * 4 + 1] = 0;
			data[i * 4 + 2] = 0;
			data[i * 4 + 3] = 255;
		}

		rf::TextureId textureId = renderCore.CreateTexture("Black Texture", 4, 4, rf::EImageUsageFlag::Texture, rf::ETextureFormat::R8G8B8A8_UNorm);
		renderCore.SetTextureData(textureId, data, 64);

		return textureId;
	}

	void CreateTextures(rf::RenderCore& renderCore) {
		g_Textures[int(rf::ETexture::Null)] = CreateNullTexture(renderCore);
		g_Textures[int(rf::ETexture::White)] = CreateWhiteTexture(renderCore);
		g_Textures[int(rf::ETexture::Black)] = CreateBlackTexture(renderCore);
	}

	void DestroyTextures(rf::RenderCore& renderCore) {
		for (int i = 0; i< int(rf::ETexture::COUNT); ++i) {
			renderCore.DestroyTexture(g_Textures[i]);
			g_Textures[i] = rf::TextureId();
		}
	}
}

void rf::GlobalObjects::Create(rf::RenderCore& renderCore) {
	CreateRasterizationStates(renderCore);
	CreateBlendStates(renderCore);
	CreateDepthStates(renderCore);
	CreateSamplerStates(renderCore);
	CreateTextures(renderCore);
}

void rf::GlobalObjects::Destroy(rf::RenderCore& renderCore) {
	DestroySamplerStates(renderCore);
	DestroyTextures(renderCore);
}

auto rf::GlobalObjects::Get(rf::ERasterizationState state)->rf::RasterizationStateId {
	return g_RasterizationStates[int(state)];
}

auto rf::GlobalObjects::Get(rf::EBlendState state)->rf::BlendStateId {
	return g_BlendStates[int(state)];
}

auto rf::GlobalObjects::Get(rf::EDepthStencilState state)->rf::DepthStateId {
	return g_DepthStates[int(state)];
}

auto rf::GlobalObjects::Get(rf::ETexture texture)->rf::TextureId {
	return g_Textures[int(texture)];
}

auto rf::GlobalObjects::Get(rf::ESamplerState samplerState)->rf::SamplerId {
	return g_Samplers[int(samplerState)];
}