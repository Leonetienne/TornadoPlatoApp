#include "TestFixture.h"

TestFixture::TestFixture(const std::string& testName)
    :
    testName { testName }
{
    return;
}

const std::string& TestFixture::GetTestName() const
{
    return testName;
}
