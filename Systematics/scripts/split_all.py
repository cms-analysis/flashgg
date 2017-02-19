from os import listdir,popen

for fn in listdir("."):
    if fn.count("pythia8.root"):
        cmd = "python ../../scripts/sort_by_HTXS.py %s" % fn
        print cmd
        result = popen(cmd).read()
        print result
