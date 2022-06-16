﻿#pragma once

#include "Base.h"

class Layer
{
public:
	Layer(const std::string& debugName);
	virtual ~Layer() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate(float ts) {}
	virtual void OnImGuiRender() {}

	const std::string& GetName() const { return m_DebugName; }

protected:
	std::string m_DebugName;
};
