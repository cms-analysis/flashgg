# Add the area containing the MVA weights (from cms-data, to appear in “external”).
# Note: the “external” area appears after “scram build” is run at least once, as above
#
cd $CMSSW_BASE/external
# below, you may have a different architecture, this is just one example from lxplus
cd $SCRAM_ARCH/
git clone https://github.com/lsoffi/RecoEgamma-PhotonIdentification.git data/RecoEgamma/PhotonIdentification/data
cd data/RecoEgamma/PhotonIdentification/data
git checkout CMSSW_9_4_0_pre3_TnP
cd $CMSSW_BASE/external
cd $SCRAM_ARCH/
git clone https://github.com/lsoffi/RecoEgamma-ElectronIdentification.git data/RecoEgamma/ElectronIdentification/data
cd data/RecoEgamma/ElectronIdentification/data
git checkout CMSSW_9_4_0_pre3_TnP
# scales and smearings
cd $CMSSW_BASE/external/$SCRAM_ARCH/
git clone https://github.com/ECALELFS/ScalesSmearings.git data/RecoEgamma/ScalesSmearings/data
# Go back to the src/
cd $CMSSW_BASE/src
