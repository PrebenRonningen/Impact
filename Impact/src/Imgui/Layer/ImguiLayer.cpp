#include "ImguiLayer.h"

#include "Graphics/Graphics.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
namespace Impact
{
	ImguiLayer::ImguiLayer()
		: m_Enabled{true}
		, show_demo_window{true}
		, m_SpeedFactor{1.f}
	{
	}
	void ImguiLayer::OnAttach(Impact::Graphics& gfx)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(gfx.GetWindowHandle());
		ImGui_ImplDX11_Init(gfx.m_pDevice, gfx.m_pDeviceContext);
	}

	void ImguiLayer::OnDetach() noexcept
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
	void ImguiLayer::Update(float dt) noexcept
	{
		if (!m_Enabled) return;
		dt;
	}
	static char buffer[1024];
	void ImguiLayer::Render(Graphics& ) noexcept
	{

	}
	void ImguiLayer::ImGuiRender() noexcept
	{
		if (!m_Enabled) return;
		if (ImGui::Begin("Simulation Speed"))
		{
			ImGui::DragFloat("Speed Factor", &m_SpeedFactor, 0.01f, -5.0f, 5.0f, "%.3f");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		ImGui::End();
	}
	void ImguiLayer::Begin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	void ImguiLayer::End()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	void ImguiLayer::Dissable()
	{
		m_Enabled = false;
	}
	void ImguiLayer::Enable()
	{
		m_Enabled = true;
	}
	bool ImguiLayer::IsEnabled()
	{
		return m_Enabled;
	}
}
