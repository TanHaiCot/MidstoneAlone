#pragma once
#include <iostream>
#include <iomanip>      // to get std::setprecision (for output precision)


// Use your own math code from GAME 106 Game Mathematics 1
// Next semester we'll switch to Dr. Scott Fielder's high powered math library!
namespace GAME106_MATH {
	struct Vec3 {
		float  x, y, z;	///  Structures are public by default

		/// Setting the values of the attributes
		void set(float x_, float y_, float z_);

		/////////////////////////////////////////////////////////
		////////////////   A Set of Constructors   /////////////
		////////////////////////////////////////////////////////

		/// Constructor with one float value or no value (i.e., default)
		explicit Vec3(float s = float(0.0)); // What does this 'explict' word mean? Look it up!

		/// Constructor with 3 float values
		Vec3(float x, float y, float z);

		/// Constructor with one Vec3 value
		Vec3(const Vec3& v);

		/// Print out the values of this vector
		void print();

		/// Destructor. Delete this object. 
		~Vec3();
	};
}



