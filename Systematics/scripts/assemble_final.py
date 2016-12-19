from os import listdir,popen

stage0catname2num = {"UNKNOWN":0, "GG2H_FWDH":10, "GG2H":11, "VBF_FWDH":20, "VBF":21, "VH2HQQ_FWDH":22, "VH2HQQ":23,
                     "QQ2HLNU_FWDH":30, "QQ2HLNU":31, "QQ2HLL_FWDH":40, "QQ2HLL":41, "GG2HLL_FWDH":50, "GG2HLL":51,
                     "TTH_FWDH":60, "TTH":61, "BBH_FWDH":70, "BBH":71, "TH_FWDH":80, "TH":81 }
stage0catnum2name = {v: k for k, v in stage0catname2num.iteritems()}
relevantstage0cats = [cat for cat in stage0catname2num.values() if cat > 0 and (cat <= 49 or cat == 60 or cat == 61)]
namestouse = [stage0catnum2name[x] for x in relevantstage0cats]
masses = [120,123,124,125,126,127,130]

print namestouse

print popen("mkdir -p final_ws").read()

logfilename = "final_ws/addlog.txt"
log = open(logfilename,'a')

for name in namestouse:
    for mass in masses:
        filestoadd = []
        for fn in listdir("."):
            if fn.count("%s.root" % name) and fn.count(str(mass)):
                filestoadd.append(fn)
        if filestoadd:
            if len(filestoadd) != 4:
                print "WARNING: Expected 4 file for %s %i, got %i" % (name,mass,len(filestoadd))
            cmd = "hadd_workspaces final_ws/%s-%i.root %s" % (name,mass," ".join(filestoadd))
            log.write(cmd)
            result = popen(cmd).read()
            log.write(result)
        else:
            print "ERROR: NO FILES FOR %s %i" % (name,mass)
        
log.close()
