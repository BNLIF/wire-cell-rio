#include "WireCellRio/WireGeometry.h"
#include "WireCellData/GeomWire.h"


WireCellRio::WireGeometry::WireGeometry(const WireCellRio::GeometryStore& store)
{
    for (auto wit = store.wires.begin(); wit != store.wires.end(); ++wit) {
	const WireCellRio::Wire& rio = *wit;
	const WireCellRio::Point& p1 = store.points[wit->point1];
	const WireCellRio::Point& p2 = store.points[wit->point2];

	gws.insert(WireCell::GeomWire(rio.ident, rio.plane, rio.index, rio.chid,
				      WireCell::Point(p1.x,p1.y,p1.z),
				      WireCell::Point(p2.x,p2.y,p2.z)));

    }
}

WireCellRio::WireGeometry::~WireGeometry()
{
}

