relevant = { "WH" : ["QQ2HLNU","QQ2HQQ"],
             "ZH" : ["QQ2HLL","QQ2HQQ"],
             "ttH" : ["TTH"],
             "VBF" : ["QQ2HQQ"],
             "GluGluH" : ["GG2H"]
             } #FIXME: need to add bbH, tH, and ggZH

cmds = ["mkdir -p irr"]

from os import listdir

for fn in listdir("."):
    rel = False
    for k,v in relevant.iteritems():
        for s in v:
            if fn.count("output_%s" % k) and (fn.count("%s.root"%s) or fn.count("%s_"%s)):
                rel = True
    if rel:
        print "KEEPING %s" % fn
    else:
        cmds.append("mv %s irr"%fn)

from os import popen
for cmd in cmds:
    print cmd
    print popen(cmd).read().strip()
