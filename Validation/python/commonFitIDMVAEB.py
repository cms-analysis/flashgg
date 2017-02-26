import FWCore.ParameterSet.Config as cms

all_pdfs = cms.PSet(
passingIDMVA_0p0To0p85_0p0To1p479 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0,-5.000,5.000],sigmaP[0.5,0.001,5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0,-5.000,5.000],sigmaF[0.5,0.001,5.000])",
"ZGeneratorLineShape::signalPhyPass(mass,\"templateIDMVAEB.root\", \"hMass_0.0To0.85_0.0To1.479_Pass\")",
"ZGeneratorLineShape::signalPhyFail(mass,\"templateIDMVAEB.root\", \"hMass_0.0To0.85_0.0To1.479_Fail\")",
"RooCMSShape::backgroundPass(mass, alphaPass[63.,50.,70.], betaPass[0.001, 0.,0.1], gammaPass[0.1, 0, 1], peakPass[90.0, 70, 80])",
"RooCMSShape::backgroundFail(mass, alphaFail[63.,50.,70.], betaFail[0.001, 0.,0.1], gammaFail[0.1, 0, 1], peakFail[90.0, 70, 100])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.5,0,1]",
"signalFractionInPassing[1.0]"
),

passingIDMVA_0p85To1p0_0p0To1p479 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0,-5.000,5.000],sigmaP[0.5,0.001,5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0,-5.000,5.000],sigmaF[1.0,0.001,5.000])",
"ZGeneratorLineShape::signalPhyPass(mass,\"templateIDMVAEB.root\", \"hMass_0.85To1.0_0.0To1.479_Pass\")",
"ZGeneratorLineShape::signalPhyFail(mass,\"templateIDMVAEB.root\", \"hMass_0.85To1.0_0.0To1.479_Fail\")",
"RooCMSShape::backgroundPass(mass, alphaPass[60.,50.,70.], betaPass[0.001, 0.,0.1], gammaPass[0.1, 0, 1], peakPass[90.0, 70, 80])",
"RooCMSShape::backgroundFail(mass, alphaFail[69.,50.,70.], betaFail[0.001, 0.0001,0.1], gammaFail[0.1, 0, 1], peakFail[90.0, 70, 100])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.5,0,1]",
"signalFractionInPassing[1.0]"
),

)
