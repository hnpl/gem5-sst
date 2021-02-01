#include <core/sst_config.h>
#include <Python.h>

#include "gem5.hh"

#include <sim/core.hh>
#include <sim/init.hh>
#include <sim/init_signals.hh>
#include <sim/system.hh>
#include <sim/sim_events.hh>
#include <sim/sim_object.hh>
#include <base/logging.hh>
#include <base/debug.hh>

#ifdef fatal
#undef fatal
#endif


#include <core/timeConverter.h>

SST::gem5::gem5Component::gem5Component(SST::ComponentId_t id, SST::Params &params)
    : SST::Component(id)
{
    dbg.init("@t:gem5:@p():@l " + getName() + ": ", 0, 0,
            (Output::output_location_t)params.find<int>("comp_debug", 0));
    info.init("gem5:" + getName() + ": ", 0, 0, SST::Output::STDOUT);

    SST::TimeConverter *clock = registerClock(
        params.find<std::string>("frequency", "1GHz"),
        new SST::Clock::Handler<SST::gem5::gem5Component>(
            this, &SST::gem5::gem5Component::clockTick
        )
    );

    // how many gem5 cycles will be simulated within an SST clock tick
    gem5_sim_cycles = clock->getFactor();

    // get the command line call to gem5
    std::string cmd = params.find<std::string>("cmd", "");
    if (cmd.empty()) {
        dbg.fatal(CALL_INFO, -1, "Component %s must have a 'cmd' parameter.\n",
            getName().c_str());
    }

    std::vector<char*> args;
    args.push_back(const_cast<char*>("sst.x")); // "what is sst.x???"
    info.output(CALL_INFO, "%s\n", cmd.c_str());
    splitCommandArgs(cmd, args);
    args.push_back(const_cast<char*>("--initialize-only"));
    dbg.output(CALL_INFO, "Command string:  [sst.x %s --initialize-only]\n",
               cmd.c_str());
    info.output(CALL_INFO, "Command string:  [sst.x %s --initialize-only]\n",
               cmd.c_str());
    for (size_t i = 0; i < args.size(); ++i) {
        //dbg.output(CALL_INFO, "  Arg [%02zu] = %s\n", i, args[i]);
        info.output(CALL_INFO, "  Arg [%02zu] = \"%s\"\n", i, args[i]);
    }

    // Initialize m5 special signal handling.
    initSignals();

    initPython(args.size(), &args[0]);

    registerAsPrimaryComponent();
    primaryComponentDoNotEndSim();

    clocks_processed = 0;
}

SST::gem5::gem5Component::~gem5Component()
{
    Py_Finalize();
}

void
SST::gem5::gem5Component::init(unsigned phase)
{
    /*
    for (auto requestor: requestors)
        requestor->init(phase);
    for (auto responder: responders)
        responder->init(phase);
    */
}

void
SST::gem5::gem5Component::setup()
{
    // setup the external ports
    /*
    for (auto requestor: requestors)
        requestor->setup();
    for (auto responder: responders)
        responder->setup();
    */
}

void
SST::gem5::gem5Component::finish()
{
    // signal the ports that the simulation has finished
    /*
    for (auto requestor: requestors)
        requestor->setup();
    */
    info.output("Complete. Clocks Processed: %" PRIu64"\n", clocks_processed);
}

bool
SST::gem5::gem5Component::clockTick(Cycle_t cycle)
{
    // what to do in a SST's Tick

    /*
    for (auto requestor: requestors)
        requestor->clock();
    */
    GlobalSimLoopExitEvent *event = simulate(gem5_sim_cycles);
    clocks_processed++;
    if (event != simulate_limit_event) { // gem5 exits due to reasons other than reaching simulation limit
        info.output("exiting: curTick()=%lu cause=`%s` code=%d\n",
            curTick(), event->getCause().c_str(), event->getCode()
        );
        primaryComponentOKToEndSim();
        return true;
    }

    // returning False means the simulation should go on
    return false;
}

void
SST::gem5::gem5Component::splitCommandArgs(std::string &cmd, std::vector<char*> &args)
{
    const std::array<char, 4> delimiters = { {'\\', ' ', '\'', '\"'} };

    std::vector<std::string> parsed_args1;
    std::vector<std::string> parsed_args2;

    auto prev = &(parsed_args1);
    auto curr = &(parsed_args2);

    curr->push_back(cmd);

    for (auto delimiter: delimiters)
    {
        std::swap(prev, curr);
        curr->clear();

        for (auto part: *prev)
        {
            size_t left = 0;
            size_t right = 0;
            size_t part_length = part.size();

            while (left < part_length)
            {
                while ((left < part_length) && (part[left] == delimiter))
                    left++;
                
                if (!(left < part_length))
                    break;

                right = part.find(delimiter, left);
                if (right == part.npos)
                    right = part_length;

                if (left <= right)
                    curr->push_back(part.substr(left, right-left));
                
                left = right + 1;
            }
        }
    }

    for (auto part: *curr)
        args.push_back(strdup(part.c_str()));
}

void
SST::gem5::gem5Component::initPython(int argc, char *_argv[])
{
    // should be similar to gem5's src/sim/main.cc

    const char * m5MainCommands[] = {
        "import m5",
        "m5.main()",
        0 // sentinel is required
    };

    PyObject *mainModule,*mainDict;

#if PY_MAJOR_VERSION >= 3
    std::unique_ptr<wchar_t[], decltype(&PyMem_RawFree)> program(
        Py_DecodeLocale(_argv[0], NULL),
        &PyMem_RawFree);
    Py_SetProgramName(program.get());
#else
    Py_SetProgramName(_argv[0]);
#endif

    // Register native modules with Python's init system before
    // initializing the interpreter.
    registerNativeModules();

    // initialize embedded Python interpreter
    Py_Initialize();

#if PY_MAJOR_VERSION >= 3
    typedef std::unique_ptr<wchar_t[], decltype(&PyMem_RawFree)> WArgUPtr;
    std::vector<WArgUPtr> v_argv;
    std::vector<wchar_t *> vp_argv;
    v_argv.reserve(argc);
    vp_argv.reserve(argc);
    for (int i = 0; i < argc; i++) {
        v_argv.emplace_back(Py_DecodeLocale(_argv[i], NULL), &PyMem_RawFree);
        vp_argv.emplace_back(v_argv.back().get());
    }

    wchar_t **argv = vp_argv.data();
#else
    char **argv = _argv;
#endif

    PySys_SetArgv(argc, argv);

    mainModule = PyImport_AddModule("__main__");
    assert(mainModule);

    mainDict = PyModule_GetDict(mainModule);
    assert(mainDict);

    PyObject *result;
    const char **command = m5MainCommands;

    while (*command) {
        result = PyRun_String(*command, Py_file_input, mainDict, mainDict);
        if (!result) {
            PyErr_Print();
            break;
        }
        Py_DECREF(result);

        command++;
    }

}