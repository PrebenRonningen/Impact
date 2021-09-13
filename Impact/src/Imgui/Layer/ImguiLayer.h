#pragma once
#include "Core\Layer.h"

#include <imgui.h>

namespace Impact
{
	class ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		virtual ~ImguiLayer() noexcept override = default;
		virtual void OnAttach(Impact::Graphics& gfx) override;
		virtual void OnDetach() noexcept override;
		virtual void Update(float dt) noexcept override;
		virtual void Render(Graphics& gfx) noexcept override;
		virtual void ImGuiRender() noexcept override;

		void Begin();
		void End();

		void Dissable();
		void Enable();
		bool IsEnabled();

		float GetSpeedFactor(){return m_SpeedFactor;}
	private:
		bool m_Enabled;

		// test
		bool show_demo_window;
		float m_SpeedFactor;
	};
}

