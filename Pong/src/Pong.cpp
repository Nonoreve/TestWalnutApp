#pragma clang diagnostic push
#pragma ide diagnostic ignored "NullDereference"
#pragma ide diagnostic ignored "DanglingPointer"
#pragma ide diagnostic ignored "UnusedParameter"

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.hpp"

#include <iostream>

using namespace Walnut;

class PongLayer : public Walnut::Layer {
public:
	void OnUIRender() override {
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.1fms (%.1f FPS)", m_LastRenderTime, 1 / m_LastRenderTime * 1000);
		if (ImGui::Button("Render")) {
			Render();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = (uint32_t) ImGui::GetContentRegionAvail().x;// - 1;
		m_ViewportHeight = (uint32_t) ImGui::GetContentRegionAvail().y;// - 1;

		auto image = m_Renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), {(float) image->GetWidth(), (float) image->GetHeight()},
			             ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render() {
		Timer timer;
/*
		if (!m_Image || m_ViewportWidth != m_Image->GetWidth() || m_ViewportHeight != m_Image->GetHeight()) {
			m_Image = std::make_shared<Image>(m_ViewportWidth, m_ViewportHeight, ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_ViewportWidth * m_ViewportHeight];
		}

		for (uint32_t i = 0; i < m_ViewportWidth * m_ViewportHeight; i++) {
			m_ImageData[i] = Random::UInt();
			m_ImageData[i] |= 0xff000000;
		}

		for (uint32_t y = 1; y < m_ViewportHeight - 1; y++) {
			for (uint32_t x = 1; x < m_ViewportWidth - 1; x++) {
				uint32_t ul = m_ImageData[(y - 1) * m_ViewportWidth + (x - 1)];
				uint32_t uc = m_ImageData[(y - 1) * m_ViewportWidth + x];
				uint32_t ur = m_ImageData[(y - 1) * m_ViewportWidth + (x + 1)];
				uint32_t ml = m_ImageData[y * m_ViewportWidth + (x - 1)];
				uint32_t mc = m_ImageData[y * m_ViewportWidth + x];
				uint32_t mr = m_ImageData[y * m_ViewportWidth + (x + 1)];
				uint32_t ll = m_ImageData[(y + 1) * m_ViewportWidth + (x - 1)];
				uint32_t lc = m_ImageData[(y + 1) * m_ViewportWidth + x];
				uint32_t lr = m_ImageData[(y + 1) * m_ViewportWidth + (x + 1)];

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
				m_ImageData[y * m_ViewportWidth + x] = avgColor;
			}
		}

		m_Image->SetData(m_ImageData);*/
		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render();
		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	Renderer m_Renderer;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	float m_LastRenderTime = 0.0f;
};


Walnut::Application *Walnut::CreateApplication(int argc, char **argv) {
	Walnut::ApplicationSpecification spec;
	spec.Name = "Pong";

	auto *app = new Walnut::Application(spec);
	app->PushLayer<PongLayer>();
	app->SetMenubarCallback([app]() {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) {
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}

#pragma clang diagnostic pop
