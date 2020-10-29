#include "SSAO.h"

Glide3D::SSAOHandler::SSAOHandler()
{
	std::uniform_real_distribution<float> RandomFloatGenerator(0.0, 1.0);
	std::default_random_engine Generator;

	for (int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(
			RandomFloatGenerator(Generator) * 2.0 - 1.0,
			RandomFloatGenerator(Generator) * 2.0 - 1.0,
			RandomFloatGenerator(Generator)
		);

		sample = glm::normalize(sample);
		sample *= RandomFloatGenerator(Generator);

		float scale = (float)i / 64.0f;
		scale = 0.1f + (scale * scale) * (1.0f - 0.1f);
		sample *= scale;

		m_SSAOKernel.push_back(sample);
	}

	for (int i = 0; i < 16; i++)
	{
		glm::vec3 noise(RandomFloatGenerator(Generator) * 2.0 - 1.0, RandomFloatGenerator(Generator) * 2.0 - 1.0,
			0.0f);
		m_SSAONoise.push_back(noise);
	}

	glGenTextures(1, &m_SSAOTexture);
	glBindTexture(GL_TEXTURE_2D, m_SSAOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0, GL_RGB, GL_FLOAT, &m_SSAONoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Generate the ssao fbo
	glGenFramebuffers(1, &m_SSAOFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOFBO);
	glGenTextures(1, &m_SSAOColorbuffer);
	glBindTexture(GL_TEXTURE_2D, m_SSAOColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 800, 600, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SSAOColorbuffer, 0);
}
