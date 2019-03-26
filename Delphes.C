#define Delphes_cxx
#include "Delphes.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void Delphes::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Delphes.C
//      root> Delphes t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   TH1F *invariantMass = new TH1F("invMass", "Invariant Mass", 100, 0, 1000);
   TFile *f = new TFile("invariant_mass.root", "RECREATE");

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if (false)
      {
         fChain->SetBranchStatus("*", 0);
         fChain->SetBranchStatus("Jet.PT", 1);
         fChain->SetBranchStatus("Jet.Eta", 1);
         fChain->SetBranchStatus("Jet.Phi", 1);
         fChain->SetBranchStatus("Jet.Mass", 1);
         fChain->SetBranchStatus("Electron.PT", 1);
         fChain->SetBranchStatus("Electron.Eta", 1);
         fChain->SetBranchStatus("Electron.Phi", 1);
         fChain->Show();
      }
      TLorentzVector jet;
      TLorentzVector electron;
      jet.SetPtEtaPhiM(Jet_PT[0], Jet_Eta[0], Jet_Phi[0], Jet_Mass[0]);
      electron.SetPtEtaPhiM(Electron_PT[0], Electron_Eta[0], Electron_Phi[0], Electron_mass);
      
      TLorentzVector invariant = jet + electron;
      
      invariantMass->Fill(invariant.M());
   }
   invariantMass->Write();
   f->Close();
}
