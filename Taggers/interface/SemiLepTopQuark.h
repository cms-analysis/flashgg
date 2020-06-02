/* 
 * File:   SemiLepTopQuark.h
 * Author: nadjieh
 *
 * Created on February 21, 2012, 6:03 PM
 */

#ifndef FLASHgg_SEMILEPTOPQUARK_H
#define	FLASHgg_SEMILEPTOPQUARK_H
#include <iostream>
#include "TLorentzVector.h"
#include "TRandom1.h"
#include "TVector3.h"
#include "TH1.h"
#include "TMath.h"
#include "Math/GenVector/VectorUtil.h"

using namespace std;
using namespace ROOT;

namespace flashgg {

class SemiLepTopQuark {
public:
  SemiLepTopQuark(TLorentzVector b, TLorentzVector mis, TLorentzVector Mu, TLorentzVector b2, TLorentzVector FwD, TH1D * res = NULL, int v = 0, bool nuCalc_ = true, bool solutionfound = false) :
    bJet(b), unTagged(b2), FwDJet(FwD), met(mis), mu(Mu), resolutions(res), verbosity(v), nuCalc(nuCalc_), nuSolutionFound_FixedWmass(solutionfound) {
    goodEvent = true;
    if (nuCalc) {
      met_W = this->neutrino_W();
      //met_MET = this->neutrino_MET();
    } else {
      met_W.SetPxPyPzE(-1, -1, -1, -1);
      met_MET.SetPxPyPzE(-1, -1, -1, -1);
    }
    muonCharge = 1000;

    //        met.SetPxPyPzE(nu.Px(),nu.Py(),nu.Pz(),sqrt(nu.Px()*nu.Px()+nu.Py()*nu.Py()+nu*nu));    
    };

  SemiLepTopQuark() :
    bJet(-1, -1, -1, -1), unTagged(-1, -1, -1, -1), FwDJet(-1, -1, -1, -1), met(-1, -1, -1, -1), mu(-1, -1, -1, -1), resolutions(0), verbosity(0) {
    goodEvent = true;
    muonCharge = 1000;
  };

  ~SemiLepTopQuark() {//delete resolutions;
  };

  bool hasFixedWmassNuSolution()const {
    return nuSolutionFound_FixedWmass;
  }

  bool hasNeutrinoSolution()const {
    return this->hasFixedWmassNuSolution();
  }

  bool keepEvent()const {
    return goodEvent;
  }

  TLorentzVector neutrino_MET() {
    nuSolutionFound_FixedWmass = false;
    double mw = 80.44;
    float solution = -10000;
    double Delta = delta(mw, mu, met);
    TLorentzVector nut;
    if (verbosity > 0)
      cout << "The first delta value is " << Delta << endl;
    if (Delta >= 0.0) {
      if (verbosity > 0)
	cout << "so I immediately calculate pz.";
      solution = pZNeutrino(mw, mu, met , Delta);
      if (verbosity > 0)
	cout << "\n\tsolution = " << solution << endl;
      nut.SetPxPyPzE(met.Px(), met.Py(), solution, sqrt(met.Px() * met.Px() + met.Py() * met.Py() + solution * solution));
      nuSolutionFound_FixedWmass = true;
      return nut;
    }
    //        if(verbosity > 0)
    //            cout<<"so I try first the MET smearing ..."<<endl;
    for (int i = 0; i < 1000; i++) {
      mw = 80.44;
      TLorentzVector tmpMet = smearedMET();
      if (verbosity > 0)
	cout << "after MET smearing\n\t" << tmpMet.Px() << "\t" << tmpMet.Py() << endl;
      Delta = delta(mw, mu, tmpMet);
      if (verbosity > 0)
	cout << "after MET smearing\n\tDelta: " << Delta << endl;
      if (Delta >= 0.0) {
	if (verbosity > 0)
	  cout << "The delta from MET smearing becomes " << Delta << endl;
	solution = pZNeutrino(mw, mu, tmpMet , Delta);
	if (verbosity > 0)
	  cout << "\n\tsolution = " << solution << endl;
	nut.SetPxPyPzE(tmpMet.Px(), tmpMet.Py(), solution, sqrt(tmpMet.Px() * tmpMet.Px() + tmpMet.Py() * tmpMet.Py() + solution * solution));
	break;
      }
      if (verbosity > 0)
	cout << "MET smearing did not help. Once again ..." << endl;
      //                
    }
    if (solution == -10000) {
      std::cout << "MET does not have solution even after MET smearing.\n";
      std::cout << "The solution is returned as if Delta was zero ..." << std::endl;
      mw = 80.44;
      solution = (A(mw, mu, met) * mu.Pz()) / pow(mu.Pt(), 2);
      nut.SetPxPyPzE(met.Px(), met.Py(), solution, sqrt(met.Px() * met.Px() + met.Py() * met.Py() + solution * solution));
    }
    return nut;
  }

  TLorentzVector neutrino_W() {
    nLoopsToSolve = 0;
    nuSolutionFound_FixedWmass = false;
    goodEvent = true;
    double mw = 80.44;
    float solution = NAN;
    TLorentzVector nut;
    double Delta = 0;
    solution = pZNeutrino(mw, mu, met , Delta);
    //cout << "First try : (delta,solution)" << Delta << "," << solution << endl;
    if( Delta >= 0){
      nut.SetPxPyPzE(met.Px(), met.Py(), solution, sqrt(met.Px() * met.Px() + met.Py() * met.Py() + solution * solution));
      nuSolutionFound_FixedWmass = true;
      return nut;
    }

    for (nLoopsToSolve = 1; nLoopsToSolve < 1001; nLoopsToSolve++) {
      mw = smearedWmass();
      solution = pZNeutrino(mw, mu, met , Delta);
      //cout << nLoopsToSolve << " try : (delta,solution)" << Delta << "," << solution << endl;
      if (Delta >= 0){
	nut.SetPxPyPzE(met.Px(), met.Py(), solution, sqrt(met.Px() * met.Px() + met.Py() * met.Py() + solution * solution));
	return nut;
      }
    }

    nLoopsToSolve = 2000;
    goodEvent = false;
    //std::cout << "MET does not have solution even after mW smearing.\n";
    //std::cout << "The solution is returned as if Delta was zero ..." << std::endl;
    mw = 80.44;
    solution = (A(mw, mu, met) * mu.Pz()) / (mu.Pt()*mu.Pt());
    //cout << "Last result " << solution << endl;
    nut.SetPxPyPzE(met.Px(), met.Py(), solution, sqrt(met.Px() * met.Px() + met.Py() * met.Py() + solution * solution));
    
    return nut;
  }

  TLorentzVector W(int skim = 1)const {
    //        cout<<"skim: "<<skim<<endl;
    if (skim == 2)
      return (met_MET + mu);
    if (skim == 1)
      return (met_W + mu);
    //        cout<<met.Pt()<<" "<<met.Px()<<" "<<met.Py()<<" "<<met.Pz()<<" "<<met.E()<<endl;
    //        cout<<mu.Pt()<<" "<<mu.Px()<<" "<<mu.Py()<<" "<<mu.Pz()<<" "<<mu.E()<<endl;
    return met + mu;
  }

  TLorentzVector top(int skim = 1)const {
    //        cout<<"skim in top: "<<skim<<endl;
    //        cout<<bJet.Px()<<" "<<bJet.Py()<<" "<<bJet.Pz()<<" "<<bJet.E()<<endl;
    return (this->W(skim) + bJet);
  }
  //    double cosThetaStar(int skim = 1)const{
  //        TLorentzVector w = this->W(skim);
  //        if(verbosity>0){
  //            cout<<"W boson:"<<w.Px()<<", "<<w.Py()<<", "<<w.Pz()<<endl;
  //        }
  //        TLorentzVector muClone = mu;
  //        TLorentzVector Top = this->top(skim);
  //        if(verbosity>0){
  //            cout<<"top quark:"<<Top.Px()<<", "<<Top.Py()<<", "<<Top.Pz()<<endl;
  //        }
  //        muClone.Boost(- w.BoostVector());
  //        w.Boost(-Top.BoostVector());
  //        if(verbosity>0){
  //            cout<<"Boosted mu:"<<muClone.Px()<<", "<<muClone.Py()<<", "<<muClone.Pz()<<endl;
  //            cout<<"Boosted W:"<<w.Px()<<", "<<w.Py()<<", "<<w.Pz()<<endl;
  //        }
  //        if(ROOT::Math::VectorUtil::CosTheta(w,muClone)>1.0)
  //            cout<<"=================== what a hell!!! "<<endl;
  //        return ROOT::Math::VectorUtil::CosTheta(w,muClone);
  //        /*const TVector3 w3 = w.Vect(); const TVector3 mu3 = muClone.Vect(); 
  //        double res = (double)(w3.Dot(mu3))/(double)(w3.Mag()*mu3.Mag());      
  //        return res;*/
  //    }

  double cosThetaStar(int skim = 1)const {
    TLorentzVector w = this->W(skim);
    if (verbosity > 0) {
      cout << "W boson:" << w.Px() << ", " << w.Py() << ", " << w.Pz() << endl;
    }
    TLorentzVector muIntop = mu;
    TLorentzVector WIntop = w;
    TLorentzVector Top = this->top(skim);
    WIntop.Boost(-Top.BoostVector());
    muIntop.Boost(-Top.BoostVector());
    muIntop.Boost(-WIntop.BoostVector());
    if (verbosity > 0) {
      cout << "top quark:" << Top.Px() << ", " << Top.Py() << ", " << Top.Pz() << endl;
    }
    if (verbosity > 0) {
      cout << "Boosted mu:" << muIntop.Px() << ", " << muIntop.Py() << ", " << muIntop.Pz() << endl;
      cout << "Boosted W:" << WIntop.Px() << ", " << WIntop.Py() << ", " << WIntop.Pz() << endl;
    }
    if (ROOT::Math::VectorUtil::CosTheta(WIntop, muIntop) > 1.0)
      cout << "=================== what a hell!!! " << endl;
    return ROOT::Math::VectorUtil::CosTheta(WIntop, muIntop);
    /*const TVector3 w3 = w.Vect(); const TVector3 mu3 = muClone.Vect(); 
      double res = (double)(w3.Dot(mu3))/(double)(w3.Mag()*mu3.Mag());      
      return res;*/
  }

  //May be N and T directions ....

  TLorentzVector getMET(int skim = 1)const {
    if (skim == 2)
      return met_MET;
    else if (skim == 1)
      return met_W;
    return met;
  }

  TLorentzVector getbJet()const {
    return bJet;
  }

  TLorentzVector getunTagged()const {
    return unTagged;
  }

  TLorentzVector getFwDJet() const {
    return FwDJet;
  }

  TLorentzVector getMuon()const {
    return mu;
  }

  void setMET(TLorentzVector MET) {
    met.SetPxPyPzE(MET.Px(), MET.Py(), MET.Pz(), MET.T());
  }

  void setbJet(TLorentzVector jet) {
    bJet = jet;
  }

  void setunTagged(TLorentzVector jet) {
    unTagged = jet;
  }

  void setFwDJet(TLorentzVector jet) {
    FwDJet = jet;
  }

  void setMuon(TLorentzVector muon) {
    mu = muon;
  }

  void setVerbosity(int v) {
    verbosity = v;
  }

  void printContent() {
    cout << "lepton  : " << mu.Px() << ", " << mu.Py() << ", " << mu.Pz() << ", " << mu.Pt() << endl;
    cout << "b       : " << bJet.Px() << ", " << bJet.Py() << ", " << bJet.Pz() << ", " << bJet.Pt() << endl;
    cout << "light b : " << unTagged.Px() << ", " << unTagged.Py() << ", " << unTagged.Pz() << ", " << unTagged.Pt() << endl;
    cout << "FwDJet  : " << FwDJet.Px() << ", " << FwDJet.Py() << ", " << FwDJet.Pz() << ", " << FwDJet.Pt() << endl;
    cout << "neutrino: " << met.Px() << ", " << met.Py() << ", " << met.Pz() << ", " << met.Pt() << endl;
  }

  void setMuCharge(int c) {
    muonCharge = c;
  }

  int MuCharge()const {
    return muonCharge;
  }

  int nLoopsToSolve;
  bool goodEvent; // based on nu solution
private:

  double A(double mw, TLorentzVector muon, TLorentzVector Emis) {
    //        cout<<"mw*mw/2: "<<mw*mw/2<<"\nmuon.Px()*Emis.Px()+muon.Py()*Emis.Py(): "<<muon.Px()*Emis.Px()+muon.Py()*Emis.Py()<<endl;
    //        cout<<"A: "<<mw*mw/2+(muon.Px()*Emis.Px()+muon.Py()*Emis.Py())<<endl;
    return ( (mw * mw / 2) + (muon.Px() * Emis.Px() + muon.Py() * Emis.Py()));
  }

  double delta(double mw, TLorentzVector muon, TLorentzVector Emis) {
    //        cout<<"Delta: "<<(muon.E()*muon.E()*A(mw,muon,Emis)*A(mw,muon,Emis)
    //                -muon.Pt()*muon.Pt()*Emis.Pt()*Emis.Pt()*muon.E()*muon.E())<<endl;
    double a ;
    return ( delta(mw, muon, Emis, a) );
  }

  double delta(double mw, TLorentzVector muon, TLorentzVector Emis , double& a) {
    //        cout<<"Delta: "<<(muon.E()*muon.E()*A(mw,muon,Emis)*A(mw,muon,Emis)
    //                -muon.Pt()*muon.Pt()*Emis.Pt()*Emis.Pt()*muon.E()*muon.E())<<endl;
    a = A(mw, muon, Emis);
    return ( (muon.Pz() * muon.Pz() * a * a)
	     - (muon.Pt() * muon.Pt() 
		* (Emis.Pt() * Emis.Pt() * muon.E() * muon.E() - a * a ) 
		) 
	     );
  }

  double pZ(double mw, TLorentzVector muon, TLorentzVector Emis, bool plusSolution , double a , double Delta) {
    double res = -1000;
    if (plusSolution)
      res = ( (a * mu.Pz()) + sqrt(Delta)) / (mu.Pt()*mu.Pt());
    else
      res = ( (a * mu.Pz()) - sqrt(Delta)) / (mu.Pt()*mu.Pt());
    return res;
  }

  float pZNeutrino(double mw, TLorentzVector muon, TLorentzVector Emis , double& Delta) {
    double a ;
    Delta = delta(mw, muon, Emis , a);
    if( Delta < 0 )
      return NAN;
    else{
      double pminus = (pZ(mw, muon, Emis, false , a , Delta));
      double pplus = (pZ(mw, muon, Emis, true , a , Delta));

      if(fabs(pminus) < fabs(pplus) )
	return pminus;
      else
	return pplus;
    }
  }

  TLorentzVector smearedMET() {
    double resolution;
    if (met.Pt() < 30) {
      resolution = resolutions->GetBinContent(1) * met.Pt();
      if (verbosity > 0) {
	cout << "MET: " << met.Pt() << ", resoution: " << resolutions->GetBinContent(1) << endl;
      }
    } else if (met.Pt() >= 30 && met.Pt() < 40) {
      resolution = resolutions->GetBinContent(2) * met.Pt();
      if (verbosity > 0) {
	cout << "MET: " << met.Pt() << ", resoution: " << resolutions->GetBinContent(2) << endl;
      }
    } else if (met.Pt() >= 40 && met.Pt() < 50) {
      resolution = resolutions->GetBinContent(3) * met.Pt();
      if (verbosity > 0) {
	cout << "MET: " << met.Pt() << ", resoution: " << resolutions->GetBinContent(3) << endl;
      }
    } else if (met.Pt() >= 50 && met.Pt() < 60) {
      resolution = resolutions->GetBinContent(4) * met.Pt();
      if (verbosity > 0) {
	cout << "MET: " << met.Pt() << ", resoution: " << resolutions->GetBinContent(4) << endl;
      }
    } else if (met.Pt() >= 60 && met.Pt() < 80) {
      resolution = resolutions->GetBinContent(5) * met.Pt();
      if (verbosity > 0) {
	cout << "MET: " << met.Pt() << ", resoution: " << resolutions->GetBinContent(5) << endl;
      }
    } else if (met.Pt() >= 80 && met.Pt() < 100) {
      resolution = resolutions->GetBinContent(6) * met.Pt();
      if (verbosity > 0) {
	cout << "MET: " << met.Pt() << ", resoution: " << resolutions->GetBinContent(6) << endl;
      }
    } else {
      resolution = resolutions->GetBinContent(7) * met.Pt();
      if (verbosity > 0) {
	cout << "MET: " << met.Pt() << ", resoution: " << resolutions->GetBinContent(7) << endl;
      }
    }
    TRandom1 r1;
    double metX = r1.Gaus(met.Px(), resolution);
    double metY = r1.Gaus(met.Py(), resolution);
    TLorentzVector tmpMet(metX, metY, 0, sqrt(metX * metX + metY * metY));
    return tmpMet;
  }

  double smearedWmass() {
    TRandom1 r;
    return r.BreitWigner(80.44, 2.1);
  }

  TLorentzVector bJet;
  TLorentzVector unTagged;
  TLorentzVector FwDJet;
  TLorentzVector met;
  TLorentzVector mu;
  TH1D * resolutions;
  int verbosity;
  bool nuCalc;
  TLorentzVector met_MET;
  TLorentzVector met_W;
  bool nuSolutionFound_FixedWmass;

  int muonCharge;
};

}
#endif	/* SEMILEPTOPQUARK_H */

