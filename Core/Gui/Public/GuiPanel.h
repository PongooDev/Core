#pragma once
#include "pch.h"

class GuiPanel
{
public:
	virtual ~GuiPanel() = default;

	virtual const char* Name() const = 0;

	virtual void Render() = 0;

	virtual void Tick() {}

	virtual const char* Category() const { return nullptr; }

	virtual int BadgeCount() const { return 0; }

	virtual bool BadgeIsAlert() const { return false; }
};
