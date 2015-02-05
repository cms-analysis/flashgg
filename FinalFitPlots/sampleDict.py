# Automagically guess a standard sample string from another string

import re
from pprint import pprint

def sampleString(dirName):
    for pattern, props in sampleInfo.iteritems():
        if pattern.search(dirName):
            template = props['template']
            try:
                d = props['regex'].search(dirName).groupdict()
                d.update(props)
                return template % d
            except KeyError:
                return template

templateSignal = '%(name)s_H_gg_%(mass)s_pu2011_0'
regexSignal = re.compile('M-(?P<mass>\d+)')

templateQCD = '%(name)s_%(combination)s_pu2011_%(threshold)s'
regexQCD = re.compile('Pt(-{0,1})(?P<threshold>\d+)(.*?)\_(?P<combination>\w{2})')

templateDY = '%(name)s_pu2011_%(threshold)s'
regexDY = re.compile('(M|Pt)(?P<threshold>\d+)')


sampleInfo = {
    re.compile('GluGlu') : {
    'name':'gluglu', 'template':templateSignal, 'regex':regexSignal
    },

    re.compile('VBF')    : {
    'name':'vbf', 'template':templateSignal, 'regex':regexSignal
    },

    re.compile('WH_ZH')  : {
    'name':'wz', 'template':templateSignal, 'regex':regexSignal
    },

    re.compile('TTH')    : {
    'name':'tth', 'template':templateSignal, 'regex':regexSignal
    },

    re.compile('QCD')    : {
    'name':'qcd', 'template':templateQCD, 'regex':regexQCD
    },

    re.compile('GJet')   : {
    'name':'gjet', 'template':templateQCD, 'regex':regexQCD
    },

    re.compile('DYJets') : {
    'name':'dy_jets', 'template':templateDY, 'regex':regexDY
    },

    re.compile('Box')    : {
    'name':'di_photon_box', 'template':templateDY, 'regex':regexDY
    },

    re.compile('DiPhotonJets') : {
    'name':'di_photon_jets', 'template':'di_photon_jets_pu2011_0'}, 
    }
    

if __name__ == '__main__':
    base = '/castor/cern.ch/user/c/cmshgg/reduced/h2gglobe_V11_04_05_reduction/MC_Bkg_Fall11_S6/'

    #gluglu_H_gg_%(mass)_pu2011_0 GluGluToHToGG_M-(?P<mass>.*)_7TeV
    print sampleString(base+'TTH_HToGG_M-120_7TeV')
    
    #vbf_H_gg_%(mass)_pu2011_0 VBF_HToGG_M-(?P<mass>.*)_7TeV
    #wz_H_gg_90_pu2011_0 WH_ZH_HToGG_M-(?P<mass>.*)
    #tt_H_gg_90_pu2011_0 TTH_HToGG_M-(?P<mass>.*)
    print sampleString(base+'GluGluToHToGG_M-95_7TeV')

    #qcd_ff_pu2011_30 QCDPt30to40_(?P<comb>.*)
    #qcd_ff_pu2011_40 QCDPt40_(?P<comb>.*)
    print sampleString(base+'QCDPt30to40_pf')
    print sampleString(base+'QCDPt40_pp')
    
    #gjet_ff_pu2011_20 GJet_Pt-20_(?P<comb>.*)
    print sampleString(base+'GJet_Pt-20_ff')

    #di_photon_box_pu2011_10 BoxPt10to25
    #di_photon_box_pu2011_25 BoxPt25to250
    #di_photon_box_pu2011_250 BoxPt250
    print sampleString(base+'BoxPt25to250')

    #dy_jets_pu2011_50 DYJetsToLL_M50
    print sampleString(base+'DYJetsToLL_M50')

    #di_photon_jets_pu2011_0 DiPhotonJets
    print sampleString(base+'DiPhotonJets')



