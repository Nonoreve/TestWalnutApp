//
// Created by nonoreve on 09/07/22.
//

#ifndef PONG_RENDERER_HPP
#define PONG_RENDERER_HPP
#pragma once

#include "Walnut/Image.h"

#include <memory>
#include <glm/glm.hpp>

class Renderer {
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render();

	[[nodiscard]] std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

private:
	static uint32_t PerPixel(glm::vec2 coord);
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t *m_ImageData = nullptr;
};

#endif //PONG_RENDERER_HPP
