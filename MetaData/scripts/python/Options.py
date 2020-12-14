############################################################################################################################################################################################
# Abraham Tishelman-Charny 
# 23 October 2020
#
# # The purpose of this module is to define options for SubmitJobs module.
#
############################################################################################################################################################################################
    
import argparse 

def GetOptions():
    parser = argparse.ArgumentParser()

    ##-- Misc
    parser.add_argument('--fggDirec', type=str, default="", help="Flashgg Directory")
    parser.add_argument('--outDirec', type=str, default="", help="Directory where condor output files will go")
    parser.add_argument('--label', type=str, default="", help="Name for condor output directory in outDirec")
    parser.add_argument('--extraFlagsStr', type=str, default=" ", help="Extra flags")
    parser.add_argument('--nEvents', type=int, default=-1, help="Max number of events to run on")
    # parser.add_argument('--runWorkspaceStd', action="store_true", help="Run workspaceStd cmssw config file")
    parser.add_argument('--saveHHWWggFinalStateVars', action="store_true", help="Save final state variables")
    parser.add_argument('--HHWWgg', action="store_true", help="Do HHWWgg tags, and do HHWWgg tags only")
    parser.add_argument('--doSystematics', action="store_true", help="Run workspaceStd.py systematics")
    parser.add_argument('--dumpTrees', action="store_true", help="Produce output trees")
    parser.add_argument('--dumpWorkspace', action="store_true", help="Produce output RooWorspace(s)")
    parser.add_argument('--dryRun', action="store_true", help="Dry Run. Do not submit jobs")
    parser.add_argument('--doHHWWggTagCutFlow', action="store_true", help="Cut flow")
    parser.add_argument('--doHHWWggNonResAnalysis', action="store_true", help="Do non resonant analysis")
    parser.add_argument('--jsonPath', type=str, default="", help="Optional local json file to use for fggrunjobs arguments such as dataset and campaign")
    parser.add_argument('--condorQueue', type=str, default="microcentury", help="Condor job flavour. Determines max running time for each job")
    parser.add_argument('--year', type=str, default="2017", help="Year to run on. Determines MetaConditions used")   

    args = parser.parse_args()
    return args