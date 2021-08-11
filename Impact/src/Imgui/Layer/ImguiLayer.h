#pragma once
#include "Core\Layer.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

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
		virtual void Render(Graphics& gfx) const noexcept override;

		void Begin();
		void End();


	};
}

