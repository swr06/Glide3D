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
			assert(!(obj == nullptr));
			p_RenderReflectionCubemap = has_reflection_map;
			p_Object = obj;
		}

		glm::vec3 GetCenter()
		{
			assert(!(p_Object == nullptr));
			glm::vec4 localized_center = glm::vec4(p_Object->p_Center, 1.0f);
			localized_center = p_Transform.GetTransformationMatrix() * localized_center;
			
			return glm::vec3(localized_center.x, localized_center.y, localized_center.z);
		}

		const Object* p_Object = nullptr;
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
		glm::vec3 p_ReflectionPosition; // The position of the camera when it renders the scene into a cubemap
	};
}