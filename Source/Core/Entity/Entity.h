#pragma once

#include <iostream>
#include "../Object/Object.h"
#include "../Transform/Transform.h"

namespace Glide3D
{
	class Entity
	{
	public :
		Entity(Object* obj)
		{
			p_Object = obj;
		}

		Object* p_Object;
		Transform p_Transform;
		bool p_Visible;

		Transform& GetTransform() { return p_Transform; }
		bool IsVisible() { return p_Visible; }
		Object* GetParentObject() { return p_Object; }
	};
}