//root
//.L invariant_histogram_root.cxx
//writeRootFile(readRootFile())

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <TLorentzVector.h>

using namespace std;

float momentum(const TLorentzVector p)
{
    return sqrt(pow(p.Px(), 2) + pow(p.Py(), 2) + pow(p.Pz(), 2));
}

float energy(const TLorentzVector p, const float mass)
{
    return sqrt(pow(mass, 2) + pow(momentum(p), 2));
}

float momentumDot(const TLorentzVector p1, const TLorentzVector p2)
{
    return p1.Px() * p2.Px() + p1.Py() * p2.Py() + p1.Pz() * p2.Pz();
}

vector<float> readRootFile()
{
    vector<float> invariant;
    TFile *file = new TFile("tag_1_delphes_events.root");
    TTree *t = (TTree *)file->Get("Delphes");
    float jetPt, jetEta, jetPhi, jetMass, ePt, ePhi, eEta;
    float eMass = 0.567;

    t->SetBranchAddress("Jet.PT", &jetPt);
    t->SetBranchAddress("Jet.Eta", &jetEta);
    t->SetBranchAddress("Jet.Phi", &jetPhi);
    t->SetBranchAddress("Jet.Mass", &jetMass);
    t->SetBranchAddress("Electron.PT", &ePt);
    t->SetBranchAddress("Electron.Eta", &eEta);
    t->SetBranchAddress("Electron.Phi", &ePhi);

    int entries = (int)t->GetEntries();

    for (int i = 0; i < entries; i++)
    {
        t->GetEntry(i);
        TLorentzVector jet;
        TLorentzVector electron;
        jet.SetPtEtaPhiM(jetPt, jetEta, jetPhi, jetMass);
        electron.SetPtEtaPhiM(ePt, eEta, ePhi, eMass);
        float invariantMass = sqrt(pow(jetMass, 2) + pow(eMass, 2) + 2 * 
        (energy(jet, jetMass) * energy(electron, eMass) - momentumDot(jet, electron)));
        invariant.push_back(invariantMass);
    }
    return invariant;
}

void writeRootFile(const vector<float> invariant)
{
    TH1F *h1 = new TH1F("myhisto", "Invariant Mass", 100, 0, 1000);
    TFile *f = new TFile("invariant_mass.root", "RECREATE");
    for (int i = 0; i < invariant.size(); i++)
    {
        h1->Fill(invariant[i]);
    }
    h1->Write();
    f->Close();
}
