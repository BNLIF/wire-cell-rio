#ifndef WIRECELLRIO_HELPERS
#define WIRECELLRIO_HELPERS


#include "WireCellRioData/GeometryStore.h"
#include "WireCellIface/IWireGeometry.h"
#include "WireCellIface/ICellGeometry.h"
#include "WireCellIface/ICellTiling.h"

#include "TTree.h"

namespace WireCellRio {

    /// Convert the data provided by the Wire Cell interfaces into the
    /// WireCellRio::GeometryStore.  The store will be emptied of any
    /// pre-existing contents.
    bool convert(WireCellRio::GeometryStore& store,
		 const WireCell::IWireGeometry& wiregeo,
		 const WireCell::ICellGeometry& cellgeo,
		 const WireCell::ICellTiling& tiling);

    /// Helper function to save the GeometryStore to the TFile.  Return false on error
    bool fill(TTree& tree, const WireCellRio::GeometryStore& geostore,
	      const char* branchname="geostore");

    /// Helper function to return a GeometryStore from the given
    /// TFile/TTree.  Caller takes ownership of the returned object.
    WireCellRio::GeometryStore* load_store(TTree& tree, const char* branch="geostore");

}

#endif
