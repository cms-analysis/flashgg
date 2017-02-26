import FWCore.ParameterSet.Config as cms

all_pdfs = cms.PSet(
passingPresel_0p0To0p9_1p479To2p5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[-0.1799,-5.000,5.000],sigmaP[1.197,0.001,5.000])",
"RooGaussian::signalResFail(mass, meanF[-0.4442,-5.000,5.000],sigmaF[1.17,0.001,5.000])",
"ZGeneratorLineShape::signalPhyPass(mass,\"templatePreselEE.root\", \"hMass_0.0To0.90_1.479To2.5_Pass\")",
"ZGeneratorLineShape::signalPhyFail(mass,\"templatePreselEE.root\", \"hMass_0.0To0.90_1.479To2.5_Fail\")",
"RooBernstein::backgroundPass(mass,{a0P[0.006,-1.0.,1.0],a1P[0.011,-1.5,1.5],a2P[0.0063,-1.5,1.5],a3F[0.0,-1.5,1.5]})",
"RooBernstein::backgroundFail(mass,{a0F[0.006,-1.0.,1.0],a1F[0.011,-1.5,1.5],a2F[0.0063,-1.5,1.5],a3F[0.0,-1.5,1.5]})",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.5,0,1]",
"signalFractionInPassing[1.0]"
),

passingPresel_0p9To1p0_1p479To2p5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[0.093,-5.000,5.000],sigmaP[1.291,0.001,5.000])",
"RooGaussian::signalResFail(mass, meanF[-0.1498,-5.000,5.000],sigmaF[1.32,0.001,5.000])",
"ZGeneratorLineShape::signalPhyPass(mass,\"templatePreselEE.root\", \"hMass_0.90To1.0_1.479To2.5_Pass\")",
"ZGeneratorLineShape::signalPhyFail(mass,\"templatePreselEE.root\", \"hMass_0.90To1.0_1.479To2.5_Fail\")",
"RooBernstein::backgroundPass(mass,{a0P[0.006,-1.0.,1.0],a1P[1.,-1.5,1.5],a2P[1.,-1.5,1.5]})",
"RooBernstein::backgroundFail(mass,{a0F[0.006,-1.0.,1.0],a1F[1.,-1.5,1.5],a2F[1.,-1.5,1.5]})",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.5,0,1]",
"signalFractionInPassing[1.0]"
),

)
