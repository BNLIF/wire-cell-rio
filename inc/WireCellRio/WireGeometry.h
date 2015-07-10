#ifndef WIRECELLRIO_WIREGEOMETRY
#define WIRECELLRIO_WIREGEOMETRY

#include "WireCellIface/IWireGeometry.h"
#include "WireCellRioData/GeometryStore.h"

namespace WireCell {
    class RioWireGeometry : public IWireGeometry {
	WireCell::GeomWireSet gws;
    public:
	WireGeometry(const WireCellRio::GeometryStore& store);
	virtual ~WireGeometry();

	const WireCell::GeomWireSet& get_wires() const { return gws; }

    };
}

#endif
