import FWCore.ParameterSet.Config as cms

all_pdfs = cms.PSet(
passingPresel_0p0To0p9_1p479To2p5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0,-5.000,5.000],sigmaP[0.5,0.001,5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0,-5.000,5.000],sigmaF[0.5,0.001,5.000])",
"ZGeneratorLineShape::signalPhyPass(mass,\"templatePreselEE_tag40.root\", \"hMass_0.0To0.90_1.479To2.5_Pass\")",
"ZGeneratorLineShape::signalPhyFail(mass,\"templatePreselEE_tag40.root\", \"hMass_0.0To0.90_1.479To2.5_Fail\")",
"RooCMSShape::backgroundPass(mass, alphaPass[60.,50.,70.], betaPass[0.001, 0.,0.1], gammaPass[0.1, 0, 1], peakPass[90.0, 70, 80])",
"RooCMSShape::backgroundFail(mass, alphaFail[60.,50.,70.], betaFail[0.001, 0.,0.1], gammaFail[0.1, 0, 1], peakFail[90.0, 70, 80])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.5,0,1]",
"signalFractionInPassing[1.0]"
),

passingPresel_0p9To1p0_1p479To2p5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0,-5.000,5.000],sigmaP[0.5,0.001,5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0,-5.000,5.000],sigmaF[0.5,0.001,5.000])",
"ZGeneratorLineShape::signalPhyPass(mass,\"templatePreselEE_tag40.root\", \"hMass_0.90To1.0_1.479To2.5_Pass\")",
"ZGeneratorLineShape::signalPhyFail(mass,\"templatePreselEE_tag40.root\", \"hMass_0.90To1.0_1.479To2.5_Fail\")",
"RooCMSShape::backgroundPass(mass, alphaPass[60.,50.,70.], betaPass[0.001, 0.,0.1], gammaPass[0.1, 0, 1], peakPass[90.0, 70, 80])",
"RooCMSShape::backgroundFail(mass, alphaFail[60.,50.,70.], betaFail[0.001, 0.,0.1], gammaFail[0.1, 0, 1], peakFail[90.0, 70, 80])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.5,0,1]",
"signalFractionInPassing[1.0]"
),

)
