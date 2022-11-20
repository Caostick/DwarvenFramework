#pragma once

#include <DwarfConstructorRender/ShaderGraph/PipelineGenerator.h>
#include <DwarfConstructorRender/ShaderGraph/VertexShaderGenerator.h>
#include <DwarfConstructorRender/ShaderGraph/FragmentShaderGenerator.h>

namespace rf {
	namespace sg {
		class GraphicsPipelineGenerator : public PipelineGenerator {
		public:
			~GraphicsPipelineGenerator() override;

			void Init();
			void Release();

			virtual bool Build(rf::sg::ShaderGraph& shaderGraph);
			virtual void Reset(rf::ParamSetDefinition* parameters);

			void SetPass(EShaderPass pass);

		protected:
			virtual auto GetVertexShader()->VertexShaderGenerator & = 0;
			virtual auto GetFragmentShader()->FragmentShaderGenerator & = 0;

			EShaderPass m_Pass;
		};

		template<typename SetupType>
		class TGraphicsPipelineGenerator : public GraphicsPipelineGenerator {
		public:
			TGraphicsPipelineGenerator(const rf::ParamSetDefinitionManager& paramSetDefinitionManager);

		protected:
			auto GetVertexShader()->VertexShaderGenerator & override;
			auto GetFragmentShader()->FragmentShaderGenerator & override;

			TVertexShaderGenerator<SetupType> m_VertexShader;
			TFragmentShaderGenerator<SetupType> m_FragmentShader;
		};
	}
}

#include <DwarfConstructorRender/ShaderGraph/GraphicsPipelineGenerator.inl>