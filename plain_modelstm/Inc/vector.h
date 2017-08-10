#ifndef _VECTOR_H_
#define _VECTOR_H_


class vector3{
public:
	float x,y,z;

	vector3();
	vector3(float, float, float);
	vector3 operator*(float c){return vector3(x*c,y*c,z*c);}
};

#endif

