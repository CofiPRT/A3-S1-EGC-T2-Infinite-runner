#include "BaseObject.h"

BaseObject::BaseObject() {

}

BaseObject::~BaseObject() {

}

void BaseObject::Update(double deltaTimeSeconds) {

	UpdateDeformation(deltaTimeSeconds);

	if (!IsInBounds()) {
		objState = ObjectState::DEAD;
		return;
	}

	switch (objState) {
	case ObjectState::ALIVE:
		UpdateAlive(deltaTimeSeconds);
		break;
	case ObjectState::DECAYING:
		UpdateDecay(deltaTimeSeconds);
		break;
	default:
		break;
	}

}

void BaseObject::UpdateTrajectory(double deltaTimeSeconds) {

	float g = GetGravitationalAcceleration();
	float t = deltaTimeSeconds;

	// x = x0 + V0x * t
	// y = y0 + V0y * t - (1/2)gt^2
	// z = z0 + V0z * t
	position += velocity * glm::vec3(t) - glm::vec3(0, (1 / 2) * g * t * t, 0);

	// Vx = V0x
	// Vy = V0y - gt
	// Vz = V0z
	velocity -= glm::vec3(0, g * t, 0);

}

void BaseObject::UpdateAlive(double deltaTimeSeconds) {

}
void BaseObject::UpdateDecay(double deltaTimeSeconds) {

	decayTime += deltaTimeSeconds;

	if (decayTime >= GetDecayLength()) {
		objState = ObjectState::DEAD;
		return;
	}

	UpdateTrajectory(deltaTimeSeconds);

}

void BaseObject::UpdateDeformation(double deltaTimeSeconds) {

	if (deformationType == DEFORMATION_NONE)
		return;

	deformationDelta += deltaTimeSeconds;

	if (deformationDelta < deformationDuration)
		return;

	// end deformation
	deformationType = DEFORMATION_NONE;

}

float BaseObject::GetGravitationalAcceleration() {
	return BASE_OBJECT_GRAVITATIONAL_ACCELERATION;
}

float BaseObject::GetDecayLength() {
	return BASE_OBJECT_DECAY_LENGTH;
}

std::string BaseObject::GetMeshName() {
	return "";
}

glm::vec3 BaseObject::GetMeshColor() {
	return COLOR_WHITE;
}

void BaseObject::StartDecay() {
	if (!IsDecaying())
		objState = ObjectState::DECAYING;
}

bool BaseObject::IsHudElement() {
	return false;
}

bool BaseObject::IsInBounds() {
	return true;
}

glm::vec3 BaseObject::GetPosition() {
	return position;
}

glm::vec3 BaseObject::GetRotation() {
	return rotation;
}

glm::vec3 BaseObject::GetScale() {
	return scale;
}

glm::vec3 BaseObject::GetVelocity() {
	return velocity;
}

glm::mat4 BaseObject::GetModelMatrix() {
	glm::mat4 modelMatrix = glm::mat4(1);

	modelMatrix *= Transform3D::Translate(position.x, position.y, position.z);

	modelMatrix *= Transform3D::RotateOX(rotation.x);
	modelMatrix *= Transform3D::RotateOY(rotation.y);
	modelMatrix *= Transform3D::RotateOZ(rotation.z);

	modelMatrix *= Transform3D::Scale(scale.x, scale.y, scale.z);

	return modelMatrix;
}

bool BaseObject::IsAlive() {
	return objState == ObjectState::ALIVE;
}

bool BaseObject::IsDecaying() {
	return objState == ObjectState::DECAYING;
}

bool BaseObject::IsDead() {
	return objState == ObjectState::DEAD;
}

void BaseObject::ApplyDeformation(int deformation, float duration) {
	this->deformationType = deformation;
	this->deformationDelta = 0;
	this->deformationDuration = duration;
}

float BaseObject::GetDeformationDelta() {
	return deformationDelta;
}

float BaseObject::GetDeformationDuration() {
	return deformationDuration;
}

int BaseObject::GetDeformation() {
	return deformationType;
}
