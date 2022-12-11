#pragma once

#include <DwarvenCore/Types.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/Vector.h>

namespace df {
    struct PipelineCompileInfo {
		Vector<uint32> m_VertexSpirV;
		Vector<uint32> m_FragmentSpirV;
		Vector<String> m_VertexAttributes;
		Vector<String> m_ParameterSets;
	};
}