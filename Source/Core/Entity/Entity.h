#pragma once

#include <iostream>
#include "../Object/Object.h"
#include "../Transform/Transform.h"

namespace Glide3D
{
	class Entity
	{
	public :
		Entity(const Object* obj, bool has_reflection_map = false)
			: p_ReflectionCubemap(obj->p_ReflectionProps.res)
		{
			p_RenderReflectionCubemap = has_reflection_map;
			p_Object = obj;
		}

		const Object* p_Object;
		Transform p_Transform;
		bool p_Visible;
		bool p_ShouldCastShadow = true;
		bool p_ShouldRender = true;

		Transform& GetTransform() { return p_Transform; }
		bool IsVisible() { return p_Visible; }
		const Object* GetParentObject() { return p_Object; }

		// The cube reflection map to reflect the in game scenes
		bool p_RenderReflectionCubemap = false;
		GLClasses::CubeReflectionMap p_ReflectionCubemap;
	};
}