#pragma once

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <vector>

#include "../Core/Application/Logger.h"

namespace Glide3D
{
	class GeometryRenderBuffer
	{
	public :

		GeometryRenderBuffer(uint32_t w, uint32_t h);
		~GeometryRenderBuffer();

		GeometryRenderBuffer(const GeometryRenderBuffer&) = delete;
		GeometryRenderBuffer operator=(GeometryRenderBuffer const&) = delete;

		GeometryRenderBuffer& operator=(GeometryRenderBuffer&& other)
		{
			std::swap(*this, other);
			return *this;
		}

		GeometryRenderBuffer(GeometryRenderBuffer&& v)
		{
			m_Width = v.m_Width;
			m_Height = v.m_Height;

			m_ColorTexture = v.m_ColorTexture;
			m_PositionTexture = v.m_PositionTexture;
			m_NormalTexture = v.m_NormalTexture;
			m_DepthRenderBuffer = v.m_DepthRenderBuffer;
			m_FBO = v.m_FBO;

			v.m_ColorTexture = 0;
			v.m_PositionTexture = 0;
			v.m_NormalTexture = 0;
			v.m_DepthRenderBuffer = 0;
			v.m_FBO = 0;
		}

		void SetDimensions(uint32_t w, uint32_t h);

	private :

		void GenerateFramebuffers();
		void DeleteEverything();

		GLuint m_ColorTexture = 0;
		GLuint m_PositionTexture = 0;
		GLuint m_NormalTexture = 0;
		GLuint m_DepthRenderBuffer = 0;
		GLuint m_FBO = 0;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
	};
}