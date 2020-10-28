#################################################################################################################################
# Abraham Tishelman-Charny                                                                                                      #
# 23 October 2020                                                                                                               #
#                                                                                                                               #
# The purpose of this module is to provide a class and methods for a SubmitJobs object to be used by the SubmitJobs module.     #
#                                                                                                                               #
#################################################################################################################################

# class SubmitJobs:

#     def __init__(self, primaryDatasetName, secondaryDatasetName):
#         self.primaryDatasetName = primaryDatasetName
#         self.secondaryDatasetName = secondaryDatasetName

#     def GetDatasetNames(self):
#         pass 

#     def GetDsetMetaData(self, dsets):
#         grepString = "nevents"
     

#     def Plot(self, dsets, ol, Process_):
#         nodes, nEvents = GetPlotVals(self.outDsets, dsets, Process_)

def GetMetaConditionsPath(fggDirec_, year_):

    MetaConditionsYearDict = {
        '2016' : "MetaData/data/MetaConditions/Era2016_RR-17Jul2018_v1.json",
        '2017' : "MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1.json",
        '2018' : "MetaData/data/MetaConditions/Era2018_RR-17Sep2018_v1.json"
    }

    MetaDataPath = MetaConditionsYearDict[year_]
    MetaConditionsPath = "%s/%s"%(fggDirec_, MetaDataPath)

    return MetaConditionsPath