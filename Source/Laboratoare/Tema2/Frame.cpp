#include "Frame.h"

Frame::Frame() {
	this->position = glm::vec3(FRAME_POS_X, FRAME_POS_Y, FRAME_POS_Z);
	this->scale = glm::vec3(FRAME_WIDTH, FRAME_HEIGHT, FRAME_LENGTH);
}

Frame::~Frame() {

}

std::string Frame::GetMeshName() {
	return "trapezoid";
}

glm::vec3 Frame::GetMeshColor() {
	return FRAME_COLOR;
}