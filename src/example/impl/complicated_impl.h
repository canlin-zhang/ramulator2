#include <fstream>
#include <iostream>
#include <list>
#include <map>

#include "example_ifce.h"
#include "serialization.h"

namespace Ramulator
{

class ComplicatedImpl : public ExampleIfce, public Implementation
{
    RAMULATOR_REGISTER_IMPLEMENTATION(ExampleIfce, ComplicatedImpl, "ComplicatedImpl",
                                      "An example of a complicated implementation class.");

  public:
    void init() {};
    void special_function();
};

} // namespace Ramulator