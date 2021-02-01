#ifndef EXT_SST_GEM5_HH
#define EXT_SST_GEM5_HH

#include <string>
#include <vector>

#include <core/sst_config.h>
#include <core/component.h>

#include <sim/simulate.hh>

//#include "ExtMaster.hh"
//#include "ExtSlave.hh"

namespace SST {
namespace gem5 {

class gem5Component : public SST::Component
                      //public ExternalSlave::Handler,
                      //public ExternalMaster::Handler 
{
private:

    Output dbg;
    Output info;
    uint64_t gem5_sim_cycles;
    uint64_t clocks_processed;

    //std::vector<ExtMaster*> masters;
    //std::vector<ExtSlave*> slaves;

    void splitCommandArgs(std::string &cmd, std::vector<char*> &args);
    void initPython(int argc, char **argv);

public:
    gem5Component(ComponentId_t id, Params &params);
    ~gem5Component();
    virtual void init(unsigned);
    virtual void setup();
    virtual void finish();
    bool clockTick(Cycle_t);

    // REGISTER THIS COMPONENT INTO THE ELEMENT LIBRARY
    SST_ELI_REGISTER_COMPONENT(
        gem5Component,
        "gem5",
        "gem5",
        SST_ELI_ELEMENT_VERSION(1,0,0),
        "gem5",
        COMPONENT_CATEGORY_UNCATEGORIZED
    )

    SST_ELI_DOCUMENT_PARAMS(
        {"cmd", "gem5 command to execute."},
        {"comp_debug", "Debug information from the component: 0 (off), 1 (stdout), 2 (stderr), 3(file)"},
        {"frequency", "Frequency with which to call into gem5"}
    )

/*
    virtual ExternalMaster::ExternalPort *getExternalPort(
        const std::string &name, ExternalMaster &owner,
        const std::string &port_data);

    virtual ExternalSlave::ExternalPort *getExternalPort(
        const std::string &name, ExternalSlave &owner,
        const std::string &port_data);
*/
};

}
}

#endif