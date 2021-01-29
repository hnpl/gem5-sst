#include <core/sst_config.h>

#include "gem5.hh"

static
SST::Component* create_gem5(SST::ComponentId_t id, SST::Params &params)
{
    return new SST::gem5::gem5Component(id, params);
}
