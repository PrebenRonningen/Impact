#include "ImguiLayer.h"

#include "Graphics/Graphics.h"
namespace Impact
{
	ImguiLayer::ImguiLayer()
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
		dt;
	}
	void ImguiLayer::Render(Graphics& gfx) const noexcept
	{
		gfx;
	}
}
