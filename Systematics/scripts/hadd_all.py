from os import listdir,popen,access,F_OK,getcwd,system
from sys import argv

from optparse import OptionParser
parser = OptionParser()
parser.add_option('--doBigData', default=False, action='store_true', help='Make one big data file')
parser.add_option('--doEGamma', default=False, action='store_true', help='use EGamma rather than DoubleEG (2018 data)')
parser.add_option('--targetString', default=None, help='String to match to include')
parser.add_option('--skipString', default=None, help='String to match to skip')
parser.add_option('--haddTrees', default=False, action='store_true', help='Hadd tree if true')
parser.add_option('--haddWorkspace', default=True, action='store_true', help='Hadd workspace if true')
(opts,args) = parser.parse_args()

targetstring = ""
if opts.targetString is not None: 
    targetstring = opts.targetString
haddTrees = opts.haddTrees
haddWorkspace = opts.haddWorkspace

print "haddTrees = ",haddTrees
print "haddWorkspace = ",haddWorkspace
print "targetstring = ",targetstring
system('pwd')

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
    print "=====> ",cmd
    result = popen(cmd).read()
    print result

print "="*51
print listdir(".")
print getcwd()
print "="*51
for fn in listdir("."):
    if fn.count(".root") and fn.count(targetstring) and (not skipstring or not fn.count(skipstring)):
        fnr = "_".join(fn[:-5].split("_")[:-1])+"_%i.root"
        try:
            fnn = int(fn[:-5].split("_")[-1])
        except Exception:
            continue
        print "==> ",fnr,fnn
        assert ((fnr % fnn) == fn)
        if filelist.has_key(fnr):
            filelist[fnr] += [fnn]
        else:
            filelist[fnr] = [fnn]

for fnr in filelist.keys():
    print "[INFO][hadd_all.py#61] fnr = ",fnr
    result = sorted(filelist[fnr])
    print fnr,result
    # assert(result[-1]+1 == len(result))
    ##-- If num files shouldn't follow ordered naming convention like USER_1, USER_2, ... then remove assertion 
    assert(result[-1]+1 == len(result)) #FIXME
    bigfile = fnr.replace("_%i","")
    print "bigfile: ",bigfile
    bigfiles.append(bigfile)
    if bigfile.count("HToGG") or bigfile.count("ttHJetToGG"):
        bigsigfiles.append(bigfile)
    if access(bigfile,F_OK):
        print "[INFO][hadd_all.py#72] skipping",bigfile
        continue
    print "[INFO][hadd_all.py#74] "
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
                print "[INFO][hadd_all.py#86] skipping",mediumfile
                continue
            if haddWorkspace:
                cmd = "hadd_workspaces %s %s" % (mediumfile," ".join(fnr%fnn for fnn in subres[i]))
            if haddTrees:
                cmd = "hadd %s %s" % (mediumfile," ".join(fnr%fnn for fnn in subres[i]))
            printAndExec(cmd)
        if haddWorkspace:
            cmd = "hadd_workspaces %s %s" % (bigfile," ".join(mediumlist))
        if haddTrees:
            cmd = "hadd %s %s" % (bigfile," ".join(mediumlist))
        printAndExec(cmd)
    else:
        if haddWorkspace:
            cmd = "hadd_workspaces %s %s" % (bigfile," ".join([fnr%fnn for fnn in result]))
        if haddTrees:
            cmd = "hadd %s %s" % (bigfile," ".join([fnr%fnn for fnn in result]))
        printAndExec(cmd)
print "[INFO][hadd_all.py#104] ... "
if not access("everything.root",F_OK) and dobig:
    if haddWorkspace:
        cmd = "hadd_workspaces everything.root %s" % (" ".join(bigfiles))
    if haddTrees:
        cmd = "hadd everything.root %s" % (" ".join(bigfiles))
    printAndExec(cmd)
else:
    print "skipping everything.root"

if not access("allsig.root",F_OK) and dobigsig:
    if haddWorkspace:
        cmd = "hadd_workspaces allsig.root %s" % (" ".join(bigfiles))
    if haddTrees:
        cmd = "hadd allsig.root %s" % (" ".join(bigfiles))
    printAndExec(cmd)
else:
    print "skipping allsig.root"

if not access("allData.root",F_OK) and dobigdata:
    if haddWorkspace:
        cmd = "hadd_workspaces allData.root *DoubleEG*USER.root"
    if haddTrees:
        cmd = "hadd allData.root *DoubleEG*USER.root"
    printAndExec(cmd)
else:
    print "skipping allData.root"
