#----------------------------------------
# global parameters (independent of model)
#----------------------------------------

# will influence certain parameters
year = 2012

#--------------------

if year == 2011:
    numCategories = 5
    numInclusiveCategories = 4
    inputFile = "../../testdata/CMS-HGG_massfacmva_2011.root"
    catNamePrefix = "cat"
    signalWorkspaceName = "wsig"
    sqrts = 7
elif year == 2012:
    numCategories = 6
    numInclusiveCategories = 4
    
    # this seems to have 7TeV xsects inside
    # inputFile = "../../testdata/CMS-HGG_massfacmva_ichep2012_topup.root"
    inputFile = "../../testdata/CMS-HGG_massfacmva_ichep2012_topup-8tev-sm-xsects.root"

    # catNamePrefix = "mvacat"
    catNamePrefix = "cat"
    signalWorkspaceName = "wsig_8TeV"
    sqrts = 8
else:
    raise Exception("unsupported year " + str(year))

#--------------------
import multiprocessing

globalConfig = {
    "signalModelLibrary": "./ICHEP2012LikeModel.so",

    # the input file containing the different signal MCs
    # "inputFile":          "../../framework/testdata/hgg_withvbf_bkg.input.root",
    "inputFile":          inputFile,

    # sqrts to determine cross sections
    "sqrts":              sqrts,

    "fermiophobic":       False,

    "useRightWrongVertex": True,

    # optional
    "htmlReportFname" : "report-%d.html" % year,

    # output filename
    "outputWorkspaceFname": "ubersignalmodel-%d.root" % year,

    # signal workspace name
    "signalWorkspaceName" : signalWorkspaceName,

    # optional (not written if not specified)
    "outputExtraWorkspaceFname": "extra-%d.root" % year,


    # number of threads to use during fitting (defaults to 1)
    #
    # count number of 'processors' in /proc/cpuinfo (includes hyperthreads).
    # probably does not work on OSX

    # "numCPU": len([x for x in open("/proc/cpuinfo").readlines() if x.startswith('processor')]),
    "numCPU": multiprocessing.cpu_count(),
    
    # optional
    # "signalProcessMerging" : {
    #     # from (input process)
    #     #         to (merged process)
    #     "ggh": "ggh",
    #     "tth": "ggh",
    #     "vbf": "ggh",
    #     "wzh": "ggh",
    #     },

    #--------------------
    # mass interpolation
    #--------------------

    "minMassHypothesis": 110,
    "maxMassHypothesis": 150,
    "massInterpolationWidth": 10,

} # config

#----------------------------------------
# configuration parameters specific
# to this model
#----------------------------------------

modelConfig = {

#   # uncertainty in smearing
#   "signalMCsmearingUncertainty": {
#         # category : value
#         "cat0" : 0.0071,
#         "cat1":  0.010,
#         "cat2": 0.017,
#         "cat3": 0.017,
#         "cat4": 0.0071,
#         },

    "catNamePrefix":          catNamePrefix,
    "numCategories":          numCategories,
    "numInclusiveCategories": numInclusiveCategories,

    }

# number of Gaussians
#
# first index is category, 
# second index is signal process,
# third is 'right' or 'wrong' for the vertex assignment
# value is the number of Gaussians
allProcs = [ 'ggh', 'vbf', 'wzh', 'tth']

import collections
numGaussians = collections.defaultdict(lambda: collections.defaultdict(dict))
if year == 2011:
    assert(numInclusiveCategories == 4)
    assert(numCategories == 5)

    for proc in allProcs:
        cat = 'cat0'; numGaussians[cat]['right'][proc] = 2; numGaussians[cat]['wrong'][proc] = 1
        cat = 'cat1'; numGaussians[cat]['right'][proc] = 3; numGaussians[cat]['wrong'][proc] = 2
        cat = 'cat2'; numGaussians[cat]['right'][proc] = 3; numGaussians[cat]['wrong'][proc] = 2
        cat = 'cat3'; numGaussians[cat]['right'][proc] = 3; numGaussians[cat]['wrong'][proc] = 2

    # process-dependend number for category 4
    cat = 'cat4'; 
    proc = 'ggh'; numGaussians[cat]['right'][proc] = 2; numGaussians[cat]['wrong'][proc] = 1
    proc = 'wzh'; numGaussians[cat]['right'][proc] = 2; numGaussians[cat]['wrong'][proc] = 1
    proc = 'tth'; numGaussians[cat]['right'][proc] = 2; numGaussians[cat]['wrong'][proc] = 1
                                                                                        
    proc = 'vbf'; numGaussians[cat]['right'][proc] = 3; numGaussians[cat]['wrong'][proc] = 2

elif year == 2012:
    assert(numInclusiveCategories == 4)
    assert(numCategories == 6)
    
    tmp = {
        'cat0': { 'ggh': 2, 'vbf': 2, 'wzh': 3, 'tth': 3 },
        'cat1': { 'ggh': 3, 'vbf': 2, 'wzh': 3, 'tth': 3 },
        'cat2': { 'ggh': 3, 'vbf': 2, 'wzh': 2, 'tth': 3 },
        'cat3': { 'ggh': 2, 'vbf': 3, 'wzh': 2, 'tth': 2 },

        'cat4': { 'ggh': 2, 'vbf': 3, 'wzh': 2, 'tth': 2 },
        'cat5': { 'ggh': 2, 'vbf': 3, 'wzh': 2, 'tth': 2 },
        }

    # the number of Gaussians for the wrong vertex assignment
    # is always one less than the one used for the correct
    # vertex assignment (for the official 2012 ICHEP
    # inputs for 8 TeV data)
    
    for cat,data in tmp.items():
        for proc, num in data.items():
            numGaussians[cat]['right'][proc] = num
            numGaussians[cat]['wrong'][proc] = num - 1

else:
    raise Exception("unsupported year " + str(year))

# DEBUG: set all the numbers to 1
# for key, data in numGaussians.items():
#     for key2, data2 in data.items():
#         for key3, data3 in data2.items():
#             data2[key3] = 1

import pprint
pprint.pprint(numGaussians)

modelConfig['numGaussiansMap'] = numGaussians

#----------------------------------------
# smearings applied to MC (read back from
# the ICHEP2012 workspace).
#
# Note that size of the smearing uncertainties
# are defined in the datacards
#----------------------------------------
smearings = {
    '7TeV': {catNamePrefix + '0': 0.006173, 
             catNamePrefix + '1': 0.005892,
             catNamePrefix + '2': 0.007364,
             catNamePrefix + '3': 0.012027,
             catNamePrefix + '4': 0.008646,
             },
    '8TeV': {catNamePrefix + '0': 0.008438,
             catNamePrefix + '1': 0.008715,
             catNamePrefix + '2': 0.009567,
             catNamePrefix + '3': 0.015156,
             catNamePrefix + '4': 0.0117,
             catNamePrefix + '5': 0.011398,
             },
          }

if year == 2011:
    modelConfig['mcSmearings'] = smearings['7TeV']
elif year == 2012:
    modelConfig['mcSmearings'] = smearings['8TeV']
else:
    raise Exception("unsupported year " + str(year))



