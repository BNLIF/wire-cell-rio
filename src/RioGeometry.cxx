#include "WireCellRio/RioGeometry.h"
#include "WireCellUtil/NamedFactory.h"

using namespace WireCell;

WIRECELL_NAMEDFACTORY(RioGeometry) {
    WIRECELL_NAMEDFACTORY_INTERFACE(RioGeometry, IConfigurable);
    WIRECELL_NAMEDFACTORY_INTERFACE(RioGeometry, IGeometry);
}

RioGeometry::RioGeometry()
    : RioGeomFile()
    , m_wires(0)
    , m_cells(0)
    , m_wiresummary(0)
    , m_tiling(0)
{
}
RioGeometry::~RioGeometry()
{
}

class RioWire : public IWire {
    int m_ident, m_index, m_channel;
    WirePlaneId m_wpid;
    Ray m_ray;
public:
    RioWire(WireCellRio::Wire& w, PointVector& points)
	: m_ident(w.ident)
	, m_index(w.index)
	, m_channel(w.channel)
    {
	switch (w.plane) {
	case 0: m_wpid = WirePlaneId(kUlayer); break;
	case 1: m_wpid = WirePlaneId(kVlayer); break;
	case 2: m_wpid = WirePlaneId(kWlayer); break;
	default: m_wpid =  WirePlaneId(kUnknownLayer); break;
	}
	m_ray = Ray(points[w.point1], points[w.point2]);
    }
	
    virtual int ident() const {
	return m_ident;
    }

    virtual WirePlaneId planeid() const {
	return m_wpid;
    }

    virtual int index() const {
	return m_index;
    }

    virtual int channel() const {
	return m_channel;
    }
    virtual int segment() const {
	return 0;		// fixme, need to add support!
    }

    virtual int face() const {
	return 0;		// fixme, need to add support!
    }

    virtual int apa() const {
	return 0;		// fixme, need to add support!
    }

    virtual WireCell::Ray ray() const {
	return m_ray;
    }
};

class RioCell : public ICell {
    int m_ident;
    PointVector m_corners;
    IWireVector m_wires;

public:

    RioCell(WireCellRio::Cell& cell, IWireVector& wires, PointVector& points)
	: m_ident(cell.ident)
    {
	for (int ind : cell.corners) {
	    m_corners.push_back(points[ind]);
	}
	m_wires.push_back(wires[cell.uid]);
	m_wires.push_back(wires[cell.vid]);
	m_wires.push_back(wires[cell.wid]);
    }

    virtual int ident() const {
	return m_ident;
    }

    virtual double area() const {
	return 0;		// fixme
    }
    
    virtual WireCell::Point center() const {
	return Point();		// fixme
    }

    virtual WireCell::PointVector corners() const {
	return m_corners;
    }

    virtual WireCell::IWireVector wires() const {
	return m_wires;
    }

};


void RioGeometry::initialize()
{
    if (m_tiling) { return; }
    
    WireCellRio::GeometryStore store;
    this->RioGeomFile::load(store);

    PointVector temp_points;
    for (auto rp : store.points) {
	temp_points.push_back(Point(rp.x, rp.y, rp.z));
    }

    IWireVector temp_wires;
    for (auto wire : store.wires) {
	temp_wires.push_back(IWire::pointer(new RioWire(wire, temp_points)));
    }

    ICellVector temp_cells;
    for (auto cell : store.cells) {
	temp_cells.push_back(ICell::pointer(new RioCell(cell, temp_wires, temp_points)));
    }

    m_wires = IWireSequence::pointer(new IWireCollection<IWireVector>(temp_wires));
    m_cells = ICellSequence::pointer(new ICellCollection<ICellVector>(temp_cells));
    
    auto tiling = WireCell::Factory::lookup<ICellSink>("Tiling");
    tiling->sink(m_cells->cells_range());
    m_tiling = WireCell::Factory::lookup<ITiling>("Tiling");
}
IWireSequence::pointer RioGeometry::wires() 
{
    initialize();
    return m_wires;
}

ICellSequence::pointer RioGeometry::cells() 
{
    initialize();
    return m_cells;
}
ITiling::pointer RioGeometry::tiling() 
{
    initialize();
    return m_tiling;
}
IWireSummary::pointer RioGeometry::wire_summary() 
{
    if (m_wiresummary) { return m_wiresummary; }
    auto wire_sink = WireCell::Factory::lookup<IWireSink>("WireSummary");
    wire_sink->sink(wires()->wires_range());
    m_wiresummary = WireCell::Factory::lookup<IWireSummary>("WireSummary");
    return m_wiresummary;
}
