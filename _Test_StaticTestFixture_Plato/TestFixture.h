#pragma once
#include <string>

class Renderer;

class TestFixture
{
public:
	TestFixture(const std::string& testName = "");

	virtual void Update(double frametime) {};
	virtual void Render(Renderer* renderer) {};

	const std::string& GetTestName() const;

private:
	std::string testName;
};
