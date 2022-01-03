#pragma once

#include <include/glm.h>

namespace Transform3D
{
	// Translate matrix
	inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
	{
		return glm::mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			translateX, translateY, translateZ, 1
		);
	}

	// Scale matrix
	inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
	{
		glm::mat3 hey = glm::mat4(1);
		return glm::mat4(
			scaleX, 0, 0, 0,
			0, scaleY, 0, 0,
			0, 0, scaleZ, 0,
			0, 0, 0, 1
		);
	}

	// Rotate matrix relative to the OZ axis
	inline glm::mat4 RotateOZ(float radians)
	{
		float resCos = cos(radians);
		float resSin = sin(radians);

		return glm::mat4(
			resCos, resSin, 0, 0,
			-resSin, resCos, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}

	// Rotate matrix relative to the OY axis
	inline glm::mat4 RotateOY(float radians)
	{
		float resCos = cos(radians);
		float resSin = sin(radians);

		return glm::mat4(
			resCos, 0, -resSin, 0,
			0, 1, 0, 0,
			resSin, 0, resCos, 0,
			0, 0, 0, 1
		);
	}

	// Rotate matrix relative to the OX axis
	inline glm::mat4 RotateOX(float radians)
	{
		float resCos = cos(radians);
		float resSin = sin(radians);

		return glm::mat4(
			1, 0, 0, 0,
			0, resCos, resSin, 0,
			0, -resSin, resCos, 0,
			0, 0, 0, 1
		);
	}
}
