import FWCore.ParameterSet.Config as cms

all_pdfs = cms.PSet(
passingPresel_0p0To0p85_0p0To1p479 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0,-5.000,5.000],sigmaP[0.5,0.001,5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0,-5.000,5.000],sigmaF[0.5,0.001,5.000])",
"ZGeneratorLineShape::signalPhyPass(mass,\"templatePreselEB.root\", \"hMass_0.0To0.85_0.0To1.479_Pass\")",
"ZGeneratorLineShape::signalPhyFail(mass,\"templatePreselEB.root\", \"hMass_0.0To0.85_0.0To1.479_Fail\")",
"RooBernstein::backgroundPass(mass,{a0P[0.2,-1.0,1.0],a1P[0.,-1.5,1.5],a2P[0.,-1.5,1.5]})",
"RooBernstein::backgroundFail(mass,{a0F[0.2,-1.0,1.0],a1F[0.,-1.5,1.5],a2F[0.,-1.5,1.5]})",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.5,0,1]",
"signalFractionInPassing[1.0]"
),

passingPresel_0p85To1p0_0p0To1p479 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0,-5.000,5.000],sigmaP[0.5,0.001,5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0,-5.000,5.000],sigmaF[0.5,0.001,5.000])",
"ZGeneratorLineShape::signalPhyPass(mass,\"templatePreselEB.root\", \"hMass_0.85To1.0_0.0To1.479_Pass\")",
"ZGeneratorLineShape::signalPhyFail(mass,\"templatePreselEB.root\", \"hMass_0.85To1.0_0.0To1.479_Fail\")",
"RooBernstein::backgroundPass(mass,{a0[0.2,-1.0,1.0],a1[0.,-1.5,1.5],a2[0.,-1.5,1.5],a3[0.,-1.0,1.0]})",
"RooBernstein::backgroundFail(mass,{a0[0.2,-1.0,1.0],a1[0.,-1.5,1.5],a2[0.,-1.5,1.5],a3[0.,-1.0,1.0]})",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.5,0,1]",
"signalFractionInPassing[1.0]"
),

)
