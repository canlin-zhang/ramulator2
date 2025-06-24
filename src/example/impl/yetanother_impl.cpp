#include <fstream>
#include <iostream>
#include <list>
#include <map>

#include "example_ifce.h"
#include "impl/complicated_impl.h"

namespace Ramulator
{

class YetAnotherImpl : public ExampleIfce, public Implementation
{
    RAMULATOR_REGISTER_IMPLEMENTATION(ExampleIfce, YetAnotherImpl, "YetAnotherImplementation",
                                      "An example of an implementation class.")

    ComplicatedImpl* m_cimpl;

  public:
    void init() override
    {
        m_cimpl = static_cast<ComplicatedImpl*>(Root::get_impl<ExampleIfce>("ComplicatedImpl"));
        m_cimpl->special_function();
    };
};

} // namespace Ramulator