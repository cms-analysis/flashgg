# Cut Down a datacard (ie select a subset of the channels)
import sys
from optparse import OptionParser

nProcesses = 5 #ggh,wzh,vbf,tth,bkg

bins = []
g_deletebins=[]

def cback(option,opt_str,value,parser):
	value = value.split(",")
	for v in value: bins.append(int(v))

def dback(option,opt_str,value,parser):
	g_deletebins.append(1)
	value = value.split(",")
	for v in value: bins.append(int(v))

parser = OptionParser()
parser.add_option("-i","--input",dest="tfileName")
parser.add_option("-b","--bins",dest="bins",action="callback",callback=cback,type='string')
parser.add_option("-d","--delbins",dest="delbins",action="callback",callback=dback,type='string')
parser.add_option("-o","--output",dest="output",default="cutdown_")


(options,args)=parser.parse_args()

f = open(options.tfileName,"r")
lines = f.readlines()
printLines = []
modifyLines = []
nCats = -1

obsline = []
chanline= []

for l in lines :
	if "bin" in l and nCats == -1:	
		nCats = len((l.split())[1:])	
		chanline=l.split();
	elif "bin" in l and nCats>-1:
		modifyLines.append(l)
	elif "observation" in l: obsline=l.split()
	elif "imax" in l or "jmax" in l or "kmax" in l: continue
	else : modifyLines.append(l)
f.close()

keepBins = []
if g_deletebins : 
	listOfBins=range(0,nCats+1)
	listOfBins[0]=-99
	for k in bins:	listOfBins[k]=-99
	keepBins = filter(lambda x:x != -99,listOfBins)
else: keepBins=bins

if len(keepBins)==0 : sys.exit("Cannot create datacard with no bins!")
print "New card will have %d bins cut down from %d "%(len(keepBins),nCats)
print "Keeping bins ", keepBins

for ml in modifyLines:
	if "lnN" not in ml and "rate" not in ml and "process" not in ml and "bin" not in ml: 
		continue
		
	newline = ""
	parts = ml.split()
	remain=[]
	if "lnN" in ml:
		newline+=" ".join(parts[0:2])+" "
		remain = parts[2:]
	elif "rate" in ml or "process" in ml or "bin" in ml:
		newline+=" ".join(parts[0:1])+" "
		remain = parts[1:]
	vals = [ remain[nProcesses*i:nProcesses*i+nProcesses] for i in range(nCats)]
	vals.insert(0,[-99]) # keeps the sizes correct
	for i in keepBins: newline += " ".join(vals[i]) + " "

	printLines.append(newline)
			

o = open(options.output+options.tfileName,"w")	
o.write("Cutdown card from %s\n"%options.tfileName)
o.write("imax=*\njmax=*\nkmax=*\n")
o.write(chanline[0]+" ")
for k in keepBins: o.write(chanline[k]+" ")
o.write("\n")
o.write(obsline[0]+" ")
for k in keepBins: o.write(obsline[k]+" ")
o.write("\n")
for pl in printLines: o.write(pl+'\n')
print "New card written to %s"%options.output+options.tfileName
o.close()

