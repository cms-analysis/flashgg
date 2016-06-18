from os import listdir,system

for fn in listdir("."):
    if fn.count(".py") and fn.count("crabConfig"):
        shortName = ""
        theName = ""
        for line in open(fn,'r').readlines():
            line = line.strip()
            if line.count(".requestName"):
                theName = line.split('"')[1]
                print theName
                if len(theName) >= 100:
                    print "  ... has length %i characters!" % len(theName)
                    shortName = theName.replace("_RunIISpring15MiniAODv2","")
                    shortName = shortName.replace("_RunIIFall15DR76","")
                    shortName = shortName.replace("_RunIIFall15MiniAODv2","")
                    shortName = shortName.replace("_pythia8","_P8")
                    shortName = shortName.replace("_asymptotic_","_asympt_")
                    shortName = shortName.replace("_mcRun2_","_mcR2_")
                    shortName = shortName.replace("Down","Do")
                    # 2016
                    shortName = shortName.replace("RunIISpring16MiniAODv1-PUSpring16RAWAODSIM", "")
                    shortName = shortName.replace("RunIISpring16MiniAODv1-PUSpring16", "")
                    if len(shortName) >= 100:
                        shortName = shortName.replace("_TuneCUETP8M1_13TeV_Pythia8","").replace("_TuneCUETP8M1","")
                        shortName = shortName.replace("_amcatnloFXFX_madspin_","").replace("_amcFX_MS_","")
                    print "  ... so we switch to %s (length %i)" % (shortName,len(shortName))
                    if len(shortName)>= 100:
                        raise Exception
        if len(shortName) > 0:            
            filetext = open(fn,'r').read()
            print "The name appears %i time(s)" % filetext.count(theName)
            newfn = fn.replace("crabConfig","crabConfig_fixed")
            newf = open(newfn,'w')
            newtext = filetext.replace(theName,shortName)
            newf.write(newtext)
            newf.close()
            origfn = fn.replace("crabConfig","crabConfig_orig")
            cmd = "mv %s %s ; mv %s %s" % (fn,origfn,newfn,fn)
            print cmd
            result = system(cmd)
            print result
