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
                    if len(shortName) >= 100:
                        shortName = shortName.replace("_TuneCUETP8M1_13TeV_Pythia8","").replace("_TuneCUETP8M1","")
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
