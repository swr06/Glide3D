#pragma once

#include <glad/glad.h>
#include <KHR/khrplatform.h>

#include <string>
#include <iostream>

/*#define DebugGL CheckOpenGLError(__FILE__, __LINE__);
#define DebugGLFunction(x) x; CheckOpenGLError(__FILE__, __LINE__);
#define GLCall(x) x; CheckOpenGLError(__FILE__, __LINE__);*/

GLenum CheckOpenGLError(const char* File, int Line);
