#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Macros.h"
#include "Application/Application.h"
#include "Model Loader/OBJFileLoader.h"
#include "Application/Logger.h"
#include "Entity/Entity.h"
#include "Object/Object.h"
#include "ObjectTypes/Cube.h"
#include "ObjectTypes/Cone.h"
#include "ObjectTypes/Sphere.h"
#include "ObjectTypes/Plane.h"
#include "FpsCamera.h"
#include "OrthographicCamera.h"

#include "GL_Classes/Fps.h"

// The Glide3D Game Engine