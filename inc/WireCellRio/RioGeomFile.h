#ifndef WIRECELLRIO_RIOMGEOFILECFG
#define WIRECELLRIO_RIOMGEOFILECFG

#include "WireCellIface/IConfigurable.h"
#include "WireCellRioData/GeometryStore.h"

namespace WireCell {

    /// A configurable ROOT file/tree/branch that can save to or load
    /// from a file a Rio geometry store.
    class RioGeomFile : public IConfigurable {
    public:
	RioGeomFile();
	virtual ~RioGeomFile();

	/** Configurable interface. */
	virtual void configure(const WireCell::Configuration& config);
	virtual WireCell::Configuration default_configuration() const;

	/** Raw setting of configs. */
	void set(const std::string& tfilename = "rio.root",
		 const std::string& treename = "riogeom",
		 const std::string& branchname = "geom");

	/// Save the store to the configured file/tree/branch.  This
	/// is not const because ROOT is not const correct.
	void save(WireCellRio::GeometryStore& store);

	/// Load the configured file/tree/branch into the store.
	void load(WireCellRio::GeometryStore& store);
	

    private:
	std::string m_fname, m_tname, m_bname;

    };
}

#endif
