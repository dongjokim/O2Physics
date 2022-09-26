// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
/// \author Dong Jo Kim (djkim@jyu.fi)
/// \since Sep 2022

#ifndef JFFlucAnalysis_cxx
#define JFFlucAnalysis_cxx

//#include <AliAnalysisTaskSE.h>
//#include "JEfficiency.h"
#include "JHistManager.h"
#include <Math/Vector4D.h>
#include <Math/LorentzVector.h>
#include <TComplex.h>
//#include <TF3.h>

using namespace ROOT;
using namespace ROOT::Math;

// TODO: needs test before switching
/*class JTrackNew : PtEtaPhiEVector{
public:
  JTrackNew(){};
  ~JTrackNew(){};
};*/

class JFFlucAnalysis
{ // : public AliAnalysisTaskSE {
 public:
  JFFlucAnalysis();
  JFFlucAnalysis(const char* name);
  JFFlucAnalysis(const JFFlucAnalysis& a);             // not implemented
  JFFlucAnalysis& operator=(const JFFlucAnalysis& ap); // not implemented

  virtual ~JFFlucAnalysis();
  virtual void UserCreateOutputObjects();
  virtual void Init();
  virtual void UserExec(Option_t* option);
  virtual void Terminate(Option_t*);

  void SetInputList(std::vector<PtEtaPhiEVector>* _fInputList) { fInputList = _fInputList; }
  // void SetInputList(TClonesArray *inputarray){fInputList = inputarray;}
  void SetEventCentralityAndBin(float cent, UInt_t cbin)
  {
    fCent = cent;
    fCBin = cbin;
  }
  float GetEventCentrality() const { return fCent; }
  void SetEventImpactParameter(float ip) { fImpactParameter = ip; }
  void SetEventVertex(const double* vtx) { fVertex = vtx; }

  void SetEtaRange(double eta_min, double eta_max)
  {
    fEta_min = eta_min;
    fEta_max = eta_max;
  }
  void SetEventTracksQA(unsigned int tpc, unsigned int glb)
  {
    fTPCtrks = (float)tpc;
    fGlbtrks = (float)glb;
  }
  void SetEventFB32TracksQA(unsigned int fb32, unsigned int fb32tof)
  {
    fFB32trks = (float)fb32;
    fFB32TOFtrks = (float)fb32tof;
  }

  void Fill_QA_plot(double eta1, double eta2);

  // new function for QC method //
  void CalculateQvectorsQC(double, double);
  TComplex Q(int n, int p);
  TComplex Two(int n1, int n2);
  TComplex Four(int n1, int n2, int n3, int n4);

  enum SUBEVENT {
    kSub_Event_A = 0x1,
    kSub_Event_B = 0x2
  };
  void SelectSubevents(UInt_t _subeventMask)
  {
    subeventMask = _subeventMask;
  }
  // set the number of bins before initialization (UserCreateOutputObjects)
  void SetNumBins(UInt_t _numBins)
  {
    numBins = _numBins;
  }
  enum {
    kFluc_Phi_Correction = 0x2,
    kFluc_Ebe_Weightening = 0x4
  };
  void AddFlags(UInt_t _flags)
  {
    flags |= _flags;
  }

  /*static Double_t CentBin_PbPb_default[][2];
  static Double_t MultBin_PbPb_1[][2];
  static Double_t MultBin_pPb_1[][2];
  static Double_t (*pBin[3])[2];*/
  static Double_t pttJacek[74];
  static UInt_t NpttJacek;

  enum { kH0,
         kH1,
         kH2,
         kH3,
         kH4,
         kH5,
         kH6,
         kH7,
         kH8,
         kH9,
         kH10,
         kH11,
         kH12,
         kNH }; // harmonics
  enum { kK0,
         kK1,
         kK2,
         kK3,
         kK4,
         nKL };  // order
#define kcNH kH6 // max second dimension + 1
 private:
  std::vector<PtEtaPhiEVector>* fInputList;
  // TClonesArray *fInputList;
  // JEfficiency *fEfficiency;
  const Double_t* fVertex; //!
  Float_t fCent;
  Float_t fImpactParameter;
  UInt_t fCBin;
  float fTPCtrks;
  float fGlbtrks;
  float fFB32trks;
  float fFB32TOFtrks;
  UInt_t subeventMask;
  UInt_t numBins; // total number of bins
  UInt_t flags;

  double fEta_min;
  double fEta_max;
  // double NSubTracks[2];

  TComplex QvectorQC[kNH][nKL];
  TComplex QvectorQCeta10[2][kNH][nKL]; // ksub

  JHistManager* fHMG; //!

  JBin fBin_Subset;  //!
  JBin fBin_h;       //!
  JBin fBin_k;       //!
  JBin fBin_hh;      //!
  JBin fBin_kk;      //!
  JBin fHistCentBin; //!
  JBin fVertexBin;   //! // x, y, z
  JBin fCorrBin;     //!

  JTH1D fh_cent;            //! // for cent dist
  JTH1D fh_ImpactParameter; //! // for impact parameter for mc
  JTH1D fh_vertex;          //!
  JTH1D fh_pt;              //! // for pt dist of tracks
  JTH1D fh_eta;             //! // for eta dist of tracks
  JTH1D fh_phi;             //! // for phi dist [ic][isub]
  JTH2D fh_phieta;          //!
  JTH3D fh_phietaz;         //!
  // JTH1D fh_Qvector;//! // for Q-Vector dist [ic][isub][ih]

  JTH1D fh_psi_n;       //!
  JTH1D fh_cos_n_phi;   //!
  JTH1D fh_sin_n_phi;   //!
  JTH1D fh_cos_n_psi_n; //!
  JTH1D fh_sin_n_psi_n; //!

  JTH1D fh_ntracks; //! // for number of tracks dist
  JTH1D fh_vn;      //!  // single vn^k  array [ih][ik][iCent]
  JTH1D fh_vna;     //! // single vn^k with autocorrelation removed (up to a limited order)
  JTH1D fh_vn_vn;   //! // combination for <vn*vn> [ih][ik][ihh][ikk][iCent]
  /*JTH1D fh_cn_4c;//!  // QC
  JTH1D fh_cn_2c;//!  // QC
  JTH1D fh_cn_cn_2c;//! // QC
  JTH1D fh_cn_2c_eta10;//!  // QC
  JTH1D fh_cn_cn_2c_eta10;//! // QC*/

  JTH1D fh_correlator;            //! // some more complex correlators
  JTH2D fh_TrkQA_TPCvsGlob;       //! // QA histos
  JTH2D fh_TrkQA_TPCvsCent;       //! // QA histos
  JTH2D fh_TrkQA_FB32_vs_FB32TOF; //!

  // additional variables for ptbins(Standard Candles only)
  enum { kPt0,
         kPt1,
         kPt2,
         kPt3,
         kPt4,
         kPt5,
         kPt6,
         kPt7,
         N_ptbins };
  double NSubTracks_pt[2][N_ptbins];
  JBin fBin_Nptbins;       //!
  JTH1D fh_SC_ptdep_4corr; //! // for < vn^2 vm^2 >
  JTH1D fh_SC_ptdep_2corr; //!  // for < vn^2 >
  // additinal variables for SC with QC
  JTH1D fh_SC_with_QC_4corr;       //! // for <vn^2 vm^2>
  JTH1D fh_SC_with_QC_2corr;       //! // for <vn^2>
  JTH1D fh_SC_with_QC_2corr_eta10; //!
  // JTH2D fh_QvectorQC;//! // check for Q-vec dist for [ic][ih]
  // JTH1D fh_QvectorQCphi;//!
  JTH1D fh_evt_SP_QC_ratio_2p; //! // check SP QC evt by evt ratio
  JTH1D fh_evt_SP_QC_ratio_4p; //! // check SP QC evt by evt ratio
                               // ClassDef(JFFlucAnalysis, 1);
};

#endif
