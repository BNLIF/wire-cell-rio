
#include "WireCellRio/RioGeomSink.h"
#include "WireCellRioData/GeometryStore.h"
#include "WireCellUtil/NamedFactory.h"
#include "WireCellUtil/IndexedSet.h"

#include "TFile.h"
#include "TTree.h"

#include <sstream>

using namespace WireCell;

WIRECELL_NAMEDFACTORY(RioGeomSink);
WIRECELL_NAMEDFACTORY_ASSOCIATE(RioGeomSink, IConfigurable);
WIRECELL_NAMEDFACTORY_ASSOCIATE(RioGeomSink, IGeomSink);


WireCell::RioGeomSink::RioGeomSink()
{
    this->set();
}
WireCell::RioGeomSink::~RioGeomSink()
{
}

void WireCell::RioGeomSink::set(const std::string& tfilename,
		   const std::string& treename,
		   const std::string& branchname)
{
    m_fname = tfilename;
    m_tname = treename;
    m_bname = branchname;
}

void WireCell::RioGeomSink::sink(IGeometry::pointer geom)
{
    WireCellRio::GeometryStore store; // fixme put everything under WireCell ns.

    WireCell::IndexedSet<WireCell::Point> point_index;
    WireCell::IndexedSet<WireCell::IWire::pointer> wire_index;
    WireCell::IndexedSet<WireCell::ICell::pointer> cell_index;

    for (auto iw : *geom->wires()) {
	wire_index(iw);
	const WireCell::Ray ray = iw->ray();
	point_index(ray.first);
	point_index(ray.second);
    }

    ITiling::pointer tiling = geom->tiling();

    for (auto ic : *geom->cells()) {
	cell_index(ic);
	WireCellRio::Cell rc;
	rc.ident = ic->ident();
	for (auto point : ic->corners()) {
	    rc.corners.push_back(point_index(point));
	};

	for (auto iw : tiling->wires(ic)) {
	    int index = wire_index(iw);
	    if (iw->plane() == WireCell::kUwire) {
		rc.uid = index;
	    }
	    if (iw->plane() == WireCell::kVwire) {
		rc.vid = index;
	    }
	    if (iw->plane() == WireCell::kYwire) {
		rc.wid = index;
	    }
	}
	store.cells.push_back(std::move(rc));
    }

    for (auto iw : wire_index.collection) {
	WireCellRio::Wire rw;
	rw.ident = iw->ident();
	rw.plane = iw->plane();
	rw.index = iw->index();
	rw.channel = iw->channel();
	const WireCell::Ray ray = iw->ray();
	rw.point1 = point_index(ray.first);
	rw.point2 = point_index(ray.second);
	store.wires.push_back(std::move(rw));
    }

    for (auto pt : point_index.collection) {
	store.points.push_back(WireCellRio::Point(pt.x(), pt.y(), pt.z()));
    }


    TFile* file = TFile::Open(m_fname.c_str(), "RECREATE");
    TTree* tree = new TTree(m_tname.c_str(), "RIO Geometry Tree");
    tree->Branch(m_bname.c_str(), &store);
    tree->Fill();
    tree->Write();
    file->Close();

    delete file;
}

void WireCell::RioGeomSink::configure(const WireCell::Configuration& cfg)
{
    m_fname = cfg.get("file", m_fname);
    m_tname = cfg.get("tree", m_tname);
    m_bname = cfg.get("branch", m_bname);
}
WireCell::Configuration WireCell::RioGeomSink::default_configuration() const
{
    std::stringstream ss;
    std::string q = "\"";

    ss << "{\n";
    ss << q << "file" << q << ":" << q << m_fname << q
       << ",\n"
       << q << "tree" << q << ":" << q << m_tname << q
       << ",\n"
       << q << "branch" << q << ":" << q << m_bname << q
       << "\n}\n";

    return configuration_loads(ss.str(), "json");
}

