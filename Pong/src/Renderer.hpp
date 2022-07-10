//
// Created by nonoreve on 09/07/22.
//

#ifndef PONG_RENDERER_HPP
#define PONG_RENDERER_HPP
#pragma once

#include "Walnut/Image.h"

#include <memory>
#include <glm/glm.hpp>

using glm::vec2;

class Renderer {
private:
	static uint32_t PerPixel(vec2 coord, vec2 leftPad, vec2 rightPad);
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t *m_ImageData = nullptr;

public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render(vec2 leftPad, vec2 rightPad);

	[[nodiscard]] std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }
};

#endif //PONG_RENDERER_HPP
