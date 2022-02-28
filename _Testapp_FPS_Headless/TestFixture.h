#pragma once
#include <string>

namespace Plato
{
	class Renderer;
}

using namespace Plato;

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
