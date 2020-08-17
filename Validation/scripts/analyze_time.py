

fns = ["new3timing_puppi%i.txt" % i for i in range(3)]
fs = [open(fns[i],'r').read() for i in range(3)]
modtimes = []
eventtimes = []
classmap = {}

for i in range(3):
    modtimes.append({})
    eventtimes.append([])
    print i,fns[i]
    for line in fs[i].split("\n"):
        if line.count("TimeEvent>"):
            eventtimes[i].append(float(line.strip().split()[-1]))
        if line.count("TimeModule>"):
            time = float(line.strip().split()[-1])
            name = line.strip().split()[-3]
            theclass = line.strip().split()[-2]
            if not modtimes[i].has_key(name):
                modtimes[i][name] = []
            modtimes[i][name].append(time)
            classmap[name] = theclass
    
        
from numpy import mean, sqrt, square, arange
a = arange(10) # For example
rms = sqrt(mean(square(a)))

totalbyproducer = []

for i in range(3):
    totalbyproducer.append({})
    dataset = eventtimes[i][1:1000]
    print "Event time for puppi=%i: %.5f s +/- %.5f s" % (i,mean(dataset),sqrt(mean(square(dataset))/1000))
    print "   Max event time: %.5f s RMS: %.5f s"%(max(dataset),sqrt(mean(square(dataset))))
    for name in modtimes[i].keys():
        dataset = modtimes[i][name][1:1000]
        if 1000*mean(dataset) > 1:
            print "  Module time for %s (%s) for puppi=%i: %.5f ms +/- %.5f ms" % (name,classmap[name],i,1000*mean(dataset),1000*sqrt(mean(square(dataset))/1000))
            print "     Max module time: %.5f ms RMS: %.5f ms"%(1000*max(dataset),1000*sqrt(mean(square(dataset))))
        if not totalbyproducer[i].has_key(classmap[name]):    
            totalbyproducer[i][classmap[name]] = 0.
        totalbyproducer[i][classmap[name]] += 1000*mean(dataset)
            
namelist = []
for i in range(3):
    for name in modtimes[i].keys():
        if not namelist.count(name):
            namelist.append(name)

for name in namelist:
    datasets = []
    for i in range(3):
        if modtimes[i].has_key(name):
            datasets.append(modtimes[i][name][1:1000])
        else:
            datasets.append([0.])
#    datasets = [modtimes[i][name][1:1000] for i in range(3) if modtimes[i].has_key(name) else [0.]]
    if 1000*mean(datasets[0]) > 1. or 1000*mean(datasets[1]) >1. or 1000*mean(datasets[2]) >1.:
        print "For name %s:"% name
        for i in range(3):
            if len(datasets[i]) > 5:
                print "  Present for puppi=%i with %.5f ms +/- %.5f ms" % (i,1000*mean(datasets[i]),1000*sqrt(mean(square(datasets[i]))/1000))

print
for i in range(3):
    for key,val in sorted(totalbyproducer[i].items(), key=lambda x: x[1],reverse=True):
        print "For puppi=%i, %s takes %.5f ms" % (i,key,val)
    
        
