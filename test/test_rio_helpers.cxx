#include "WireCellRio/Helpers.h"

#include "WireCellNav/ExampleWires.h"
#include "WireCellNav/GeomDataSource.h"

#include "WireCellTiling/GraphTiling.h"

#include "WireCellIface/IWireGeometry.h"

#include "WireCellRioData/GeometryStore.h"

#include "WireCellUtil/Testing.h"

#include "TFile.h"


#include <iostream>
using namespace std;

using namespace WireCell;


int main()
{
    IWireGeometry* wiregeo = make_example_wires(10*units::mm);
    GeomDataSource gds;
    gds.use_wires(*wiregeo);

    WireCell::GraphTiling tiling(gds);

    WireCellRio::GeometryStore store;

    bool ok = convert(store, *wiregeo, tiling, tiling);
    
    Assert(ok);

    Assert(store.cells.size() == 53484, "wrong number of cells");
    Assert(store.wires.size() == 330, "wrong number of wires");
    Assert(store.points.size() == 197625, "wrong number of points");

    const char* filename = "test_rio_helpers.root";
    const char* treename = "test_rio";

    TFile* file = TFile::Open(filename, "RECREATE");
    Assert(file, "Failed to open ROOT file for writing");

    TTree* tree = new TTree(treename, "RIO Helpers Test Tree");
    WireCellRio::fill(*tree, store);
    tree->Write();
    file->Close();
    delete file;

    file = TFile::Open(filename, "READ");
    Assert(file, "Failed to open ROOT file for reading");
    
    tree = dynamic_cast<TTree*>(file->Get(treename));
    Assert(tree, "Failed to get tree back");

    WireCellRio::GeometryStore* pstore = WireCellRio::load_store(*tree);
    Assert(pstore, "Failed to get geometry store back");

    Assert(pstore->cells.size() == 53484, "wrong number of cells");
    Assert(pstore->wires.size() == 330, "wrong number of wires");
    Assert(pstore->points.size() == 197625, "wrong number of points");

}
