#include "BaseFuelIndicator.h"

BaseFuelIndicator::BaseFuelIndicator() {
	this->position = glm::vec3(INDICATOR_POS_X, INDICATOR_POS_Y, INDICATOR_POS_Z);
	this->scale = glm::vec3(INDICATOR_WIDTH, INDICATOR_HEIGHT, INDICATOR_LENGTH);
}

BaseFuelIndicator::~BaseFuelIndicator() {
	
}

std::string BaseFuelIndicator::GetMeshName() {
	return "box";
}
