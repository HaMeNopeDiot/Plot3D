#ifndef LIGHT_3D_H
#define LIGHT_3D_H

#include "Data.h"

enum LIGHT_TYPE { background, directional, non_directional};

class Light3D {
	Position pos_light;
	Position pos_target;
	LIGHT_TYPE type;
public:

};
#endif LIGHT_3D_H
