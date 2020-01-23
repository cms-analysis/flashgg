from os import listdir,popen,access,F_OK
from sys import argv

targetstring = ""
if len(argv) > 1:
    targetstring = argv[1]

skipstring = ""
if len(argv) > 2:
    skipstring = argv[2]

dobig = False
dobigsig = False
dobigdata = False # LOL

filelist = {}
bigfiles = []
bigsigfiles = []

def printAndExec(cmd):
    print cmd
    result = popen(cmd).read()
    print result

for fn in listdir("."):
    if fn.count(".root") and fn.count(targetstring) and (not skipstring or not fn.count(skipstring)):
        fnr = "_".join(fn[:-5].split("_")[:-1])+"_%i.root"
        try:
            fnn = int(fn[:-5].split("_")[-1])
        except Exception:
            continue
        print fnr,fnn
        assert ((fnr % fnn) == fn)
        if filelist.has_key(fnr):
            filelist[fnr] += [fnn]
        else:
            filelist[fnr] = [fnn]
 
for fnr in filelist.keys():
    result = sorted(filelist[fnr])
    print fnr,result
    assert(result[-1]+1 == len(result))
    bigfile = fnr.replace("_%i","")
    bigfiles.append(bigfile)
    if bigfile.count("HToGG") or bigfile.count("ttHJetToGG"):
        bigsigfiles.append(bigfile)
    if access(bigfile,F_OK):
        print "skipping",bigfile
        continue
    if len(result) > 8:
        filesperintermediate = int((len(result))**0.5)
        subres = []
        nextone = 0
        while nextone < len(result):
            subres.append(result[nextone:nextone+filesperintermediate])
            nextone += filesperintermediate
        mediumlist = []    
        for i in range(len(subres)):
            mediumfile = fnr.replace("_%i","intermediate%i"%i)
            mediumlist.append(mediumfile)
            if access(mediumfile,F_OK):
                print "skipping",mediumfile
                continue
            cmd = "hadd_workspaces %s %s" % (mediumfile," ".join(fnr%fnn for fnn in subres[i]))
            printAndExec(cmd)
        cmd = "hadd_workspaces %s %s" % (bigfile," ".join(mediumlist))    
        printAndExec(cmd)
    else:    
        cmd = "hadd_workspaces %s %s" % (bigfile," ".join([fnr%fnn for fnn in result]))
        printAndExec(cmd)

print
if not access("everything.root",F_OK) and dobig:
    cmd = "hadd_workspaces everything.root %s" % (" ".join(bigfiles))
    printAndExec(cmd)
else:
    print "skipping everything.root"

if not access("allsig.root",F_OK) and dobigsig:
    cmd = "hadd_workspaces allsig.root %s" % (" ".join(bigfiles))
    printAndExec(cmd)
else:
    print "skipping allsig.root"

if not access("allData.root",F_OK) and dobigdata:
    cmd = "hadd_workspaces allData.root *DoubleEG*USER.root"
    printAndExec(cmd)
else:
    print "skipping allData.root"
