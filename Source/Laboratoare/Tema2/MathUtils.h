#pragma once

#include <functional>

struct MathUtils {
	static std::function<float(float)> MapLinear(float oldStart, float oldEnd, float newStart, float newEnd) {
		return [oldStart, oldEnd, newStart, newEnd](float t) {
			return newStart + (newEnd - newStart) / (oldEnd - oldStart) * (t - oldStart);
		};
	}

	static std::function<float(float)> MapSmooth(float oldStart, float oldEnd, float newStart, float newEnd, float smoothness) {
		return [oldStart, oldEnd, newStart, newEnd, smoothness](float t) {
			return newStart + (newEnd - newStart) * powf((t - oldStart) / (oldEnd - oldStart), smoothness);
		};
	}

};