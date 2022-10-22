#include "VertexDescription.h"

bool rf::VertexDescription::operator == (const VertexDescription& other) const {
	return m_APIData == other.m_APIData;
}

bool rf::VertexDescription::operator != (const VertexDescription& other) const {
	return m_APIData != other.m_APIData;
}