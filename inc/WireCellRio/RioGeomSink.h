#ifndef WIRECELLRIO_RIOGEOMSINK
#define WIRECELLRIO_RIOGEOMSINK

#include "WireCellIface/IGeometry.h"
#include "WireCellIface/IConfigurable.h"

#include <string>

namespace WireCell {

    class RioGeomSink : public IGeomSink
		      , public IConfigurable
    {
    public:
	RioGeomSink();
	virtual ~RioGeomSink();

	/** IGeomSink interface */
	virtual void sink(IGeometry::pointer geo);

	/** Configurable interface. */
	virtual void configure(const WireCell::Configuration& config);
	virtual WireCell::Configuration default_configuration() const;

	/** Raw setting of configs. */
	void set(const std::string& tfilename = "rio.root",
		 const std::string& treename = "riogeom",
		 const std::string& branchname = "geom");

    private:
	std::string m_fname, m_tname, m_bname;
    };


}

#endif

