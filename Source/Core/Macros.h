#pragma once

#define GLIDE3D_IMPLEMENTATION
#define GLIDE3D_PROJECT

#define MAX_DIRECTIONAL_LIGHTS 2
#define MAX_POINT_LIGHTS 14

#if MAX_DIRECTIONAL_LIGHTS > 2
#error The maximum directional lights cannot be above 2
#endif

#if MAX_POINT_LIGHTS > 15
#error The maximum point lights cannot be above 15
#endif