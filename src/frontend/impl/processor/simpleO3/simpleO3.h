#pragma once
#include "frontend.h"
#include "impl/processor/simpleO3/core.h"
#include "impl/processor/simpleO3/llc.h"
#include "translation.h"
#include "utils.h"

namespace Ramulator
{

class SimpleO3 final : public IFrontEnd, public Implementation
{
    RAMULATOR_REGISTER_IMPLEMENTATION(IFrontEnd, SimpleO3, "SimpleO3",
                                      "Simple timing model OoO processor frontend.")

  private:
    ITranslation* m_translation;

    int m_num_cores = -1;
    std::vector<SimpleO3Core*> m_cores;
    SimpleO3LLC* m_llc;

    size_t m_num_expected_insts = 0;

    std::string serialization_filename;

  public:
    void init() override;

    void tick() override;

    void receive(Request& req);

    bool is_finished() override;

    void connect_memory_system(IMemorySystem* memory_system) override;

    int get_num_cores() override;
};

} // namespace Ramulator