#pragma once
class Layer
{
public:
	virtual ~Layer() = default;
	virtual void OnAttach() = 0;
	virtual void OnDetatch() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() const = 0;
};

