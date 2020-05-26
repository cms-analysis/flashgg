#!/usr/bin/env python
# 
# --------------------------------------------
# Standard python import
from optparse import OptionParser, make_option
import fnmatch, glob, os, sys, json, itertools, array
#sys.argv.append( '-b' )

## ------------------------------------------------------------------------------------------------------------------------------------------------------



from ROOT import TH2D, TH1D, TFile, TProfile, TCanvas, TGraphAsymmErrors
from ROOT import RooWorkspace
from ROOT import RooAbsData
from ROOT import RooDataSet
from ROOT import *

#gSystem.Load('transfBDT_C.so')


class Load:
    def __call__(self,option, opt_str, value, parser, *args, **kwargs):
        if option.dest == "__opts__":
            dest = parser.values
        else:
            dest = getattr(parser.values,option.dest)
        origin = getattr(parser.values,"%s_src" % option.dest,None)
        if origin:
            origin += ",%s" % value
        else:
            origin = value
            setattr(parser.values,"%s_src" % option.dest,origin)

        if type(dest) == dict:
            setter = dict.__setitem__
            getter = dict.get
        else:
            setter = setattr
            getter = getattr
        
        for cfg in value.split(","):
            cf = open(cfg)
            settings = json.loads(cf.read())
            for k,v in settings.iteritems():
                print k,v
                attr  = getter(dest,k,None)
                if attr and type(attr) == list:           
                    attr.extend(v)
                setter(dest,k,v)
            cf.close()



#"variables": [
#        ("genPt","recoPt",[0.0,100.0,200.0,300.0,400.0,500.0,600.0,700.0,10000.0],[0.0,10000.0]), 
#        ("genNjets2p5","recoNjets2p5",[-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,100.0],[]) ],



#variables=[("leadGenPt", "subleadGenPt", [0.,100.,200.,300.,400.,500.,600.,700.,10000.])]
variables=[]
#[["genPt", "recoPt", [0.,100.,200.,300.,400.,500.,600.,700.,10000.],[0.,10000.]], ["genNjets2p5", "recoNjets2p5", [-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,100.],[]] ]
procs=[]
#procs=["InsideAcceptance_130_13TeV"]
cats=[]
#cats=["SigmaMpTTag_0", "SigmaMpTTag_1", "SigmaMpTTag_2"]



savefmts=['.png','.root','.pdf','.jpg']
# Main routine
def main(o,args):
    variables = options.variables
    procs = options.procs
    cats = options.cats
    print "****variables***"
    print variables
    #from configfile import variables, procs, cats
    if 0 and options.files.startswith("/store"):
        filepath = "root://eoscms/"+str(options.files)
    else:
        filepath = options.files
    print 'filepath is '+str(filepath)
    infile = TFile.Open(filepath)
    infile.Print()
    wspace = infile.Get("tagsDumper/cms_hgg_13TeV")
    wspace.Print()

    isData=False
    dname=""
    print variables
    for varset in variables:
        print 'we look at the variable ...'
        print varset
        if not varset[-1]:
                    #varset[-1]=varset[-2]
            varset = (varset[0], varset[1], varset[2], varset[2])
        if not varset[1]:
            varset = (varset[0], varset[0], varset[2], [1,1])
            isData=True
        datasetsReduced={}
        for proc in procs:
            for cat in cats:
                print "reading ds "+str(proc)+"_"+str(cat)+" from ws"
                dataset = wspace.data(str(proc)+"_"+str(cat))
                dataset.Print()
                for bound1 in range(len(varset[-2])-1):
                    for bound2 in range(len(varset[-1])-1):
                        lowb1=str(varset[-2][bound1]).replace("-","m").replace(".","p")
                        upb1=str(varset[-2][bound1+1]).replace("-","m").replace(".","p")
                        if isData:
###                            dname = str(proc)+"_"+str(cat)+"_"+varset[0]+"_"+str(varset[-2][bound1])+"to"+str(varset[-2][bound1+1])
                            dname = str(proc)+"_"+str(cat)+"_"+varset[0]+"_"+str(lowb1)+"to"+str(upb1)
                        else:
#                            dname = str(proc)+"_"+str(cat)+"_"+varset[0]+"_"+str(varset[-2][bound1])+"to"+str(varset[-2][bound1+1])+"_Vs_"+varset[1]+"_"+str(varset[-1][bound2])+"to"+str(varset[-1][bound2+1])
                            dname = str(proc)+"_"+varset[0]+"_"+str(varset[-2][bound1])+"to"+str(varset[-2][bound1+1])+"_"+str(cat)+"_"+varset[1]+"_"+str(varset[-1][bound2])+"to"+str(varset[-1][bound2+1])
                        datRed = dataset.Clone(dname)
                        datRed.reset()
                        datasetsReduced[ dname ] = datRed
                val=0.
                maxEntries = dataset.numEntries()
                if options.maxEntries != -1:
                    maxEntries = options.maxEntries
                for i in range(maxEntries):
                    if i%10000==0: 
                        print i
                    iset = dataset.get(i)
                    val1 = iset.getRealValue(varset[0])
                    val2 = iset.getRealValue(varset[1])
#                    print val1,val2

                    for bound1, bound2 in ((b1,b2) for b1 in range(len(varset[-2])-1) for b2 in range(len(varset[-1])-1) ):
                        lowb1=str(varset[-2][bound1]).replace("-","m").replace(".","p")
                        upb1=str(varset[-2][bound1+1]).replace("-","m").replace(".","p")
                        if isData:
                            selCondition = (val1 > varset[-2][bound1]) and (val1 < varset[-2][bound1+1]) 
                            dname = str(proc)+"_"+str(cat)+"_"+varset[0]+"_"+str(lowb1)+"to"+str(upb1)
                        else:
                            selCondition = (val1 > varset[-2][bound1]) and (val1 < varset[-2][bound1+1]) and (val2 > varset[-1][bound2]) and (val2 < varset[-1][bound2+1])
#                            dname = str(proc)+"_"+str(cat)+"_"+varset[0]+"_"+str(varset[-2][bound1])+"to"+str(varset[-2][bound1+1])+"_Vs_"+varset[1]+"_"+str(varset[-1][bound2])+"to"+str(varset[-1][bound2+1])
                            dname = str(proc)+"_"+varset[0]+"_"+str(varset[-2][bound1])+"to"+str(varset[-2][bound1+1])+"_"+str(cat)+"_"+varset[1]+"_"+str(varset[-1][bound2])+"to"+str(varset[-1][bound2+1])
                        if selCondition:
#                            print "filling dataset"
##                            print dataset.weight()
                            datasetsReduced[dname].add(iset, dataset.weight())
                            break

#            print "####  Compare reductions  ####"
        new_wspace = RooWorkspace("cms_hgg_13TeV")
        getattr(new_wspace, 'import')(wspace.var("CMS_hgg_mass"))
        getattr(new_wspace, 'import')(wspace.var("IntLumi"))
        if not isData:
            getattr(new_wspace, 'import')(wspace.var("dZ"))
        getattr(new_wspace, 'import')(wspace.var("weight"))
#    alldata = new_wspace.allData()
#    for ds in alldata :
#        new_wspace.removeSet(str(ds.GetName()))
        outfilename = options.outfile.split('.')[0]+'_'+str(varset[0])+'_1bis.root'
        outfile = TFile(outfilename, 'RECREATE')
        stepsize=int(len(datasetsReduced.keys())/10)
        iteration = 0
        while(len(datasetsReduced.keys())>0):
            print 'iteration '+str(iteration)
            iteration=iteration+1
            try: outfile
            except NameError: outfile = TFile(outfilename, 'UPDATE')

            try: new_wspace
            except NameError: 
                outfile.ls()
                new_wspace = outfile.Get("cms_hgg_13TeV")
                print "cms_hgg_13TeV;1"
                gDirectory.Delete("cms_hgg_13TeV;1")
                outfile.ls()
#        if not 'outfile' in globals():
#            print 'outfile is not defined'
#            outfile = TFile(options.outfile, 'UPDATE')
#        if not 'new_wspace' in globals():
#            new_wspace = outfile.Get("cms_hgg_13TeV")
#            print 'newspace is not defined'
            written=[]
            if stepsize > len(datasetsReduced.keys()):
                stepsize = len(datasetsReduced.keys())
            for ikey in range(stepsize):
            #    for ikey in range(len(datasetsReduced.keys())):
            #        dataset.Print("V")
            #        if dataset.numEntries() > 100:
                getattr(new_wspace, 'import')(datasetsReduced[datasetsReduced.keys()[ikey]])
                datasetsReduced[datasetsReduced.keys()[ikey]].Write()
                written.append(datasetsReduced.keys()[ikey])
            print 'Number of data in ws '
            print len(new_wspace.allData())
        #new_wspace.Print()
            new_wspace.Write()
            outfile.Write()
            outfile.Close()
            for wkey in written:
                del datasetsReduced[wkey]
            del new_wspace
            del outfile


#    for dataset in datasetsReduced.values():
##        dataset.Print("V")
##        if dataset.numEntries() > 100:
#        getattr(new_wspace, 'import')(dataset)
#        dataset.Write()
##            datasetReduce.Print("V")
##            datasetReduce2.Print("V")
#    histos={}
#    histos= getPlots(infile)
#    #draw and save plots
#    outfile.cd()
#    for histo in histos:
#        histo.Write()
#        c = TCanvas('c','',1)
#        histo.Draw('ehist')
#        c.SetLogy(variables[str(histo.GetName()).split('_')[1]]['logy'])
#        c.SetLogx(variables[str(histo.GetName()).split('_')[1]]['logx'])
#        for fmt in savefmts:
#            c.SaveAs(str(options.outdir)+'/'+str(histo.GetName())+str(fmt))
#    new_wspace.Write()

#    outfile.Write()
#    outfile.Close()

    

## ------------------------------------------------------------------------------------------------------------------------------------------------------    
if __name__ == "__main__":
    parser = OptionParser(option_list=[
            make_option("-i", "--indir",
                        action="store", type="string", dest="indir",
                        default="./",
                        help="input directory", metavar="DIR"
                        ),
            make_option("-f", "--files",
                        action="store", type="string", dest="files",
                        default="allSig125IA.root",
                        help="pattern of files to be read", metavar="PATTERN"
                        ), 
            make_option("-t", "--treeName",
                        action="store", type="string", dest="treename",
                        default="TestTree",
                        help="TTree name", metavar="TREENAME"
                        ),
            make_option("-o", "--outfile",
                        action="store", type="string", dest="outfile",
                        default="reduced.root",
                        help="outputfile", metavar="FILE"
                        ),
            make_option("-l", "--label",
                        action="store", type="string", dest="label",
                        default="",
                        help="label", metavar="LABEL"
                        ),
            make_option("-w", "--weightsdir",
                        action="store", type="string", dest="weightsdir",
                        default="weights",
                        help="tmva weights dir", metavar="DIR"
                        ),
#            make_option("-V", "--variables",
#                        action="store", dest="variables", type="string",
#                        default="",
#                        help="list of variables"
#                        ),
            make_option("-T", "--tmvaSettings",
                        action="store", dest="tmvaSettings", type="string",
                        default="dipho.json",
                        help="settings for the TMVA training"
                        ),
            make_option("-v", "--verbose",
                        action="store_true", dest="verbose",
                        default=False,
                        ),
            make_option("-O", "--optimize",
                        action="store_true", dest="optimize",
                        default=False,
                        ),
            make_option("-D", "--outputdir",
                        action="store_true", dest="outdir",
                        default="plots",
                        ),
            make_option("-N", "--maxEntries",
                        action="store", type="int", dest="maxEntries",
                        default=-1,
                        ),
            make_option("--load",  # special option to load whole configuaration from JSON
                        action="callback",callback=Load(),dest="__opts__",
                        type="string",
                        help="load JSON file with configuration",metavar="reduce_cfg.json"
                        ),

            ])

    (options, args) = parser.parse_args()

    sys.argv.append("-b")
    main(options, args)
