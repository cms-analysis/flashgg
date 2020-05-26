import pandas as pd
import numpy as np
import root_numpy
import ROOT
import itertools


class RooDatasetFromDataframe(object):

    def __init__(self, df, variables, weight, dsetName):
        
        self.df = df
        self.weight = weight
        self.parseVariables(variables)
        self.variables = [var[0] for var in variables]
        self.name = dsetName

        if 'weight' not in self.variables:
            self.variables.append('weight')

    def prepareTTree(self):

        self.df['weight'] = self.df.eval(self.weight)
        rec = self.df.loc[:, self.variables].to_records(index=False)
        self.tree = root_numpy.array2tree(rec)

    def parseVariables(self, variables):

        self.rooRealVars = []
        for tpl in variables:
            self.rooRealVars.append(ROOT.RooRealVar(tpl[0], tpl[0], tpl[1], tpl[2]))

    def prepareDataset(self):

        argSet = ROOT.RooArgSet()
        for var in self.rooRealVars:
            argSet.add(var)

        if not hasattr(self, 'tree'):
            self.prepareTTree()

        self.dataSet = ROOT.RooDataSet(self.name, self.name, self.tree, argSet, '1', 'weight')

    def getDataset(self):

        self.prepareDataset()
        return self.dataSet


class RooWorkspaceFromDataframe(object):

    def __init__(self, df, splitDic, variables, weight, wsName, dsetPreSuffix=None, workspace=None):

        self.splitDic = splitDic
        self.variables = variables
        self.weight = weight
        self.name = wsName
        if dsetPreSuffix is not None:
            self.dsetPreSuffix = dsetPreSuffix

        if workspace is not None and isinstance(workspace, ROOT.RooWorkspace):
            self.workspace = workspace
        elif workspace is not None:
            print("Object passed to workspace variable has to be a RooWorkspace. Object you passed is not and will not be read, a new RooWorkspace will be created")

        self.splitTree(df.loc[:, [var[0] for var in variables] + splitDic.keys()])

    def splitTree(self, df):

        binvars = []
        for key in self.splitDic.keys():
            df['{}_bin'.format(key)] = pd.cut(df[key], bins=self.splitDic[key], labels=self.makeBinLabels(key, self.splitDic[key]), include_lowest=True, right=False)
            
            binvars.append('{}_bin'.format(key))

        self.gb = df.groupby(binvars)

    @staticmethod
    def makeBinLabels(var, bins):
        return ['{}_{}_{}'.format(var, bins[i], bins[i+1]).replace('.', 'p').replace('-', 'm') for i in range(len(bins) - 1)]

    def makeCategories(self):

        self.labels = []
        for key in self.splitDic.keys():
            self.labels.append(self.makeBinLabels(key, self.splitDic[key]))
        self.categories = list(itertools.product(*self.labels))
        
    def makeWorkspace(self):

        if not hasattr(self, 'workspace'):
            self.workspace = ROOT.RooWorkspace(self.name)
            
        dummyDf = pd.DataFrame(columns=[var[0] for var in self.variables], dtype=np.float32)
        
        for cat in self.categories:
            dsetlabel = ''
            for varl in cat:
                dsetlabel += '{}_'.format(varl)
            dsetlabel = dsetlabel[:-1]

            if hasattr(self, 'dsetPreSuffix'):
                dsetlabel = '{}_{}_{}'.format(self.dsetPreSuffix[0], dsetlabel, self.dsetPreSuffix[1])

            if cat in self.gb.indices.keys():
                dsetFrDf = RooDatasetFromDataframe(self.gb.get_group(cat), self.variables, self.weight, dsetlabel)
            else:
                dsetFrDf = RooDatasetFromDataframe(dummyDf, self.variables, self.weight, dsetlabel)

            dset = dsetFrDf.getDataset()
            getattr(self.workspace, 'import')(dset, ROOT.RooFit.Rename(dset.GetName()))

    def getWorkspace(self):

        self.makeCategories()
        self.makeWorkspace()
        
        return self.workspace
