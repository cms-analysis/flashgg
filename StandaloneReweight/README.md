## Standalone programs to perform (kl,kt) reweight     
# Compilation     
```
g++ -Wall -o "Hreweight.exe" `root-config --cflags --glibs` -L $ROOTSYS/lib -lRooFitCore -lFoam -lMinuit -lMathMore DoubleHReweighter.h SingleHReweighter.h CfgManager.cc Hreweight.cpp       
g++ -Wall -o "MINIAODdumper.exe" `root-config --cflags --glibs` -L $ROOTSYS/lib -lRooFitCore -lFoam -lMinuit -lMathMore DoubleHReweighter.h CfgManager.cc MINIAODdumper.cpp          
g++ -Wall -o "Compute_allHHNodeMap2D.exe" `root-config --cflags --glibs` -L $ROOTSYS/lib -lRooFitCore -lFoam -lMinuit -lMathMore  CfgManager.cc Compute_allHHNodeMap2D.cpp        
```      

# Running      
To run the reweight of the flashgg tree
```
./Hreweight.exe hreweight.cfg     
```
Input and outputs are configurable through the config file. The provided configuration is for 2017.      
       
To dump the miniaod and compute the normalizations      
```
python SplitMINIAODdumperOnLxplus.py -l alabel -e /afs/cern.ch/user/f/fmonti/work/flashggNew/CMSSW_10_5_0/src/flashgg/StandaloneReweight/MINIAODdumper.exe  -p /afs/cern.ch/user/f/fmonti/x509up_u102658 -o /afs/cern.ch/user/f/fmonti/work/flashggNew/CMSSW_10_5_0/src/flashgg/outfolder/ -c /afs/cern.ch/user/f/fmonti/work/flashggNew/CMSSW_10_5_0/src/flashgg/StandaloneReweight/miniaoddumperTEMPLATE.cfg -i /afs/cern.ch/user/f/fmonti/work/flashggNew/CMSSW_10_5_0/src/flashgg/StandaloneReweight/nanoAOD2017.txt     
```      



