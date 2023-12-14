#pragma once
#include "Vector.h"

using namespace GAME106_MATH;

class Body {

public:

	// Vectors set to [0,0,0] by their default constructor
	Vec3 pos;
	Vec3 vel;
	Vec3 acc;
	// A mass of 1 kg seems like a good default value
	float mass = 1.0f;
	const char* textureFile = "";

	/// <summary>
	/// Default Constructor
	/// </summary>
	Body();

	/// <summary>
	/// Constructor. Create Body object and assign its attributes' values. 
	/// </summary>
	/// <param name="pos_">position</param>
	/// <param name="vel_">velocity</param>
	/// <param name="accel_">acceleration</param>
	/// <param name="mass_">mass</param>
	/// <param name="textureFile_">object texture file</param>
	Body(Vec3 pos_, Vec3 vel_, Vec3 accel_, 
		float mass_, const char* textureFile_);

	/// <summary>
	/// Update the location of this body based on the given detla time
	/// </summary>
	/// <param name="deltaTime">the difference in time</param>
	void Update(float deltaTime);

	/// <returns>The current position of this body</returns>
	Vec3 getPos();

	/// <returns>The texture of this body</returns>
	const char* GetTextureFile();

	/// <summary>
	/// Set a new texture file for this body
	/// </summary>
	/// <param name="textureFile_">The new texture file</param>
	void SetTextureFile(const char* textureFile_);

	/// <summary>
	/// Destructor. Delete all pointer attributes in this class 
	/// then delete this object.
	/// </summary>
	~Body();

};
