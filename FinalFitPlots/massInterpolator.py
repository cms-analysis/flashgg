#!/bin/env python

from optparse import OptionParser, make_option
import sys
import array

def main(options,args):

    import ROOT
    import numpy
    
    ## Load library
    ROOT.gSystem.Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so");
    ROOT.gSystem.Load("../libLoopAll.so")
    from copy import copy

    ## input and output
    if ".root" in options.input:
        options.input = options.input.replace(".root","")
    elif not "/" in options.input:
        options.input = "%s/CMS-HGG" % options.input
        
    if options.output == "":
        options.output = copy(options.input)
    if ".root" in options.output:
        options.output = options.output.replace(".root","")

    ## input masses
    # Warning harded skipping of 145 for 7TeV
    inputMasses = numpy.array( [ float(m) for m in options.inputMasses.split(",") if not options.is7TeV or m!='145' ] )

    ## build list of mass points to interpolate
    interpolations = []

    if options.outputMasses != "":
        interpolations.append( (numpy.array( [float(m) for m in options.outputMasses.split(",")] ), "%s_interpolated.root" % options.output) )
    elif options.pval:
        interpolations.append( (numpy.arange(110,150.5,0.5), "%s_interpolated.root" % options.output) )
    else:
        if options.massScanType == "coarse" :
            interpolations.append( (numpy.concatenate( (numpy.arange(120,122.5,0.5), numpy.arange(123,126,0.1), numpy.arange(126,130.5,0.5)) ), "%s_mass_scan_coarse.root" % options.output) )
        elif options.massScanType == "fine" :
            interpolations.append( (numpy.arange(124.5,125.82,0.02), "%s_mass_scan_fine.root" % options.output) )
        else:
            interpolations.append( (numpy.concatenate( (numpy.arange(120,122.5,0.5), numpy.arange(123,124.5,0.1), numpy.arange(124.5,125.82,0.02), numpy.arange(125.9,126,0.1), numpy.arange(126,130.5,0.5)) ), "%s_mass_scan.root" % options.output) )

    # just rescale an already interpolated ws
    if options.rescaleOnly:
        inputMasses = interpolations[0][0]
        options.doSmoothing = False

    ROOT.RooMsgService.instance().setGlobalKillBelow(ROOT.RooFit.FATAL)

    ## open input file
    fin  = ROOT.TFile.Open("%s.root" % options.input)
    fkeys = fin.GetListOfKeys()

    ## Read input workspace
    ws   = fin.Get(options.workspace)
    inputLumi = ws.var("IntLumi")

    # figure out specific scaling options for spin
    scalingOptions = {} 
    for opt in options.scaleOptions:
      scalingOptions[opt.split(':')[0]] = [float(opt.split(':')[1]),opt.split(':')[2].split(',')]
        
    ## scale histograms by lumi
    if options.lumi > 0.:
        options.scaleSignal = options.lumi / inputLumi.getVal()
        inputLumi.setVal(options.lumi)
    
    ## Read input histograms
    inputs = {}
    procs    = set()
    channels = set()
    tocopy = []
    for k in fkeys:
        if options.th1Prefix in k.GetName():
            if '_grav' in k.GetName() or '_spin2' in k.GetName():
              decomp = str(k.GetName()).replace(options.th1Prefix,"").split("_",4)
              proc, mass, channel = decomp[0]+'_'+decomp[1], float(decomp[3].replace("m","")), decomp[4]
            else:
              decomp = str(k.GetName()).replace(options.th1Prefix,"").split("_",3)
              proc, mass, channel = decomp[0], float(decomp[2].replace("m","")), decomp[3]
            h = k.ReadObj()
            if mass in inputMasses:
                inputs[ (mass, proc, channel) ] = h
                procs.add(proc)
                channels.add(channel)
                if options.scaleSignal>0.:
                    h.Scale(options.scaleSignal)
            else:
                tocopy.append(h)
        elif "TH1" in k.GetClassName() or "TCanvas" in k.GetClassName():
            tocopy.append(k.ReadObj())
  
    # special scaling options
    # have to work out norm factors first
    normFactors = {}
    cats = [x for x in channels if 'sigma' not in x and 'rv' not in x and 'wv' not in x]
    for proc, scOpts in scalingOptions.items():
      scalingFactor = scOpts[0]
      scalingProcs = scOpts[1]
      for m in inputMasses:
        norm_num=0.
        norm_dem=0.
        for cat in cats:
          for scaleProc in scalingProcs:
            norm_num += inputs[m,scaleProc,cat].Integral()
          norm_dem += inputs[m,proc,cat].Integral()
        normFactors[(m,proc)] = scalingFactor*norm_num/norm_dem
    
    # can now scale histogram appropriately
    for (mass,proc,channel), hist in inputs.items():
      if proc in scalingOptions.keys():
        hist.Scale(normFactors[(mass,proc)])

    ## Loop over interpolations
    doSmoothing = options.doSmoothing
    files = []
    for masses, file in interpolations:

        print "-------------------------------------------------------------------- "
        print "Will generate the following mass points: ", masses
        print "Output file            : ", file
        print "Input masses           : ", inputMasses
        print "Sig k-factor           : ", options.scaleSignal
        print "Processes              : ", procs
        print "# of channels          : ", len(channels)
        if scalingOptions:
          print "Special scaling options: "
          for key, items in scalingOptions.items():
            print "\t", key, items[0], items[1]

        ## instantiate the mass interpolator
        interpolator = ROOT.MassInterpolator(len(masses), masses, doSmoothing, "th1f_sig_", "roohist_sig_" )
        
        # parse special options for sample normalisation (e.g. spin)
        for key,item in scalingOptions.items():
          myvec = ROOT.vector('string')()
          for el in item[1]: myvec.push_back(el)
          interpolator.addToMap(key,item[0],myvec)

        ## loop over processes, channels and input masses and fill the interpolator
        print "\nFilling interpolator"
        print "-----------------------"
        for p in procs:
            print p
            for c in channels:
                for m in inputMasses:
                    interpolator.addInput(m, p, c, inputs[(m,p,c)] )
        
        ## prepare output
        print "\nPreparing output"
        print "-----------------------"
        fout = ROOT.TFile.Open(file,"recreate")
        # transfer macros
        for key in fin.GetListOfKeys():
          obj = key.ReadObj()
          if type(obj)==type(ROOT.TMacro()):
            fout.cd()
            print obj.GetName()
            obj.Write()
        
        files.append(fout)
        fout.cd()
        for o in tocopy:
            o.Write(o.GetName())
            
        ows  = ws.Clone()
        files.append(ows)
        mass = ows.var(options.massName)

        interpolator.printMap()
        ## run interpolation
        print "Running interpolation"
        print "-----------------------"
        interpolator.outputFile(fout)
        interpolator.outputWs(ows,mass)
        interpolator.runInterpolation(options.is7TeV)

        ## save output
        print "\nSaving file"
        print "-----------------------"
        fout.cd()
        ows.Write( options.workspace )
        fout.Close()
        
        ## smooth the histograms only once
        doSmoothing = False
            
if __name__ == "__main__":
    parser = OptionParser(option_list=[
        make_option("-i", "--input",
                    action="store", type="string", dest="input",
                    default="",
                    help="Input file name", metavar=""
                    ),
        make_option("-o", "--output",
                    action="store", type="string", dest="output",
                    default="",
                    help="Output file name", metavar=""
                    ),
        make_option("--is7TeV",
                    action="store_true", dest="is7TeV",
                    default=False,
                    help="", metavar=""
                    ),
        make_option("-w","--workspace",
                    action="store", type="string", dest="workspace",
                    default="cms_hgg_workspace",
                    help="Name of the workspace", metavar=""
                    ),
        make_option("-m","--massName",
                    action="store", type="string", dest="massName",
                    default="CMS_hgg_mass",
                    help="", metavar=""
                    ),
        make_option("-I","--inputMasses",
                    action="store", type="string", dest="inputMasses",
                    default="110,115,120,125,130,135,140,145,150",
                    help="List input mass points", metavar=""
                    ),
        make_option("-O","--outputMasses",
                    action="store", type="string", dest="outputMasses",
                    default="",
                    help="List of output mass points", metavar=""
                    ),
        make_option("-p","--pval",
                    action="store_true", dest="pval",
                    default=True,
                    help="Generate output for the standard pvalue scan, ie 0.5GeV steps (default). ", metavar=""
                    ),
        make_option("-S","--massScan",
                    action="store_false", dest="pval",
                    help="Generate output for the mass scan", metavar=""
                    ),
        make_option("-s","--massScanType",
                    action="store", type="string", dest="massScanType",
                    default="",
                    help="", metavar=""
                    ),
        make_option("-k","--scaleSignal",
                    action="store", type="float", dest="scaleSignal",
                    default=0.,
                    help="Re-scale the signal normalization", metavar=""
                    ),
        make_option("-l","--lumi",
                    action="store", type="float", dest="lumi",
                    default=0.,
                    help="Normalize the signal to a given luminosity (and update IntLumi). Expressed in 1/pb.", metavar=""
                    ),
        make_option("-t","--th1Prefix",
                    action="store", type="string", dest="th1Prefix",
                    default="th1f_sig_",
                    help="Histograms prefix", metavar=""
                    ),
        make_option("--doSmoothing",
                    action="store_true", dest="doSmoothing",
                    default=True,
                    help="Smooth the input histograms", metavar=""
                    ),
        make_option("--noSmoothing",
                    action="store_false", dest="doSmoothing",
                    help="", metavar=""
                    ),
        make_option("--rescaleOnly",
                    action="store_true", dest="rescaleOnly",
                    default=False,
                    help="Just rescale an already interpolated ws (implies noSmoothing)", metavar=""
                    ),
        make_option("-H","--scaleOptions",
                    action="append", dest="scaleOptions",
                    default=[], 
                    help="""Set different scaling options for different samples. Used for spin analysis rescaling. Can pass multiple times needs to be colon separated (procToScale:Factor:normalisationProcs) 
                    E.g to scale the gg_grav and qq_grav samples to the SM pass -H gg_grav:1.:ggh,vbf,wzh,tth -H qq_grav:1.:ggh,vbf,wzh,tth""", metavar=""
                    ),
        ]
                          )
    
    (options, args) = parser.parse_args()

    sys.argv.append("-b")
    main( options, args ) 
    
