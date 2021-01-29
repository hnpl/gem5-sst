#ifdef fatal
#undef fatal
#endif

#include <core/sst_config.h>
#include <Python.h>

#include "gem5.hh"

#include <core/timeConverter.h>

SST::gem5::gem5Component::gem5Component(SST::ComponentId_t id, SST::Params &params)
    : SST::Component(id)
{
    info.init("gem5:" + getName() + ": ", 0, 0, SST::Output::STDOUT);

    info.output("hello, world!");

    //SST::TimeConverter *clock = 
}

SST::gem5::gem5Component::~gem5Component()
{
    Py_Finalize();
}

void
SST::gem5::gem5Component::init(unsigned phase)
{
}

void
SST::gem5::gem5Component::setup()
{
}

void
SST::gem5::gem5Component::finish()
{

}

bool
SST::gem5::gem5Component::clockTick(Cycle_t cycle)
{

}

void
SST::gem5::gem5Component::splitCommandArgs(std::string &cmd, std::vector<char*> &args)
{

}

void
SST::gem5::gem5Component::initPython(int argc, char *argv[])
{

}