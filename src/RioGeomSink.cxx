#include "WireCellRio/RioGeomSink.h"
#include "WireCellUtil/NamedFactory.h"
#include "WireCellUtil/IndexedSet.h"

#include <sstream>

using namespace WireCell;

WIRECELL_NAMEDFACTORY(RioGeomSink);
WIRECELL_NAMEDFACTORY_ASSOCIATE(RioGeomSink, IConfigurable);
WIRECELL_NAMEDFACTORY_ASSOCIATE(RioGeomSink, IGeomSink);


WireCell::RioGeomSink::RioGeomSink()
{
    this->set();
}
WireCell::RioGeomSink::~RioGeomSink()
{
}


void WireCell::RioGeomSink::sink(IGeometry::pointer geom)
{
    WireCellRio::GeometryStore store; // fixme put everything under WireCell ns.

    WireCell::IndexedSet<WireCell::Point> point_index;
    WireCell::IndexedSet<WireCell::IWire::pointer> wire_index;
    WireCell::IndexedSet<WireCell::ICell::pointer> cell_index;

    for (auto iw : *geom->wires()) {
	wire_index(iw);
	const WireCell::Ray ray = iw->ray();
	point_index(ray.first);
	point_index(ray.second);
    }

    ITiling::pointer tiling = geom->tiling();

    for (auto ic : *geom->cells()) {
	cell_index(ic);
	WireCellRio::Cell rc;
	rc.ident = ic->ident();
	for (auto point : ic->corners()) {
	    rc.corners.push_back(point_index(point));
	};

	for (auto iw : tiling->wires(ic)) {
	    int index = wire_index(iw);
	    if (iw->planeid().layer() == WireCell::kUlayer) {
		rc.uid = index;
	    }
	    if (iw->planeid().layer() == WireCell::kVlayer) {
		rc.vid = index;
	    }
	    if (iw->planeid().layer() == WireCell::kWlayer) {
		rc.wid = index;
	    }
	}
	store.cells.push_back(std::move(rc));
    }

    for (auto iw : wire_index.collection) {
	WireCellRio::Wire rw;
	rw.ident = iw->ident();
	rw.plane = iw->planeid().index();
	rw.index = iw->index();
	rw.channel = iw->channel();
	const WireCell::Ray ray = iw->ray();
	rw.point1 = point_index(ray.first);
	rw.point2 = point_index(ray.second);
	store.wires.push_back(std::move(rw));
    }

    for (auto pt : point_index.collection) {
	store.points.push_back(WireCellRio::Point(pt.x(), pt.y(), pt.z()));
    }

    this->save(store);
}
