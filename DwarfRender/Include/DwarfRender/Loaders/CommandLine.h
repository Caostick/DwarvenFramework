#pragma once

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/HashMap.h>
#include <DwarvenCore/Types.h>

namespace rf {
	class ParamSet;
	class ParamSetDefinition;

	namespace sg {
		class ShaderGraph;
		class Node;
	}
}

namespace rf {
	class CommandLine {
	public:
		auto GetError() const -> const df::String&;

		bool ExecuteCode(const df::StringView& code);

		virtual bool Execute() = 0;

	protected:
		bool ExecuteLine(const df::StringView& line);
		bool ParseLine(const df::StringView& line);
		bool ParseLeft(const df::StringView& code);
		bool ParseRight(const df::StringView& code);

		bool ParseValue(float* values, uint32* count, uint32 maxCount);

		bool Error(const char* text);

	protected:
		uint32 m_Line;

		struct Left {
			df::StringView Name;
			df::StringView PropertyName;
		} m_Left;

		struct Right {
			df::StringView NodeType;

			df::StringView NameValue;
			df::StringView ConstValue;
			df::StringView StringValue;
		} m_Right;

		df::String m_Error;
	};

	class CommandLine_ParamSetMaterialName : public CommandLine {
	public:
		auto GetName() const -> const df::StringView&;

	protected:
		bool Execute() override;

	private:
		df::StringView m_Name;
	};

	class CommandLine_ParamSet : public CommandLine {
	public:
		CommandLine_ParamSet(ParamSet& paramSet);

	protected:
		bool Execute() override;

	private:
		ParamSet& m_ParamSet;
	};

	namespace sg {
		class CommandLine_ShaderGraph : public CommandLine {
		public:
			CommandLine_ShaderGraph(ShaderGraph& shaderGraph);

		protected:
			bool Execute() override;

		private:
			ShaderGraph& m_ShaderGraph;

			df::HashMap<df::String, Node*> m_Nodes;
		};
	}
}