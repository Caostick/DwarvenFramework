#pragma once

#include <DwarfRender/ObjectId.h>
#include <DwarfRender/RenderViewRegistry.h>
#include <DwarfRender/ParamSet.h>

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Math/Math.h>
#include <DwarvenCore/TypeList.h>

namespace rf {
	class RenderCore;
	class RenderPass;
	class ParamSet;
	class FramedDataBuffer;
	class ParamSetUpdater;
	class ParamSetDefinition;
	class RenderPassInstance;
	class ModelStream;
	class ParamSetDefinitionManager;

	struct RenderContext;
}

namespace rf {
	class RenderView {
		template<typename, typename>
		friend struct RenderViewPassConstructor;
	public:
		virtual ~RenderView();

		using RenderPasses = df::TypeList<>;
		using DataBuffers = df::TypeList<>;

		void Init(rf::RenderCore& renderCore);
		void Release(rf::RenderCore& renderCore);

		template<typename RenderTargetType>
		auto GetRenderTarget()->rf::TextureId;

		template<typename DataBufferType>
		auto GetDataBuffer()->rf::BufferId;

		auto CreateModelStream()->rf::ModelStream*;
		void DestroyModelStream(rf::ModelStream* modelStream);

		bool RenderFrame(const rf::RenderContext& renderContext);

		auto GetOutput() const->rf::TextureId;

	protected:
		virtual void UpdateParams(uint32 frameIdx) = 0;
		virtual void ReleaseParams(rf::RenderCore& renderCore) = 0;

	public:
		auto RequestRenderTarget(rf::RenderCore& renderCore, uint32 renderTargetId)->rf::TextureId;

		template<typename RenderPassType, typename ...TArgs>
		auto AddRenderPass(TArgs&&... args)->RenderPassType*;

		void AddRenderPass(rf::RenderPass* renderPass, uint32 typeId);

	private:
		df::Vector<RenderPassInstance*> m_RenderPasses;
		df::Vector<rf::ModelStream*> m_ModelStreams;
		df::Vector<rf::BufferId> m_DataBuffers;
		df::Vector<rf::TextureId> m_RenderTargets;

		df::Vector<rf::TextureId> m_RenderTargetsToInit;
	};

	template<typename SetupType>
	class TRenderView : public RenderView {
	public:

		void InitParams(rf::RenderCore& renderCore, rf::ParamSetDefinitionManager& paramsDefinitionManager);

		template<typename ParamType, typename DataType = typename ParamType::DataType>
		void SetViewConstant(const DataType& value);

	protected:
		void UpdateParams(uint32 frameIdx) override;
		void ReleaseParams(rf::RenderCore& renderCore) override;

	protected:
		rf::TParamSet<typename SetupType::ViewShaderParams>* m_Params;
	};
}

#include <DwarfRender/RenderView.inl>