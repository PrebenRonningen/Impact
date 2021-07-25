#pragma once
namespace Impact
{
	class Graphics;
	class Layer
	{
	public:
		virtual ~Layer() noexcept = default;
		virtual void OnAttach(Impact::Graphics& gfx) = 0;
		virtual void OnDetach() noexcept = 0;
		virtual void Update(float dt) noexcept = 0;
		virtual void Render(Graphics& gfx) const noexcept = 0;
	};
}