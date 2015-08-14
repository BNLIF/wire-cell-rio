#include "WireCellIface/IGeometry.h"
#include "WireCellIface/IConfigurable.h"

#include "WireCellUtil/Testing.h"
#include "WireCellUtil/NamedFactory.h"
#include "WireCellUtil/TimeKeeper.h"
#include "WireCellUtil/MemUsage.h"

#include <iostream>
using namespace std;
using namespace WireCell;


int main()
{
    TimeKeeper tk("test rio");
    MemUsage mu("test rio");

    WIRECELL_NAMEDFACTORY_USE(ParamGeometry);
    WIRECELL_NAMEDFACTORY_USE(RioGeomSink);

    tk("made factories");
    mu("made factories");

    auto geom = WireCell::Factory::lookup<IGeometry>("ParamGeometry");

    tk("Geometry made");
    mu("Geometry made");

    auto rio_cfg = WireCell::Factory::lookup<IConfigurable>("RioGeomSink");
    auto cfg = rio_cfg->default_configuration();
    cfg.put("file", "test_rio.root");
    cout << configuration_dumps(cfg) << endl;
    rio_cfg->configure(cfg);

    tk("RIO configured");
    mu("RIO configured");

    auto rio_sink = WireCell::Factory::lookup<IGeomSink>("RioGeomSink");
    AssertMsg(rio_sink, "Failed to get RioGeomSink as IGeomSink");
    rio_sink->sink(geom);

    tk("Geometry sunk to ROOT");
    mu("Geometry sunk to ROOT");

    cout << "Time summary:\n" << tk.summary() << endl;
    cout << "Memory usage:\n" << mu.summary() << endl;
}
