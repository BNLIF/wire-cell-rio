#ifndef WIRECELLRIO_GEOMETRY
#define WIRECELLRIO_GEOMETRY

#include "WireCellNav/GeomDataSource.h"
#include "WireCellRioData/GeometryStore.h"

#include "WireCellIface/IWireGeometry.h"
#include "WireCellIface/ICellGeometry.h"
#include "WireCellIface/ICellTiling.h"

#include "TFile.h"

namespace WireCellRio {


    class Geometry : public WireCell::IWireGeometry,
		     public WireCell::ICellGeometry,
		     public WireCell::ICellTiling {
    public:
	Geometry(const GeometryStore& geostore);
	virtual ~Geometry();

	/// IWireGeometry interface

	/// Return all wires (IWireGeometry)
	const WireCell::GeomWireSet& get_wires() const;

	/// ICellGeometry interface

	/// Return all cells (ICellGeometry)
	const WireCell::GeomCellSet& get_cells() const;

	/// ICellTiling interface

	/// Must return all wires associated with the given cell
	GeomWireSelection wires(const GeomCell& cell) const;
	
	/// Must return all cells associated with the given wire
	GeomCellSelection cells(const GeomWire& wire) const;

	/// Must the one cell associated with the collection of wires or 0.
	const GeomCell* cell(const GeomWireSelection& wires) const;

    private:
    };

}

#endif
