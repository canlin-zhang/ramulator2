#include <fstream>
#include <iostream>
#include <list>
#include <map>

#include "example_ifce.h"
#include "impl/complicated_impl.h"

namespace Ramulator
{

void ComplicatedImpl::special_function()
{
    std::cout << "I am special" << std::endl;
};

} // namespace Ramulator