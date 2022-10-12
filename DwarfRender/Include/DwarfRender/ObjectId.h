#pragma once

#include <DwarvenCore/Types.h>

namespace rf {
	struct Texture;
	using TextureId = Texture*;

	class Mesh;
	using MeshId = Mesh*;

	struct ShaderModule;
	using ShaderModuleId = ShaderModule*;

	struct RasterizationState;
	using RasterizationStateId = RasterizationState*;

	struct BlendState;
	using BlendStateId = BlendState*;

	struct DepthState;
	using DepthStateId = DepthState*;

	struct VertexDescription;
	using VertexDescriptionId = VertexDescription*;

	struct Pass;
	using PassId = Pass*;

	struct Framebuffer;
	using FramebufferId = Framebuffer*;

	struct Buffer;
	using BufferId = Buffer*;

	struct Sampler;
	using SamplerId = Sampler*;
	
	struct DescriptorSetLayout;
	using DescriptorSetLayoutId = DescriptorSetLayout*;

	struct DescriptorSet;
	using DescriptorSetId = DescriptorSet*;

	struct VertexShaderModule;
	using VertexShaderModuleId = VertexShaderModule*;

	struct FragmentShaderModule;
	using FragmentShaderModuleId = FragmentShaderModule*;

	struct ComputeShaderModule;
	using ComputeShaderModuleId = ComputeShaderModule*;

	struct Pipeline;
	using PipelineId = Pipeline*;

	struct PipelineLayout;
	using PipelineLayoutId = PipelineLayout*;

	class RenderView;
	using RenderViewId = RenderView*;

	struct GraphicsPipeline;
	using GraphicsPipelineId = GraphicsPipeline*;

	class MaterialRule;
	using MaterialRuleId = MaterialRule*;

	class Material;
	using MaterialId = Material*;
}