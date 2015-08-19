#include "WireCellRio/RioGeometry.h"
#include "WireCellUtil/NamedFactory.h"

using namespace WireCell;

WIRECELL_NAMEDFACTORY(RioGeometry);
WIRECELL_NAMEDFACTORY_ASSOCIATE(RioGeometry, IConfigurable);
WIRECELL_NAMEDFACTORY_ASSOCIATE(RioGeometry, IGeometry);

RioGeometry::RioGeometry()
    : RioGeomFile()
    , m_store(0)
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
    WireCellRio::Wire& m_riowire;
    PointVector& m_points;
public:
    RioWire(WireCellRio::Wire& riowire, PointVector& points)
	: m_riowire(riowire)
	, m_points(points)
    {
    }

    virtual int ident() const {
	return m_riowire.ident;
    }
    virtual WirePlaneType_t plane() const {
	switch (m_riowire.plane) {
	case 0: return WireCell::kUwire;
	case 1: return WireCell::kVwire;
	case 2: return WireCell::kWwire;
	}
	return WireCell::kUnknownWirePlaneType;
    }
	
    virtual int index() const {
	return m_riowire.index;
    }

    virtual int channel() const {
	return m_riowire.channel;
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
	int p1 = m_riowire.point1;
	int p2 = m_riowire.point2;

	return Ray(m_points[p1], m_points[p2]);
    }
};

class RioCell : public ICell {
    WireCellRio::Cell& m_cell;
    IWireVector& m_wires;
    PointVector& m_points;

public:

    RioCell(WireCellRio::Cell& cell, IWireVector& wires, PointVector& points)
	: m_cell(cell)
	, m_wires(wires)
	, m_points(points)
	{ }

    virtual int ident() const {
	return m_cell.ident;
    }

    virtual double area() const {
	return 0;		// fixme
    }
    
    virtual WireCell::Point center() const {
	return Point();		// fixme
    }

    virtual WireCell::PointVector corners() const {
	WireCell::PointVector ret;
	for (int ind : m_cell.corners) {
	    ret.push_back(m_points[ind]);
	}
	return ret;
    }

    virtual WireCell::IWireVector wires() const {
	IWireVector ret;
	ret.push_back(m_wires[m_cell.uid]);
	ret.push_back(m_wires[m_cell.vid]);
	ret.push_back(m_wires[m_cell.wid]);
	return ret;
    }

};

static void get_wires(WireCellRio::GeometryStore& store, IWireVector& wires,
		      PointVector& points)
{
    for (auto wire : store.wires) {
	IWire::pointer p(new RioWire(wire, points));
	wires.push_back(p);
    }
}
static void get_cells(WireCellRio::GeometryStore& store, ICellVector& cells,
		      IWireVector& wires,
		      PointVector& points)
{
    for (auto cell : store.cells) {
	ICell::pointer p(new RioCell(cell, wires, points));
	cells.push_back(p);
    }
}



void RioGeometry::initialize()
{
    if (m_store) return;
    
    m_store = new WireCellRio::GeometryStore;
    this->RioGeomFile::load(*m_store);

    PointVector temp_points;
    for (auto rp : m_store->points) {
	temp_points.push_back(Point(rp.x, rp.y, rp.z));
    }

    IWireVector temp_wires;
    get_wires(*m_store, temp_wires, temp_points);
    ICellVector temp_cells;
    get_cells(*m_store, temp_cells, temp_wires, temp_points);

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
