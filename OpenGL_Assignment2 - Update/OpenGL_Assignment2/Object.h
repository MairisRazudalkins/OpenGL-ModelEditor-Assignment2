#pragma once
class Object
{

public:
	Object() = default;
	
	virtual void Render() = 0;
	virtual void Update(float deltaTime) = 0;
};