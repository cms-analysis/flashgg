print "Remember to get files with get_xs.sh first"

gghpos = 999
brdict = {}
xsdict = {}
keys = ["ttH","ggH","ZH","WH","vbfH"]
combkeys = ["ttH","ggH","VH","vbfH"]

templates = {}
templates["vbfH"] = ["VBFHToGG_M%i_13TeV_amcatnlo_pythia8","VBFHToGG_M-%i_13TeV_powheg_pythia8"]
templates["ggH"] = ["GluGluHToGG_M%i_13TeV_amcatnloFXFX_pythia8","GluGluHToGG_M%i_13TeV_amcatnlo_pythia8"]
templates["VH"] = ["VHToGG_M%i_13TeV_amcatnloFXFX_madspin_pythia8"]
templates["ttH"] = ["ttHJetToGG_M%i_13TeV_amcatnloFXFX_madspin_pythia8","ttHJetToGG_M%i_13TeV_amcatnloFXFX_madspin_pythia8_v2"]

itypes = {}
itypes["ggH"] = 0
itypes["VH"] = 4
itypes["vbfH"] = 1
itypes["ttH"] = 3

masslist = []

def computeItype(combkey,mass):
    return -1*(1000*mass+100*itypes[combkey])

def createLines(combkey,mass):
    result = ""
    for template in templates[combkey]:
        stringargs = (template%mass,xsdict[(combkey,mass)],brdict[mass],computeItype(combkey,mass))
        result += '        "%s"                          : { "xs" : %.4f,   "br" : %.3g, "itype":%i },\n'%stringargs
    return result[:-1]    

for line in open("BR4.txt",'r').readlines():
    try:
        mass = float(line.split()[0])
    except Exception:
        print line.strip()
        for i in range(len(line.split())):
            if line.split()[i] == "hgg":
                gghpos = i
        continue
    if abs(mass-int(mass)) > 0.01:
        continue
    mass = int(mass)
    br = float(line.split()[gghpos])
    print line.strip()
    print mass,br
    brdict[mass] = br
    masslist.append(mass)

for key in keys:
    for line in open("13TeV-%s.txt" % key,'r').readlines():
        try:
            mass = float(line.split()[0])
        except Exception:
            continue
        if abs(mass-int(mass)) > 0.01:
            continue
        mass = int(mass)
        xs = float(line.split()[1])
        print line.strip()
#        shortkey = key[:3].lower()
#        print shortkey,mass,xs
#        xsdict["%s_%i"%(shortkey,mass)] = br
        xsdict[(key,mass)] = xs

for key in ["WH","ZH"]:
    for mass in masslist:
        xsdict[("VH",mass)] = xsdict[("WH",mass)]+xsdict[("ZH",mass)]

print 

for key in combkeys:
    for mass in masslist:
        print createLines(key,mass)
        

#print 
#print brdict
#print xsdict
