#include "WireCellIface/IGeometry.h"
#include "WireCellIface/IConfigurable.h"

#include "WireCellUtil/Testing.h"
#include "WireCellUtil/NamedFactory.h"
#include "WireCellUtil/ExecMon.h"

#include <boost/range.hpp>

#include <iostream>
using namespace std;
using namespace WireCell;

static int nwires = 0;
static int ncells = 0;

void test_write()
{
    ExecMon em("write rio");

    auto geom = WireCell::Factory::lookup<IGeometry>("ParamGeometry");
    AssertMsg(geom, "Failed to get ParamGeometry as IGeometry");

    nwires = boost::distance(geom->wires()->wires_range());
    ncells = boost::distance(geom->cells()->cells_range());

    em("got input geometry");

    auto rio_cfg = WireCell::Factory::lookup<IConfigurable>("RioGeomSink");
    AssertMsg(rio_cfg, "failed to get configurable for RioGeomSink");

    auto cfg = rio_cfg->default_configuration();
    cfg.put("file", "test_rio.root");
    cout << configuration_dumps(cfg) << endl;
    rio_cfg->configure(cfg);

    em("got rio geom sink");
    auto rio_sink = WireCell::Factory::lookup<IGeomSink>("RioGeomSink");
    AssertMsg(rio_sink, "Failed to get RioGeomSink as IGeomSink");

    rio_sink->sink(geom);
    em("writing done");



    cerr << em.summary() << endl;
}

void test_read()
{
    ExecMon em("read rio");

    auto rio_cfg = WireCell::Factory::lookup<IConfigurable>("RioGeometry");
    AssertMsg(rio_cfg, "Failed to get RioGeometry as configurable");

    em("got rio input");

    auto cfg = rio_cfg->default_configuration();
    cfg.put("file", "test_rio.root");
    cout << configuration_dumps(cfg) << endl;
    rio_cfg->configure(cfg);
    
    auto geom = WireCell::Factory::lookup<IGeometry>("RioGeometry");
    AssertMsg(geom, "Failed to get RioGeometry as IGeometry");

    em("got rio geom");

    int got_nwires = boost::distance(geom->wires()->wires_range());
    AssertMsg(nwires == got_nwires, "Didn't read back the number of wires we wrote");
    int got_ncells = boost::distance(geom->cells()->cells_range());
    AssertMsg(ncells == got_ncells, "Didn't read back the number of cells we wrote");

    vector<int> wids, cids;
    for (auto w : geom->wires()->wires_range()) {
	wids.push_back(w->ident());
    }
    for (auto c : geom->cells()->cells_range()) {
	cids.push_back(c->ident());
    }

    em("accessed geometry");

    cerr << em.summary() << endl;
}

int main()
{
    test_write();
    test_read();

    return 0;
}
