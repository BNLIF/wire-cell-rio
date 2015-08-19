#ifndef WIRECELLRIO_RIOGEOMSINK
#define WIRECELLRIO_RIOGEOMSINK

#include "WireCellIface/IGeometry.h"
#include "WireCellRio/RioGeomFile.h"

#include <string>

namespace WireCell {

    class RioGeomSink : public IGeomSink, public RioGeomFile
    {
    public:
	RioGeomSink();
	virtual ~RioGeomSink();

	/** IGeomSink interface */
	virtual void sink(IGeometry::pointer geo);
    };

}

#endif

