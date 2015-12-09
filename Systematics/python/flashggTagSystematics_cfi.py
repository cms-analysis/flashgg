import FWCore.ParameterSet.Config as cms

RVBins = cms.PSet(
                variables = cms.vstring("diPhoton().pt"),
                bins = cms.VPSet(
cms.PSet(lowBounds = cms.vdouble(0), upBounds = cms.vdouble(10), values = cms.vdouble(1.04969,0.925092), uncertainties = cms.vdouble(0.00369751,0.00369751,0.00557573,0.00557573)),
cms.PSet(lowBounds = cms.vdouble(10), upBounds = cms.vdouble(20), values = cms.vdouble(1.03627,0.915031), uncertainties = cms.vdouble(0.00284216,0.00284216,0.00665951,0.00665951)),
cms.PSet(lowBounds = cms.vdouble(20), upBounds = cms.vdouble(30), values = cms.vdouble(1.00917,0.962609), uncertainties = cms.vdouble(0.00237848,0.00237848,0.00969786,0.00969786)),
cms.PSet(lowBounds = cms.vdouble(30), upBounds = cms.vdouble(40), values = cms.vdouble(0.999164,1.00591), uncertainties = cms.vdouble(0.00217114,0.00217114,0.0153518,0.0153518)),
cms.PSet(lowBounds = cms.vdouble(40), upBounds = cms.vdouble(50), values = cms.vdouble(1.0002,0.997897), uncertainties = cms.vdouble(0.00202796,0.00202796,0.0208318,0.0208318)),
cms.PSet(lowBounds = cms.vdouble(50), upBounds = cms.vdouble(70), values = cms.vdouble(0.998695,1.02067), uncertainties = cms.vdouble(0.00137969,0.00137969,0.021848,0.021848)),
cms.PSet(lowBounds = cms.vdouble(70), upBounds = cms.vdouble(110), values = cms.vdouble(1.00354,0.899309), uncertainties = cms.vdouble(0.00105337,0.00105337,0.0299365,0.0299365)),
cms.PSet(lowBounds = cms.vdouble(110), upBounds = cms.vdouble(170), values = cms.vdouble(1.00369,0.786148), uncertainties = cms.vdouble(0.00105199,0.00105199,0.0609889,0.0609889)),
cms.PSet(lowBounds = cms.vdouble(170), upBounds = cms.vdouble(400), values = cms.vdouble(1,1), uncertainties = cms.vdouble(0.00383577,0.00383577,0.47521,0.47521)),
cms.PSet(lowBounds = cms.vdouble(400), upBounds = cms.vdouble(7000), values = cms.vdouble(1,1), uncertainties = cms.vdouble(0.,0.,0.,0.))
)
)


flashggTagSystematics = cms.EDProducer('FlashggTagSystematicProducer',
                                       src = cms.InputTag("flashggSystTagMerger"),
                                       SystMethods2D = cms.VPSet(),
                                       SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggTagWeightFromFracRV"),
                                                                          Label = cms.string("FracRVWeight"),
                                                                          NSigmas = cms.vint32(-1,1),
                                                                          OverallRange = cms.string("1"),
                                                                          BinList = RVBins,
                                                                          Debug = cms.untracked.bool(True)
                                                                         )
                                                               )
                                       )

