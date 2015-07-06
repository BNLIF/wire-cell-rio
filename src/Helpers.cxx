#include "WireCellData/GeomWire.h"
#include "WireCellData/GeomCell.h"

#include "WireCellRio/Helpers.h"

#include "WireCellRioData/GeometryStore.h"

#include "WireCellUtil/IndexedSet.h"



bool WireCellRio::convert(WireCellRio::GeometryStore& store,
			  const WireCell::IWireGeometry& wiregeo,
			  const WireCell::ICellGeometry& cellgeo,
			  const WireCell::ICellTiling& tiling)
{
    // The IDs used to store references rely on being indices into
    // these vectors.  We don't support passing in any pre-filled
    // vectors.
    store.cells.clear();
    store.wires.clear();
    store.points.clear();

    WireCell::IndexedSet<WireCell::Point> point_index;
    WireCell::IndexedSet<WireCell::GeomWire> wire_index;
    WireCell::IndexedSet<WireCell::GeomCell> cell_index;

    /// Pre index the wires directly.  In principle, this is redundant
    /// as we also index the wires as we go through the cells.
    /// However doing this pre-indexing will retain the original
    /// ordering in the wire store.
    {
	auto gwires = wiregeo.get_wires();
	for (auto gwit = gwires.begin(); gwit != gwires.end(); ++gwit) {
	    wire_index(*gwit);
	    point_index(gwit->point1());
	    point_index(gwit->point2());
	}
    }

    // Convert the cells
    auto gcells = cellgeo.get_cells();
    for (auto gcit = gcells.begin(); gcit != gcells.end(); ++gcit) {
	const WireCell::GeomCell& gcell = *gcit;
	cell_index(gcell);

	// copy over cell
	WireCellRio::Cell rcell;
	rcell.ident = gcell.ident();
	auto boundary = gcell.boundary();
	for (auto bit = boundary.begin(); bit != boundary.end(); ++bit) {
	    rcell.corners.push_back(point_index(*bit));
	}

	WireCell::GeomWireSelection wgs = tiling.wires(gcell);
	for (auto wit=wgs.begin(); wit != wgs.end(); ++wit) {
	    const WireCell::GeomWire* wire = *wit;
	    int index = wire_index(*wire);
	    if (wire->plane() == WireCell::kUwire) {
		rcell.uid = index;
	    }
	    if (wire->plane() == WireCell::kVwire) {
		rcell.vid = index;
	    }
	    if (wire->plane() == WireCell::kYwire) {
		rcell.wid = index;
	    }
	}
	store.cells.push_back(rcell);
    }


    {				// now actually convert wires
	for (auto gwit = wire_index.collection.begin(); gwit != wire_index.collection.end(); ++gwit) {
	    WireCellRio::Wire rio;
	    rio.ident = gwit->ident();
	    rio.plane = gwit->iplane();
	    rio.index = gwit->index();
	    rio.channel = gwit->channel();
	    rio.point1 = point_index(gwit->point1());
	    rio.point2 = point_index(gwit->point2());
	    store.wires.push_back(rio);
	}
    }

    {				// and finally the points
	for (auto gpit = point_index.collection.begin(); gpit != point_index.collection.end(); ++gpit) {
	    store.points.push_back(WireCellRio::Point(gpit->x, gpit->y, gpit->z));
	}
    }

    return true;
}


/// Helper function to save the GeometryStore to the TFile.  Return false on error
bool WireCellRio::fill(TTree& tree, const WireCellRio::GeometryStore& geostore,
		       const char* branchname)
{
    const WireCellRio::GeometryStore* gptr = &geostore;
    tree.Branch(branchname, &gptr);
    tree.Fill();
}



/// Helper function to return a GeometryStore from the given
/// TFile/TTree.  Caller takes ownership of the returned object.
WireCellRio::GeometryStore* WireCellRio::load_store(TTree& tree, const char* branchname)
{
    WireCellRio::GeometryStore* store = new WireCellRio::GeometryStore();
    tree.SetBranchAddress(branchname, &store);
    tree.GetEntry(0);
    tree.SetBranchAddress(branchname, 0);
    return store;    
}


