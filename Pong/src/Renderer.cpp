//
// Created by nonoreve on 09/07/22.
//
#include "Renderer.hpp"

#include "Walnut/Random.h"

using namespace Walnut;

void Renderer::OnResize(uint32_t width, uint32_t height) {
	if (m_FinalImage) {
		// No resize necessary
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height);
	} else {
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

void Renderer::Render() {
	for (uint32_t i = 0; i < m_FinalImage->GetWidth() * m_FinalImage->GetHeight(); i++) {
		m_ImageData[i] = Random::UInt();
		m_ImageData[i] |= 0xff000000;
	}

	for (uint32_t y = 1; y < m_FinalImage->GetHeight() - 1; y++) {
		for (uint32_t x = 1; x < m_FinalImage->GetWidth() - 1; x++) {
			uint32_t ul = m_ImageData[(y - 1) * m_FinalImage->GetWidth() + (x - 1)];
			uint32_t uc = m_ImageData[(y - 1) * m_FinalImage->GetWidth() + x];
			uint32_t ur = m_ImageData[(y - 1) * m_FinalImage->GetWidth() + (x + 1)];
			uint32_t ml = m_ImageData[y * m_FinalImage->GetWidth() + (x - 1)];
			uint32_t mc = m_ImageData[y * m_FinalImage->GetWidth() + x];
			uint32_t mr = m_ImageData[y * m_FinalImage->GetWidth() + (x + 1)];
			uint32_t ll = m_ImageData[(y + 1) * m_FinalImage->GetWidth() + (x - 1)];
			uint32_t lc = m_ImageData[(y + 1) * m_FinalImage->GetWidth() + x];
			uint32_t lr = m_ImageData[(y + 1) * m_FinalImage->GetWidth() + (x + 1)];

			// BLUE
			uint32_t avgBlue = 0;
			avgBlue += ul & 0x00ff0000;
			avgBlue += uc & 0x00ff0000;
			avgBlue += ur & 0x00ff0000;
			avgBlue += ml & 0x00ff0000;
			avgBlue += mc & 0x00ff0000;
			avgBlue += mr & 0x00ff0000;
			avgBlue += ll & 0x00ff0000;
			avgBlue += lc & 0x00ff0000;
			avgBlue += lr & 0x00ff0000;
			avgBlue /= 9; // TODO not sure it works

			// GREEEN
			uint32_t avgGreen = 0;
			avgGreen += ul & 0x0000ff00;
			avgGreen += uc & 0x0000ff00;
			avgGreen += ur & 0x0000ff00;
			avgGreen += ml & 0x0000ff00;
			avgGreen += mc & 0x0000ff00;
			avgGreen += mr & 0x0000ff00;
			avgGreen += ll & 0x0000ff00;
			avgGreen += lc & 0x0000ff00;
			avgGreen += lr & 0x0000ff00;
			avgGreen /= 9;

			// RED
			uint32_t avgRed = 0;
			avgRed += ul & 0x000000ff;
			avgRed += uc & 0x000000ff;
			avgRed += ur & 0x000000ff;
			avgRed += ml & 0x000000ff;
			avgRed += mc & 0x000000ff;
			avgRed += mr & 0x000000ff;
			avgRed += ll & 0x000000ff;
			avgRed += lc & 0x000000ff;
			avgRed += lr & 0x000000ff;
			avgRed /= 9;

			uint32_t avgColor = 0xff000000;
			avgColor += avgBlue & 0x00ff0000;
			avgColor += avgGreen & 0x0000ff00;
			avgColor += avgRed & 0x000000ff;
			m_ImageData[y * m_FinalImage->GetWidth() + x] = avgColor;
		}
	}

	/*
for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
	for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
		glm::vec2 coord = {(float) x / (float) m_FinalImage->GetWidth(),
						   (float) y / (float) m_FinalImage->GetHeight()};
		coord = coord * 2.0f - 1.0f; // -1 -> 1
		m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coord);
	}
}*/

	m_FinalImage->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(glm::vec2 coord) {
	glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
	glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	float radius = 0.5f;

	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	float discriminant = b * b - 4.0f * a * c;
	if (discriminant >= 0.0f)
		return 0xffff00ff;

	return 0xff000000;
}
