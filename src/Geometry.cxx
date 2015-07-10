#include "WireCellRio/Geometry.h"

using namespace WireCellRio;


Geometry::Geometry(const GeometryStore& geostore)
{
}


Geometry::~Geometry()
{
}


const WireCell::GeomWireSet& Geometry::get_wires() const
{

}


const WireCell::GeomCellSet& Geometry::get_cells() const
{
}


GeomWireSelection Geometry::wires(const GeomCell& cell) const
{
}
	

GeomCellSelection Geometry::cells(const GeomWire& wire) const
{
}


const GeomCell* Geometry::cell(const GeomWireSelection& wires) const
{
}




