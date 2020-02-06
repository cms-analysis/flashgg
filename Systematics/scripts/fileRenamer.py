associated = ['WH','ZH','VBF']
thMap = {'bbH':'testBBH','THQ':'testTHQ','THW':'testTHW'}

from os import listdir
from os import system
for filename in listdir('./'):
  for thName in thMap:
    if thName in filename:
      print "renaming %s.root as %s.root"%(thName,thMap[thName])
      system("mv %s output_%s_M125_13TeV_amcatnlo_pythia8_%s.root"%(filename,thName,thMap[thName]))
  for assoc in associated:
    if ('%sToGG'%assoc in filename or '%sHToGG'%assoc in filename) and 'QQ2HQQ' in filename:
      newfilename = filename.replace('QQ2HQQ','%s2HQQ'%assoc)
      newfilename = newfilename.replace('VBF2HQQ','VBF')
      print "renaming",filename,"as",newfilename
      system("mv %s %s"%(filename,newfilename))
