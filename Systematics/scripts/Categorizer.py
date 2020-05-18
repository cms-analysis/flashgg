import numpy as np
import matplotlib
# matplotlib.use('Agg')

import scipy.optimize as opt
import pandas as pd
import dill
import argparse
import root_pandas
import iminuit
import probfit
import scipy.integrate as integr
from joblib import Parallel, delayed
from bayes_opt import BayesianOptimization


def pyxponential(x,x0,lambd):
    return lambd*np.exp(-lambd*(x-x0))
    
def getSignificance(sig, bkg, minPeak=None, maxPeak=None, debug=False, plots=False):
        
    if sig.size < 100. or bkg.size < 100. or bkg[:,1].sum()<1000.:
        if debug:
            print('Very low population in Category, setting its significance to ZERO !')
        return 0.

    prefct = 1/(1+np.exp(-0.05*(bkg[:,1].sum()-1000.)))
    if debug:
        print('Prefactor: ', prefct)
        print('Sig weight: ', sig[:,1].sum())
        print('Bkg weight: ', bkg[:,1].sum())
    
    xpon = probfit.functor.Extended(pyxponential, 'N')
    gauss = probfit.pdf.gaussian
    twogauss = probfit.functor.AddPdfNorm(gauss,gauss,prefix=['g1','g2'])
    combined_pdf =  probfit.functor.AddPdf(probfit.functor.Extended(twogauss,'G'), probfit.functor.Extended(pyxponential, 'N'))
    
    
    #     bkg = bkg[np.logical_and(bkg[:,0]>minPeak, bkg[:,0]<maxPeak) ,:]
    data = np.vstack((bkg[np.logical_and(bkg[:,0]>110, bkg[:,0]<140),:],sig[np.logical_and(sig[:,0]>110, sig[:,0]<140),:]))
    sig = sig[np.logical_and(sig[:,0]>110, sig[:,0]<140),:]
        
    binnedLH = probfit.BinnedLH(twogauss,sig[:,0],bins=120,weights=sig[:,1],use_w2=True, extended=False)
    minu = iminuit.Minuit(binnedLH, f_0=0.5, g1mean=125.,g2mean=125., g1sigma=2., g2sigma=2., limit_g1mean=(123,127), limit_g2mean=(123,127), error_f_0 = 0.01, error_g1mean=1., error_g2mean=1., error_g1sigma=0.1, error_g2sigma=0.1, errordef=0.5)
    minu.migrad()
    if debug>1:
        minu.print_fmin()
    if plots:
        binnedLH.show(minu)
            
    dynRange = np.sqrt((minu.fitarg['f_0']*minu.fitarg['g1sigma'])**2+((1-minu.fitarg['f_0'])*minu.fitarg['g2sigma'])**2)
    sigMean = minu.fitarg['f_0']*minu.fitarg['g1mean']+(1-minu.fitarg['f_0'])*minu.fitarg['g2mean']

    if np.any(np.isnan(minu.np_errors())) or not minu.get_fmin()['is_valid']:
        print("WARNING: Two gaussian fit failed using only one")
        
        binnedLH = probfit.BinnedLH(gauss,sig[:,0],bins=120,weights=sig[:,1],use_w2=True, extended=False)  
        minu = iminuit.Minuit(binnedLH, mean=125., sigma=1., limit_mean=(123,127), error_mean=1, error_sigma=0.1, errordef=0.5)
        res = minu.migrad()
        combined_pdf =  probfit.functor.AddPdf(probfit.functor.Extended(gauss,'G'), probfit.functor.Extended(pyxponential, 'N'))
        if plots:
            binnedLH.show(minu)
        if debug>1:
            minu.print_fmin()
        dynRange = minu.fitarg['sigma']
        sigMean = minu.fitarg['mean']

    binnedLH_bkg = probfit.BinnedLH(pyxponential,bkg[:,0],bins=160,weights=bkg[:,1],use_w2=True, extended=False)
    minu_bkg = iminuit.Minuit(binnedLH_bkg, lambd=0.03, x0=100, error_lambd=0.003, error_x0=10., errordef=0.5)
    minu_bkg.migrad()
    if plots:
        binnedLH_bkg.show(minu_bkg)
    if debug>1:
        minu_bkg.print_fmin()
        
    fix_dict = {}
    for par in minu.parameters + minu_bkg.parameters:
        fix_dict['fix_{}'.format(par)]=True
    
    binnedLH_comb = probfit.BinnedLH(combined_pdf,data[:,0],bins=60,weights=data[:,1],use_w2=True, extended=True)
    minu_comb = iminuit.Minuit(binnedLH_comb, errordef=0.5, N=bkg[:,1].sum(), G=sig[:,1].sum(), error_N=bkg[:,1].sum()/10., error_G=sig[:,1].sum()/10., limit_G = (0.,2 * sig[:,1].sum()), **dict(minu.values), **dict(minu_bkg.values), **fix_dict)
    minu_comb.migrad()
    if plots:
        binnedLH_comb.show(minu_comb)
    if debug>1:
        minu_comb.print_fmin()
            
    if minPeak is None:
        minPeak = sigMean - dynRange
        if debug:
            print(minPeak)
    if maxPeak is None:
        maxPeak = sigMean + dynRange
        if debug:
            print(maxPeak)

    return prefct*(integr.quad(lambda x: combined_pdf(x, *minu_comb.np_values()), minPeak, maxPeak)[0] - integr.quad(lambda x: dict(minu_comb.values)['N'] * pyxponential(x, *minu_bkg.np_values()), minPeak, maxPeak)[0])/np.sqrt(integr.quad(lambda x: dict(minu_comb.values)['N'] * pyxponential(x, *minu_bkg.np_values()), minPeak, maxPeak)[0])

    
class Optimizer(object):

    def __init__(self, dfSig, dfsBkg, cutVar='sigmaMoM_decorr', addVars=['leadmva', 'subleadmva'], debug=False, plotFits=False, setZero=True):

        self.dfSig = dfSig.loc[:, [cutVar, 'CMS_hgg_mass', 'weight'] + addVars ]
        self.dfBkg = dfsBkg[0].loc[:, [cutVar, 'CMS_hgg_mass', 'weight'] + addVars ]
        for dfBackground in dfsBkg[1:]:
            self.dfBkg = pd.concat([self.dfBkg, dfBackground.loc[:, [cutVar, 'CMS_hgg_mass', 'weight'] + addVars ]], ignore_index=True)

        self.cutVar = cutVar
        self.debug = debug
        self.plots = plotFits
        self.setZero = setZero

    
    def selectSigBkg(self, cutLow, cutHigh, addCut=None):

        if len(addCut) is not None:
            cut = addCut + ' and {0} > {1} and {0} < {2}'.format(self.cutVar, cutLow, cutHigh)
        else:
            cut = '{0} > {1} and {0} < {2}'.format(self.cutVar, cutLow, cutHigh)
        if self.debug:
            print(cut)
            
        bkg = np.array(self.dfBkg.query(cut,engine='python').loc[:,['CMS_hgg_mass','weight']].values,dtype=np.float64)
        sig = np.array(self.dfSig.query(cut,engine='python').loc[:,['CMS_hgg_mass','weight']].values, dtype=np.float64)
        
        return sig, bkg

    
    def sigOneCat(self, cutLow, cutHigh, addCut):

        sig, bkg = self.selectSigBkg(cutLow, cutHigh, addCut)
        ret = self.getSignificance(sig, bkg, minPeak=None, maxPeak=None)
        if self.debug:
            print(ret)
        return ret

    
    def sigMultCat(self, cuts, addCut):

        ovSigSq = 0.
        signals = []
        bkgs = []
        for i, binn in enumerate(cuts[:-1]):
            sig,bkg = self.selectSigBkg(binn, cuts[i+1], addCut)
            signals.append(sig)
            bkgs.append(bkg)
    
        # sigfs = Parallel(n_jobs=len(signals))(delayed(getSignificance)(signals[i], bkgs[i]) for i in range(len(signals)))
        sigfs = [getSignificance(signals[i], bkgs[i], debug=self.debug, plots=self.plots) for i in range(len(signals))]
        for sigf in sigfs:
            if self.debug:
                print(sigf)
            ovSigSq += sigf**2

        if np.any(np.array(sigfs)<0.005*np.array(sigfs).max()) and self.setZero:
            if self.debug:
                print('Significance of one Category very low, setting overall significance to ZERO !')
            return 0.

        ovSig = np.sqrt(ovSigSq)
        if self.debug:
            print(ovSig)
            
        return ovSig

    def optimizeOneCut(self, start, cutOpt, indOpt, addCut):

        cuts = start
        cuts[indOpt] = cutOpt
        return self.sigMultCat(cuts, addCut)
        
    
    def iterativeOptim(self, nCats, addCut, start):

        currMin = 0.
        ovSigSq = 0.
        nCat = 0
        cats = []
        currCut = start
        while nCat < nCats:
            minuCategorization = iminuit.Minuit(lambda cut: -self.optimizeOneCut(currCut, cut, nCat+1, addCut=addCut), cut=currCut[nCat+1], error_cut=0.5*currCut[nCat+1], limit_cut=(currCut[nCat], None))
            minuCategorization.migrad()
            optimCut = minuCategorization.np_values()
            fval = np.array([minuCategorization.fval])
            ovSigSq += fval**2
            # cats.append(optimCut)
            currMin = optimCut[0]
            currCut[nCat + 1] = currMin
            if self.debug:
                print('Significance: ', fval)
                print('Cut Value: ', optimCut[0])
            nCat += 1

        ovSig = self.sigMultCat(currCut, addCut)

        return currCut, ovSig
