//#include "duneanasel/nd/ndlar/Selections.h"

#include "duneanaobj/StandardRecord/StandardRecord.h"

#include "TChain.h"
#include "TFile.h"
#include "TSystem.h"

int main(int argc, char const *argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <caf file(s)>" << std::endl;
    return 1;
  }

  TChain ch("cafTree");

  for(int i = 1; i < argc; ++i){
    ch.Add(argv[i]);
  }

  caf::StandardRecord *SR = nullptr;
  if (!ch.GetBranch("rec")) {
    std::cerr << "Error: branch 'rec' not found in cafTree."
              << " Check input file content and loaded dictionaries." << std::endl;
    return 2;
  }
  ch.SetBranchAddress("rec", &SR);

  Long64_t ents = ch.GetEntries();
  if (ents <= 0) {
    std::cerr << "Error: no entries found in cafTree." << std::endl;
    return 3;
  }

  if (ch.GetEntry(0) <= 0 || SR == nullptr) {
    std::cerr << "Error: failed to read first entry from cafTree rec branch."
              << " Dictionaries may be missing at runtime." << std::endl;
    return 4;
  }

  for (Long64_t i = 0; i < ents; ++i) {
    if (ch.GetEntry(i) <= 0 || SR == nullptr) {
      continue;
    }

    if (SR->common.ixn.dlp.empty()) {
      continue;
    }

    std::cout << SR->common.ixn.dlp[0].vtx.x << std::endl;

    // for (auto const &nd_int : SR->common.ixn.dlp) {
    //   auto longp = ana::GetLongestParticle(nd_int);
    //   //float vtx_x = nd_int.vtx.x;
    //   //std::cout << vtx_x << std::endl;
    // }

  }
  return 0;
}