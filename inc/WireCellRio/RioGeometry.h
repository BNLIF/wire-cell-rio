#ifndef WIRECELLRIO_RIOGEOMETRY
#define WIRECELLRIO_RIOGEOMETRY

#include "WireCellIface/IGeometry.h"
#include "WireCellRio/RioGeomFile.h"


#include <string>

namespace WireCell {

    /// An IGeometry which comes from a Rio file.
    class RioGeometry : public IGeometry, public WireCell::RioGeomFile
    {
    public:
	RioGeometry();
	virtual ~RioGeometry();

	virtual IWire::vector wires();
	virtual ICell::vector cells();
	virtual ITiling::pointer tiling();
	virtual IWireSummary::pointer wire_summary();

    private:
	void initialize();

	IWire::vector m_wires;
	ICell::vector m_cells;

	IWireSummary::pointer m_wiresummary;
	ITiling::pointer m_tiling;

    };


}

#endif

