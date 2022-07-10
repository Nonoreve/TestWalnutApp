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

void Renderer::Render(vec2 leftPad, vec2 rightPad) {
	auto aspectRatio = (float) m_FinalImage->GetWidth() / (float) m_FinalImage->GetHeight();
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
			vec2 coord = {(float) x / (float) m_FinalImage->GetWidth(),
			              (float) y / (float) m_FinalImage->GetHeight()};
			// y coords are from -1 to 1, x coords are scaled according to the aspect ratio
			vec2 normalisedCoord = {coord.x * 2.0f * aspectRatio - aspectRatio, coord.y * 2.0f - 1.0f};
			m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(normalisedCoord, leftPad, rightPad);
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(vec2 coord, vec2 leftPad, vec2 rightPad) {
	vec2 padSize = {0.07f, 0.2f};

	// m_LeftPad
	if (coord.x > leftPad.x - padSize.x && coord.x < leftPad.x + padSize.x && coord.y > leftPad.y - padSize.y &&
	    coord.y < leftPad.y + padSize.y)
		return 0xff808080;

	// m_RightPad
	if (coord.x > rightPad.x - padSize.x && coord.x < rightPad.x + padSize.x && coord.y > rightPad.y - padSize.y &&
	    coord.y < rightPad.y + padSize.y)
		return 0xff808080;

	// middle line
	auto lineThickness = 0.008f;
	if (coord.x > -lineThickness && coord.x < lineThickness)
		return 0xff505050;

	// borders
	auto border = 1.3f;
	if (coord.x < -border || coord.x > border)
		return 0xff505050;

	return 0xff000010;
}
