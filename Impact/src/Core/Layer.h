#pragma once
namespace Impact
{
	class Layer
	{
	public:
		virtual ~Layer() noexcept = default;
		virtual void OnAttach() noexcept = 0;
		virtual void OnDetach() noexcept = 0;
		virtual void Update(float dt) noexcept = 0;
		virtual void Render() const noexcept = 0;
	};
}