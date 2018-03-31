#include <sentinel/TestRunner.h>
#include <sugar/ParserTest.h>

using namespace sugar;
using namespace lithic::sentinel;

int main() {
	TestRunner* testRunner = new TestRunner();
	ParserTest* parserTest = new ParserTest();
	testRunner->add(parserTest);

	return testRunner->run() ? 0 : 1;
}