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
