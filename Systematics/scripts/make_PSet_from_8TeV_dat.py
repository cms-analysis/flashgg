# EBLowEtaBad 0 0 1 -999.00 0.94 190645 190781 0.0078 0.0004

#scaleBins = cms.VPSet(cms.PSet( Range = cms.string("pt>10.&&pt<15."), Shift = cms.double(0.003), Uncertainty = cms.double(0.001) ),
#                      cms.PSet( Range = cms.string("pt>15.&&pt<20."), Shift = cms.double(-0.001), Uncertainty = cms.double(0.0005) ),
#                      cms.PSet( Range = cms.string("pt>20."), Shift = cms.double(0.002), Uncertainty = cms.double(0.0004) )
#                      )

# category      type(0=any,1=CM,2=GAP) minEta  MaxEta   mr9    Mr9      mrun    Mrun   scaleC  scaleErr
# EBlowEtaGold8TeV 0    0.0              1.0     0.94     999.00 -999999  999999  0.0    0.05e-2

outString = "scaleBins = cms.VPSet(\n"

for line in open("energy_scale_errors.dat").readlines():
    splitline = line.strip().split()
    if len(splitline) == 10:
        label = splitline[0]
        type = int(splitline[1])
        minEta = float(splitline[2])
        maxEta = float(splitline[3])
        minR9 = float(splitline[4])
        maxR9 = float(splitline[5])
        minRun = int(splitline[6])
        maxRun = int(splitline[7])
        shift = float(splitline[8])
        unc = float(splitline[9])
        print label,minEta,maxEta,minR9,maxR9,shift,unc
        outString += 10*" "
        outString += 'cms.PSet( Range = cms.string("abs(eta)>%.3f&&abs(eta)<%.3f&&r9>%.2f&&r9<%.2f"),' % (minEta,maxEta,minR9,maxR9)
        outString += ' Shift = cms.double(%.5f), Uncertainty = cms.double(%.5f) ),\n' % (shift,unc)

print
outString += "                      )\n"
print outString        
