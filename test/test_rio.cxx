#include "WireCellRio/Helpers.h"

#include "WireCellNav/ExampleWires.h"
#include "WireCellNav/GeomDataSource.h"

#include "WireCellTiling/GraphTiling.h"

#include "WireCellIface/IWireGeometry.h"

using namespace WireCell;

int main()
{
    IWireGeometry* wires = make_example_wires(10*units::mm);
    GeomDataSource gds;
    gds.use_wires(*wires);

    WireCell::GraphTiling tiling(gds);
    
}
