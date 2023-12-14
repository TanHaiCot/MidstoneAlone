#include "Body.h"

Body::Body() {}

Body::Body(Vec3 pos_, Vec3 vel_, Vec3 acc_,
	float mass_, const char* textureFile_) {
	pos = pos_; 
	vel = vel_; 
	acc = acc_;
	mass = mass_; 
	textureFile = textureFile_;
}

void Body::Update(float deltaTime) {
}

Vec3 Body::getPos() {
	return pos;
}

const char* Body::GetTextureFile() {
	return textureFile;
}

void Body::SetTextureFile(const char* textureFile_)
{
	textureFile = textureFile_;
}

Body::~Body() {
}

