#pragma once

#include <iostream>
#include "../Transform/Transform.h"

namespace Glide3D
{
	template<typename T>
	class Instance
	{
	public :
		T* p_Object;
		Transform p_Transform;
		bool p_Visible;

		Transform& GetTransform() { return p_Transform; }
		bool IsVisible() { return p_Visible; }
		T* GetParentObject{ return p_Object; }
	};
}