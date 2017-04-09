#include "WireCellRio/RioGeomFile.h"
#include "WireCellUtil/Persist.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

using namespace WireCell;


RioGeomFile::RioGeomFile()
{
    this->set();
}
RioGeomFile::~RioGeomFile()
{
}


void WireCell::RioGeomFile::set(const std::string& tfilename,
				const std::string& treename,
				const std::string& branchname)
{
    m_fname = tfilename;
    m_tname = treename;
    m_bname = branchname;
}


void RioGeomFile::configure(const WireCell::Configuration& cfg)
{
    m_fname = get(cfg, "file", m_fname);
    m_tname = get(cfg, "tree", m_tname);
    m_bname = get(cfg, "branch", m_bname);
}

WireCell::Configuration RioGeomFile::default_configuration() const
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

    return Persist::loads(ss.str());
}


void RioGeomFile::save(WireCellRio::GeometryStore& store)
{
    TFile* file = TFile::Open(m_fname.c_str(), "UPDATE");
    TTree* tree = new TTree(m_tname.c_str(), "RIO Geometry Tree");
    tree->Branch(m_bname.c_str(), &store);
    tree->Fill();
    tree->Write();
    file->Close();
    delete file;
}

void RioGeomFile::load(WireCellRio::GeometryStore& store)
{
    TFile* file = TFile::Open(m_fname.c_str(), "READ");
    TTree* tree = dynamic_cast<TTree*>(file->Get(m_tname.c_str()));
    WireCellRio::GeometryStore* storeptr = &store;
    tree->SetBranchAddress(m_bname.c_str(), &storeptr);
    tree->GetEntry(0);
    file->Close();
    delete file;
}

