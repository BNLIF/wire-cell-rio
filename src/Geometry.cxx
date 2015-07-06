#include "WireCellRio/RioGDS.h"


using namespace WireCellRio;
using namespace WireCell;

    /// Produce a GeomDataSource from the given GeometryStore
GeomDataSource* store2gds(const GeometryStore& store)
{
}

    /// Produce a GeometryStore from the given GeomDataSource
GeometryStore* gds2store(const GeomDataSource& gds)
{
    
}
    
    /// Save the GeometryStore to the TFile.  Return false on error
bool save_store(TFile* tfile, const WireCellRio::GeometryStore& store,
		const char* treename = "geom")
{
}

    /// Return a GeometryStore from the given TFile/TTree.
GeometryStore* load_store(TFile* tfile, const char* treename="geom")
{

}
