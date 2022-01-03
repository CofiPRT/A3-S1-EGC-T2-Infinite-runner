#pragma once

#include "BaseObject.h"

class BaseHUDObject : public BaseObject {

public:
	bool IsHudElement() override;

	void TogglePosition();

};