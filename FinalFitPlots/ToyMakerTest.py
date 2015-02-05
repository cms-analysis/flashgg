#inputmassfacws
#outputmassfactoy="massfac_toy_ws.root"
#inputpdfworkspace
#outputpdfworspace="combToyWS.root"
#treefilename
#randomizeBackground
#datatreename="dataTree"
#signatreename="sigTree"


import ROOT
from CombinedToyMaker import CombinedToyMaker

inputmassfac = "/vols/cms02/h2g/latest_workspace/CMS-HGG_massfacmva-13-04-2012.root"
inputfullmva = "/vols/cms02/h2g/latest_workspace/CMS-HGG_jan16reload_fullmva.root_interpolated.root"
inputtrees     = "/vols/cms02/h2g/latest_workspace/Tree_CMS-HGG_jan16reload_treesV2.root"
outputPdfWs    = "combToyWS.root"
outputmassfacws = "massfactoy.root"

expSig=0.

toymaker = CombinedToyMaker(inputmassfac)
#toymaker.createPdfs(inputtrees,outputPdfWs,expSig) 
toymaker.loadPdfs(outputPdfWs,expSig)
toymaker.plotData("./",160,200)

toymaker.genData(outputmassfacws,expSig)
toymaker.plotToy("./",160,95,expSig)

flist = toymaker.returnWindowToyData(125,0.02)

outTest = open("outTest.txt",'w')
print 'elements in list: ', len(flist)
for el in flist:
  outTest.write("%1.4f    %+0.5f \n"%(el[0],el[1]))
