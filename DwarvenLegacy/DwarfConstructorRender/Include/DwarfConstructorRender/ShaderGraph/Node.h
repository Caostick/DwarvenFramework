#pragma once

#include <DwarfConstructorRender/ShaderGraph/PipelineGenerator.h>

#include <DwarvenCore/New.h>
#include <DwarvenCore/Types.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/HashMap.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Result.h>
#include <DwarvenCore/Math/Math.h>

namespace rf {
	namespace sg {
		class Variable;
		class PipelineGenerator;
	}

	class ParamSetDeclarator;
}

namespace rf {
	namespace sg {
		class Node {
		public:
			Node() = default;
			virtual ~Node() {}

			virtual void MakeBindings() = 0;
			virtual void MakeVariable(PipelineGenerator& pipeline) = 0;
			virtual void AddParameter(ParamSetDeclarator& paramDeclarator);
			virtual bool IsParameter() const;

			bool Set(const df::StringView& socketName, Node* node);
			bool Set(const df::StringView& propertyName, float value);
			bool Set(const df::StringView& propertyName, const Vec2& value);
			bool Set(const df::StringView& propertyName, const Vec3& value);
			bool Set(const df::StringView& propertyName, const Vec4& value);
			bool Set(const df::StringView& propertyName, const df::StringView& value);

			auto GetVariable(PipelineGenerator& pipeline) -> const Variable*;
			void ResetVariable();
			void ResetValues();
			void Reset();

		protected:
			void Bind(const df::StringView& socketName, Node** node);
			void Bind(const df::StringView& propertyName, float* propertyPtr);
			void Bind(const df::StringView& propertyName, Vec2* propertyPtr);
			void Bind(const df::StringView& propertyName, Vec3* propertyPtr);
			void Bind(const df::StringView& propertyName, Vec4* propertyPtr);
			void Bind(const df::StringView& propertyName, df::String* propertyPtr);

			void Bind(const df::Vector<df::StringView> socketNames, Node** node);
			void Bind(const df::Vector<df::StringView> propertyNames, float* propertyPtr);
			void Bind(const df::Vector<df::StringView> propertyNames, Vec2* propertyPtr);
			void Bind(const df::Vector<df::StringView> propertyNames, Vec3* propertyPtr);
			void Bind(const df::Vector<df::StringView> propertyNames, Vec4* propertyPtr);
			void Bind(const df::Vector<df::StringView> propertyNames, df::String* propertyPtr);

			bool CheckError(bool condition, const df::StringView& error);
			bool HasError() const;

		protected:
			df::HashMap<df::String, float*> m_FloatProperties;
			df::HashMap<df::String, Vec2*> m_Vec2Properties;
			df::HashMap<df::String, Vec3*> m_Vec3Properties;
			df::HashMap<df::String, Vec4*> m_Vec4Properties;
			df::HashMap<df::String, Node**> m_Nodes;
			df::HashMap<df::String, df::String*> m_StringProperties;

			const Variable* m_Variable = nullptr;
			df::String m_Error;
		};
	}
}