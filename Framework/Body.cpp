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

void Body::ApplyForce(Vec3 force) {
	acc.x = force.x / mass;
	acc.y = force.y / mass;
	acc.z = force.z / mass;
}

//void Body::ApplyDrag(Vec3 force)
//{
	//acc.x -= c * vel.x / mass;
	//acc.y -= c * vel.y / mass;
	//acc.z -= c * vel.z / mass;
//}

void Body::setWidth(float w)
{
	width = w;
}

void Body::setHeight(float h)
{
	height = h;
}

float Body::getWidth()
{
	return width;
}

float Body::getHeight()
{
	return height;
}

void Body::Update(float deltaTime) {
	//code up the equasions of motion
	//remember previous assignment
	pos.x += vel.x * deltaTime + 0.5f * acc.x * deltaTime * deltaTime;
	vel.x += acc.x * deltaTime;

	pos.y += vel.y * deltaTime + 0.5f * acc.y * deltaTime * deltaTime;
	vel.y += acc.y * deltaTime;

	//if (pos.y <= 0.0f) {
		//pos.y = 0;
		//vel.y = 0;
	//}
}

void Body::Update(char direction, float horzLimit, float verLimit)
{
	if (direction == 'L' && (pos.x - vel.x) >= 0) {
		pos.x -= vel.x;
	}

	else if (direction == 'R' && (pos.x + width + vel.x < horzLimit)) {
		pos.x += vel.x;
	}

	else if (direction == 'U') {
		pos.y += vel.y;
	}

	else if (direction == 'D') {
		pos.y -= vel.y;
	}
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

