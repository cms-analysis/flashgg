from os import listdir,popen,access,F_OK

filelist = {}
bigfiles = []

for fn in listdir("."):
    if fn.count(".root"):
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
    if access(bigfile,F_OK):
        print "skipping",bigfile
        continue
    if len(result) > 80:
        subres = []
        nextone = 0
        while nextone < len(result):
            subres.append(result[nextone:nextone+50])
            nextone += 50
        mediumlist = []    
        for i in range(len(subres)):
            mediumfile = fnr.replace("_%i","intermediate%i"%i)
            if access(mediumfile,F_OK):
                print "skipping",mediumfile
                continue
            cmd = "hadd_workspaces %s %s" % (mediumfile," ".join(fnr%fnn for fnn in subres[i]))
            print cmd
            result = popen(cmd).read()
            print result
            mediumlist.append(mediumfile)
        cmd = "hadd_workspaces %s %s" % (bigfile," ".join(mediumlist))    
        print cmd
        result = popen(cmd).read()
        print result
    else:    
        cmd = "hadd_workspaces %s %s" % (bigfile," ".join([fnr%fnn for fnn in result]))
        print cmd
        result = popen(cmd).read()
        print result
    bigfiles += [bigfile]

print
cmd = "hadd_workspaces everything.root %s" % (" ".join(bigfiles))
print cmd
result = popen(cmd).read()
print result

