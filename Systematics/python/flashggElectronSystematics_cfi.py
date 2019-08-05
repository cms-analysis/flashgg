import FWCore.ParameterSet.Config as cms
import os, json, re
from collections import OrderedDict

class EleSF_JSONReader :
    def __init__(self , file_name , sf_name ):
        self.Name = sf_name
        self.JSONFileName = os.path.expanduser( os.path.expandvars(str('$CMSSW_BASE/src/')+file_name) )
        with open( self.JSONFileName , 'r' ) as f :
            j = json.load( f ,object_pairs_hook=OrderedDict)
            self.Info = j[ sf_name ]
        self.binInfo = cms.PSet(
            variables = cms.vstring( "eta" , "pt" ),
            bins = cms.VPSet()
            )
        format_bins = r'^(?P<VarName>.*):\[(?P<From>-?\d*.\d*),(?P<To>-?\d*.\d*)\]'
        for pt_region in self.Info :
            pt_values = re.match( format_bins , pt_region , re.M|re.I )
            if pt_values and len( pt_values.groups() ) == 3 :
                pt_from = float( pt_values.group("From") )
                pt_to = float( pt_values.group("To") )

                for eta_region in self.Info[pt_region]:
                    eta_values = re.match( format_bins , eta_region , re.M|re.I )
                    sf_value = self.Info[pt_region][eta_region]["value"]
                    sf_error = self.Info[pt_region][eta_region]["error"]
                    eta_from = float( eta_values.group("From") )
                    eta_to = float( eta_values.group("To") )
                    
                self.binInfo.bins.append(
                    cms.PSet(
                        lowBounds = cms.vdouble( eta_from , pt_from ) ,
                        upBounds = cms.vdouble( eta_to , pt_to ) ,
                        values = cms.vdouble( sf_value ) ,
                        uncertainties = cms.vdouble( sf_error )
                    )
                )
         
            else :
                print pt_region, "of sf" , sf_name , "from file", file_name , "can not be interpreted correctly"



flashggElectronSystematics = cms.EDProducer('FlashggElectronEffSystematicProducer',
                                            src = cms.InputTag("flashggSelectedElectrons"),
                                            SystMethods2D = cms.VPSet(),
                                            SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggElectronWeight"),
                                                                              Label = cms.string("ElectronWeight"),
                                                                              NSigmas = cms.vint32(-1,1),
                                                                              OverallRange = cms.string("abs(eta)<2.5"),
                                                                              BinList = binInfo2016,
                                                                              Debug = cms.untracked.bool(False),
                                                                              ApplyCentralValue = cms.bool(True)
                                                                              )	
                                                                    )
                                            )
