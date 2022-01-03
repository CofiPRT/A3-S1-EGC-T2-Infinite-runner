#pragma once

#include "BaseHUDObject.h"

#define FRAME_WIDTH (2.f)
#define FRAME_HEIGHT (0.2f)
#define FRAME_LENGTH (1.f)

#define FRAME_POS_X (0.f)
#define FRAME_POS_Y (-1.f + FRAME_HEIGHT / 2)
#define FRAME_POS_Z (1.f)

#define FRAME_COLOR	(COLOR_SPRAY_BLUE)

class Frame : public BaseHUDObject {

public:
	Frame();
	~Frame();

	std::string GetMeshName() override;
	glm::vec3 GetMeshColor() override;

};