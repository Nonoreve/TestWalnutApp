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

	PongLayer() : m_LeftPad(-1.2, 0), m_RightPad(1.2, 0) {}
	void OnUIRender() override {
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.1fms (%.1f FPS)", m_LastRenderTime, 1 / m_LastRenderTime * 1000);
		if (ImGui::Button("Up") && m_RightPad.y < 0.7f) {
			m_RightPad.y += 0.04f;
		}
		if (ImGui::Button("Down") && m_RightPad.y > -0.7f) {
			m_RightPad.y -= 0.04f;
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = (uint32_t) ImGui::GetContentRegionAvail().x - 1;
		m_ViewportHeight = (uint32_t) ImGui::GetContentRegionAvail().y - 1;

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

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_LeftPad, m_RightPad);
		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	Renderer m_Renderer;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	float m_LastRenderTime = 0.0f;

	// game specific stuff TODO move to other class
	vec2 m_LeftPad;
	vec2 m_RightPad;
};


Walnut::Application *Walnut::CreateApplication(int argc, char **argv) {
	Walnut::ApplicationSpecification spec;
	spec.Name = "Pong";
	spec.Width = 1000;
	spec.Height = 562;

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
