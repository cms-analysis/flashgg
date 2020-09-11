from os import listdir,popen,access,F_OK,getcwd
from sys import argv

from optparse import OptionParser
parser = OptionParser()
parser.add_option('--doBigData', default=False, action='store_true', help='Make one big data file')
parser.add_option('--doEGamma', default=False, action='store_true', help='use EGamma rather than DoubleEG (2018 data)')
parser.add_option('--targetString', default=None, help='String to match to include')
parser.add_option('--skipString', default=None, help='Strong to match to skip')
(opts,args) = parser.parse_args()

targetstring = ""
if opts.targetString is not None: 
    targetstring = opts.targetString

skipstring = ""
if opts.skipString is not None: 
    skipstring = opts.skipString

dobig = False
dobigsig = False
dobigdata = opts.doBigData
doegamma = opts.doEGamma

filelist = {}
bigfiles = []
bigsigfiles = []

def printAndExec(cmd):
    print cmd
    result = popen(cmd).read()
    print result

print listdir(".")
print getcwd()
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
<<<<<<< HEAD
    # assert(result[-1]+1 == len(result))
=======
    assert(result[-1]+1 == len(result)) #FIXME
>>>>>>> 4304e0dc8b0a07feeab99c492fa51624281a7ee4
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
    if doegamma: cmd = "hadd_workspaces allData.root *EGamma*USER.root"
    else: cmd = "hadd_workspaces allData.root *DoubleEG*USER.root"
    printAndExec(cmd)
else:
    print "skipping allData.root"
