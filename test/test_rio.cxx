#include "WireCellIface/IGeometry.h"
#include "WireCellIface/IWireParameters.h"
#include "WireCellIface/IWireGenerator.h"
#include "WireCellIface/IWire.h"
#include "WireCellIface/ICell.h"
#include "WireCellIface/ITiling.h"
#include "WireCellIface/IConfigurable.h"

#include "WireCellUtil/Testing.h"
#include "WireCellUtil/NamedFactory.h"

#include <iostream>
using namespace std;

using namespace WireCell;


class MyGeometry : IGeometry {
    IWireSequence::pointer m_wires;
    ICellSequence::pointer m_cells;
    IWireSummary::pointer m_wiresummary;
    ITiling::pointer m_tiling;
public:

    virtual IWireSequence::pointer wires() { return m_wires; }
    virtual ICellSequence::pointer cells() { return m_cells; }
    virtual ITiling::pointer tiling() { return m_tiling; }
    virtual IWireSummary::pointer wire_summary() { return m_wiresummary; }

    MyGeometry() {
	auto wp_wps = WireCell::Factory::lookup<IWireParameters>("WireParams");
	auto pw_gen = WireCell::Factory::lookup<IWireGenerator>("ParamWires");
	pw_gen->generate(*wp_wps);
	m_wires = WireCell::Factory::lookup<IWireSequence>("ParamWires");
	IWireSink::pointer bc_sink = WireCell::Factory::lookup<IWireSink>("BoundCells");
	bc_sink->sink(m_wires->wires_range());
	m_cells = WireCell::Factory::lookup<ICellSequence>("BoundCells");
	//m_tiling = WireCell::Factory::lookup<ITiling>("GraphTiling");
	m_tiling = 0;
    }

    virtual ~MyGeometry() {}

};

int main()
{
    WIRECELL_NAMEDFACTORY_USE(WireParams);
    WIRECELL_NAMEDFACTORY_USE(ParamWires);
    WIRECELL_NAMEDFACTORY_USE(BoundCells);
    //WIRECELL_NAMEDFACTORY_USE(GraphTiling);
    WIRECELL_NAMEDFACTORY_USE(RioGeomSink);

    MyGeometry geom;

    auto rio_cfg = WireCell::Factory::lookup<IConfigurable>("RioGeomSink");


}
