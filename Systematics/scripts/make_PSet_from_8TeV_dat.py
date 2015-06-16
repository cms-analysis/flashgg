# EBLowEtaBad 0 0 1 -999.00 0.94 190645 190781 0.0078 0.0004

#        cms.PSet (lowBounds = cms.vdouble(1.0,-999.), upBounds =  cms.vdouble(1.5,0.94), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.0012 )),                                                                                          
# category      type(0=any,1=CM,2=GAP) minEta  MaxEta   mr9    Mr9      mrun    Mrun   scaleC  scaleErr
# EBlowEtaGold8TeV 0    0.0              1.0     0.94     999.00 -999999  999999  0.0    0.05e-2

outString = """scaleBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
"""

#outString = "scaleBins = cms.VPSet(\n"

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
#        print label,minEta,maxEta,minR9,maxR9,shift,unc
        outString += 21*" "
        outString += 'cms.PSet( lowBounds = cms.vdouble(%.3f,%.3f), upBounds = cms.vdouble(%.3f,%.3f),\n' % (minEta,minR9,maxEta,maxR9)
        outString += 30*" "
        outString += 'values = cms.vdouble( %.5f ), uncertainties = cms.vdouble( %.5f ) ),\n' % (shift,unc)
#        outString += 'cms.PSet( Range = cms.string("abs(eta)>%.3f&&abs(eta)<%.3f&&r9>%.2f&&r9<%.2f"),' % (minEta,maxEta,minR9,maxR9)
#        outString += ' Shift = cms.double(%.5f), Uncertainty = cms.double(%.5f) ),\n' % (shift,unc)

print
outString += "                    ))\n"
print outString        

outString = """smearBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
"""

for line in open("smearing_sigma_and_errors_stocastic_rd_mc.dat").readlines():
    splitline = line.strip().split()
    if len(splitline) == 14:
        label = splitline[0]
        type = int(splitline[1])
        minEta = float(splitline[2])
        maxEta = float(splitline[3])
        minR9 = float(splitline[4])
        maxR9 = float(splitline[5])
        minRun = int(splitline[6])
        maxRun = int(splitline[7])
        emean = float(splitline[8])
        emean_err = float(splitline[9])
        rho = float(splitline[10])
        rho_err = float(splitline[11])
        phi = float(splitline[12])
        phi_err = float(splitline[13])
#        print label,minEta,maxEta,minR9,maxR9,emean,emean_err,rho,rho_err,phi,phi_err
        outString += 21*" "
        outString += 'cms.PSet( lowBounds = cms.vdouble(%.3f,%.3f), upBounds = cms.vdouble(%.3f,%.3f),\n' % (minEta,minR9,maxEta,maxR9)
        outString += 30*" "
        if emean == 0.:
            outString += 'values = cms.vdouble( %.5f ), uncertainties = cms.vdouble( %.5f ) ),\n' % (rho,rho_err)
        else:    
            outString += 'values = cms.vdouble( %.5f, %.5f, %.5f ), uncertainties = cms.vdouble( %.5f, %.5f ) ),\n' % (rho,phi,emean,rho_err,phi_err)

print
outString += "                    ))\n"
print outString


# category      type(0=any,1=CM,2=GAP) minEta  MaxEta   mr9    Mr9      mrun    Mrun   Emean  Emean_err rho rho_err phi phi_err
#EBlowEtaBad8TeV    0 0.0 1.0 -999. 0.94 -999999 999999 6.73 0. 7.7e-3  6.32e-4 0.00 0.16
