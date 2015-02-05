import sys

input = open( sys.argv[-1], 'r')

import os.path
base,ext = os.path.splitext(input.name)

originalcontents = input.read()

corrs = ('doEscale',
         'doEresol',
         'doPhotonIdEff',
         'doVtxEff',
         'doTriggerEff',
         'doR9',
         'doKFactor'
         )

header = """
->
Turn on-off all smearings 
->
doMCSmearing=1 

->
Individual smearing flags
->

"""

corrs_str = [header]
for corr in corrs:
    corrs_str.append('%sSmear=%%(%s)d\n'%(corr,corr))
    corrs_str.append('%sSyst=0\n\n'%corr)
    
template = ''.join([line for line in corrs_str])

corrflags_empty = {}
for corr in corrs:
    corrflags_empty[corr] = 0

from copy import deepcopy
import re
reps = [
        # '''intL=204. histfile=/castor/cern.ch/user/c/cmshgg/StatAnalysis/11-06-24/CMS-HGG.root output=shifo.root'''        
        (re.compile(ur'''histfile=([^\s]+).root''',re.U), ur'''histfile=\1_%(optsstr)s.root '''),

        # '''analyzer StatAnalysis photonanalysis.dat statanalysis.dat'''
        (re.compile(ur'''statanalysis.dat''',re.U), ur'''statanalysis.dat %(optsfname)s'''),        
]

def process(optsstr, corrflags):
    optsfname = '%s.dat'%optsstr
    f = open(optsfname, 'w')
    f.write(template%corrflags)
    f.close()
#    print optsfname
#    print template%corrflags
#    print '*'*50
        
    maincontents = deepcopy(originalcontents)
    for rep in reps:
        s = re.sub(rep[0],rep[1],maincontents)
        maincontents = s

    m = open('%s_%s'%(base,optsfname), 'w')
    m.write(maincontents%locals())
    m.close()
#    print '%s_%s'%(base,optsfname)
#    print maincontents%locals()    
#    print '*'*50


process('NoSmear', corrflags_empty)

for corr in corrs:
#    print corr
#    print '*'*50
    optsstr='%sSmearOnly'%corr
    corrflags = deepcopy(corrflags_empty)
    corrflags[corr]=1
    process(optsstr, corrflags)    
