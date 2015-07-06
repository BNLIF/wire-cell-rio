#ifndef WIRECELLRIO_GEOMETRY
#define WIRECELLRIO_GEOMETRY

#include "WireCellNav/GeomDataSource.h"
#include "WireCellRioData/GeometryStore.h"

#include "WireCellIface/IWireGeometry.h"
#include "WireCellIface/ICellGeometry.h"

#include "TFile.h"

namespace WireCellRio {


    class Geometry : public WireCell::IWireGeometry,
		     public WireCell::ICellGeometry {
    public:
	Geometry(const GeometryStore& geostore);
	virtual ~Geometry();

	/// Return all wires (IWireGeometry)
	const WireCell::GeomWireSet& get_wires() const;

	/// Return all cells (ICellGeometry)
	const WireCell::GeomCellSet& get_cells() const;

    };

}

#endif
