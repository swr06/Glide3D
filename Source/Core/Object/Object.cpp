#include "Object.h"

namespace Glide3D
{
	Object::Object() : p_DefaultColor(glm::vec3(1.0f, 0.5f, 0.31f))
	{
		p_Texture = new GLClasses::Texture();
		p_LightMap = new GLClasses::Texture();
		p_AlbedoMap = new GLClasses::Texture();
		p_NormalMap = new GLClasses::Texture();
	}

	Object::~Object()
	{
		delete p_Texture;
		delete p_LightMap;
		delete p_AlbedoMap;
		delete p_NormalMap;
	}
}