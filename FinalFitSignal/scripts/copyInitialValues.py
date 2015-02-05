#!/usr/bin/env python

import sys, os


#----------------------------------------------------------------------
# main
#----------------------------------------------------------------------
from optparse import OptionParser
parser = OptionParser("""

  usage: %prog [options] proc cat_number from_mass to_mass

  copies the initial values from one mass point to another

"""
)

(options, ARGV) = parser.parse_args()
#--------------------
if len(ARGV) != 4:
    parser.print_help()
    sys.exit(1)

proc, catNumber, fromMass, toMass = ARGV
# catNumber = int(catNumber)
# fromMass = int(fromMass)
# toMass = int(toMass)

# e.g. initFit_vbf_cat1.dat

fname = "dat/in/initFit_%s_cat%s.dat" % (proc, catNumber) 

if not os.path.exists(fname):
    print >> sys.stderr,"file %s does not exist" % fname
    sys.exit(1)

# read all lines first and only later replace the necessary lines
# (we might e.g. copy from a higher to a lower mass)
lines = open(fname).readlines()

# remove whitespaces at beginning and end
lines = [ line.strip() for line in lines ]

# first index is mass
# second index is parameter name
# third index is index of Gaussian
# value is the initial / fitted value
paramValues = {}

import re

for index,line in enumerate(lines):
    paramPart, value = re.split('\s+',line,1)

    paramName, mass, gaussianIndex = re.split('_',paramPart,2)

    # keep the line number
    # paramValues.setdefault(mass, {}).setdefault(paramName, {})[gaussianIndex] = dict(value = value, index = index)

    paramValues.setdefault(mass, {}).setdefault(paramName, {})[gaussianIndex] = value

# now copy the desired values
fromValues = paramValues['mh%s' % fromMass]

toValues = paramValues['mh%s' % toMass ]
toValues.clear() # just to be sure...

for paramName, val in fromValues.items():
    for gaussianIndex, value in val.items():
        toValues.setdefault(paramName,{})[gaussianIndex] = value


outLines = []

for mass in paramValues.keys():
    for paramName in paramValues[mass].keys():
        for gaussianIndex in paramValues[mass][paramName].keys():

            outLines.append("%s_%s_%s %s" % (paramName, mass, gaussianIndex,
                                             paramValues[mass][paramName][gaussianIndex])
                            )


outLines.sort()

# print it back to the original file        
# but make a backup first
import shutil
shutil.copy(fname, fname + ".bak")

# fout = sys.stdout
fout = open(fname,"w")
for line in outLines:
    print >> fout,line

fout.close()    
