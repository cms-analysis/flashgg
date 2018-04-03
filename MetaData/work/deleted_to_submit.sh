# LIST OF COMMANDS FOR DELETING UNNEEDED FILES TO GET THE WORK AREA LIGHT ENOUGH FOR GRID SUBMISSION (100 MB)

echo
echo "LIST OF COMMANDS FOR DELETING UNNEEDED FILES TO GET THE WORK AREA LIGHT ENOUGH FOR GRID SUBMISSION (100 MB)"
echo "EVERYTHING IS COMMENTED OUT, PLEASE SEE SCRIPT AND EDIT AS NEEDED - BUT USE WITH CARE!"
echo

 rm $CMSSW_BASE/src/flashgg/Taggers/data/*
 rm $CMSSW_BASE/external/$SCRAM_ARCH/data/RecoEgamma/ElectronIdentification/data/*.xml
 rm $CMSSW_BASE/external/$SCRAM_ARCH/data/RecoEgamma/ElectronIdentification/data/PHYS14/*.xml
 rm $CMSSW_BASE/external/$SCRAM_ARCH/data/RecoEgamma/ElectronIdentification/data/Spring15/*.xml
 rm $CMSSW_BASE/external/$SCRAM_ARCH/data/RecoEgamma/PhotonIdentification/data/PHYS14/*.xml
 rm $CMSSW_BASE/external/$SCRAM_ARCH/data/RecoEgamma/PhotonIdentification/data/Spring15/*.xml
 rm $CMSSW_BASE/src/flashgg/MetaData/data/*/*.json
 rm $CMSSW_BASE/lib/$SCRAM_ARCH/pluginPhysicsToolsTagAndProbe_plugins.so
 rm -rf $CMSSW_BASE/external/$SCRAM_ARCH/data/RecoEgamma/PhotonIdentification/data/.git/
 rm -rf $CMSSW_BASE/external/$SCRAM_ARCH/data/RecoEgamma/ElectronIdentification/data/.git/

