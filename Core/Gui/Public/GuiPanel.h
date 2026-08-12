#pragma once
#include "pch.h"

class GuiPanel
{
public:
	virtual ~GuiPanel() = default;

	virtual const char* Name() const = 0;

	virtual void Render() = 0;

	virtual const char* Category() const { return nullptr; }
};
