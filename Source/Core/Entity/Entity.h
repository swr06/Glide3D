#pragma once

#include <iostream>
#include "../Object/Object.h"
#include "../Transform/Transform.h"

namespace Glide3D
{
	struct ReflectionMapProperties
	{
		uint32_t res;
		uint32_t update_rate; 
	};


	class Entity
	{
	public :
		Entity(Object* obj, bool has_reflection_map = false, const ReflectionMapProperties& props = { 128, 30 })
			: p_ReflectionCubemap(props.res)
		{
			p_RenderReflectionCubemap = has_reflection_map;
			p_ReflectionProps = props;
			p_Object = obj;
		}

		Object* p_Object;
		Transform p_Transform;
		bool p_Visible;
		bool p_ShouldCastShadow = true;
		bool p_ShouldRender = true;

		Transform& GetTransform() { return p_Transform; }
		bool IsVisible() { return p_Visible; }
		Object* GetParentObject() { return p_Object; }

		// The cube reflection map to reflect the in game scenes
		bool p_RenderReflectionCubemap = false;
		GLClasses::CubeReflectionMap p_ReflectionCubemap;
		ReflectionMapProperties p_ReflectionProps;
	};
}