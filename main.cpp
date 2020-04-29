#include <Driver.hpp>
#include <iostream>
using namespace std;
using namespace kaleidoscope;
int main() {
    // Parser parser(cin, cerr);
    // Printer printer(cout);
    // parser.update_token();
    // auto b_e = parser.function();
    // b_e->accept(printer);
    Driver driver(cin, cout, cerr);
    driver.loop();
    return 0;
}