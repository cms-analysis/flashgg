import FWCore.ParameterSet.Config as cms

import os, json, re, math

class MuonSF_JSONReader :
    def __init__(self , file_name , sf_name , lowPt_file_name="" , extendPt=0 ):
        self.Name = sf_name
        self.JSONFileName = os.path.expanduser( os.path.expandvars(file_name) )
        with open( self.JSONFileName , 'r' ) as f :
            j = json.load( f ) 
            self.Info = j[ sf_name ]
        self.binInfo = cms.PSet( 
            variables = cms.vstring( "eta" , "pt" ),
            bins = cms.VPSet() 
            )

        sub_branch_name = self.Info.keys()[0]

        #First loop to get min pT
        format_bins = r'^(?P<VarName>.*):\[(?P<From>\d*.\d*),(?P<To>\d*.\d*)\]'

        minPt = 1000
        for eta_region in self.Info[sub_branch_name] :
            for pt_region in self.Info[sub_branch_name][eta_region] :
                pt_values = re.match( format_bins , pt_region , re.M|re.I )
                if pt_values and len( pt_values.groups() ) == 3 :
                    pt_from = float( pt_values.group("From" ) )
                    if pt_from < minPt :
                        minPt = pt_from

        if lowPt_file_name!="" and sf_name!="NUM_HighPtID_DEN_genTracks" and sf_name!="NUM_MediumPromptID_DEN_genTracks" and sf_name!="NUM_TrkHighPtID_DEN_genTracks" :
            self.JSONFileNameForLowPt = os.path.expanduser( os.path.expandvars(lowPt_file_name) )
            with open( self.JSONFileNameForLowPt , 'r' ) as f_lowPt :
                j_lowPt = json.load( f_lowPt ) 
                self.Info_lowPt = j_lowPt[ sf_name ]
                for eta_region in self.Info_lowPt[sub_branch_name] :
                    for pt_region in self.Info_lowPt[sub_branch_name][eta_region] :
                        pt_values = re.match( format_bins , pt_region , re.M|re.I )
                        if pt_values and len( pt_values.groups() ) == 3 :
                            pt_to = float( pt_values.group("To" ) )
                            if pt_to <= minPt:
                                self.Info[sub_branch_name][eta_region][pt_region] = self.Info_lowPt[sub_branch_name][eta_region][pt_region]

        for eta_region in self.Info[sub_branch_name] :
            eta_values = re.match( format_bins , eta_region , re.M|re.I )
            if eta_values and len( eta_values.groups() ) == 3 :
                eta_from = float( eta_values.group("From") )
                eta_to = float( eta_values.group("To") )

                pt_bins = {}
                minPtBin = 0

                for pt_region in self.Info[sub_branch_name][eta_region] :
                    if pt_region.count("[" + str(minPt)) :
                        minPtBin = pt_region
                    pt_values = re.match( format_bins , pt_region , re.M|re.I )
                    if pt_values and len( pt_values.groups() ) == 3 :
                        pt_from = float( pt_values.group("From" ) )
                        pt_to = float( pt_values.group("To" ) )

                        sf_value = self.Info[sub_branch_name][eta_region][pt_region]["value"]
                        sf_stat = self.Info[sub_branch_name][eta_region][pt_region]["stat"]
                        sf_syst = self.Info[sub_branch_name][eta_region][pt_region]["syst"]

                        sf_error = math.sqrt(sf_stat*sf_stat + sf_syst*sf_syst)
                        self.binInfo.bins.append( 
                            cms.PSet(
                                lowBounds = cms.vdouble( eta_from , pt_from ) ,
                                upBounds = cms.vdouble( eta_to , pt_to ) ,
                                values = cms.vdouble( sf_value ) ,
                                uncertainties = cms.vdouble( sf_error , sf_error )
                                )
                            )
                        self.binInfo.bins.append( 
                            cms.PSet(
                                lowBounds = cms.vdouble( -1*eta_to , pt_from ) ,
                                upBounds = cms.vdouble( -1*eta_from , pt_to ) ,
                                values = cms.vdouble( sf_value ) ,
                                uncertainties = cms.vdouble( sf_error , sf_error )
                                )
                            )

                        pt_bins[ pt_from ] = [ self.binInfo.bins[-1], self.binInfo.bins[-2] ]
                    else :
                        print pt_region, "of sf" , sf_name , "from file", file_name , "can not be interpreted correctly"
                min_pt = sorted( pt_bins.keys() )[0]

                if extendPt!=0 and min_pt>extendPt: #create one bin from extendPt to min_pt with next bin SF and doubled uncertainty

                    minSF = self.Info[sub_branch_name][eta_region][minPtBin]["value"]
                    minStat = self.Info[sub_branch_name][eta_region][minPtBin]["stat"]
                    minSyst = self.Info[sub_branch_name][eta_region][minPtBin]["syst"]
                    minErr = math.sqrt(minSyst*minSyst +minStat*minStat )

                    self.binInfo.bins.append( 
                        cms.PSet(
                            lowBounds = cms.vdouble( eta_from , 0.00 ) ,
                            upBounds = cms.vdouble( eta_to , extendPt ) ,
                            values = cms.vdouble( 1.00 ) ,
                            uncertainties = cms.vdouble( 0.00 , 0.00 )
                            )
                        )
                    self.binInfo.bins.append( 
                        cms.PSet(
                            lowBounds = cms.vdouble( -1*eta_to , 0.00 ) ,
                            upBounds = cms.vdouble( -1*eta_from , extendPt ) ,
                            values = cms.vdouble( 1.00 ) ,
                            uncertainties = cms.vdouble( 0.00 , 0.00 )
                            )
                        )
                    self.binInfo.bins.append( 
                        cms.PSet(
                            lowBounds = cms.vdouble( eta_from , extendPt ) ,
                            upBounds = cms.vdouble( eta_to , min_pt ) ,
                            values = cms.vdouble( minSF ) ,
                            uncertainties = cms.vdouble( 2*minErr , 2*minErr )
                            )
                        )
                    self.binInfo.bins.append( 
                        cms.PSet(
                            lowBounds = cms.vdouble( -1*eta_to , extendPt ) ,
                            upBounds = cms.vdouble( -1*eta_from , min_pt ) ,
                            values = cms.vdouble( minSF ) ,
                            uncertainties = cms.vdouble( 2*minErr , 2*minErr )
                            )
                        )
                else :
                    self.binInfo.bins.append( 
                        cms.PSet(
                            lowBounds = cms.vdouble( eta_from , 0.00 ) ,
                            upBounds = cms.vdouble( eta_to , min_pt ) ,
                            values = cms.vdouble( 1.00 ) ,
                            uncertainties = cms.vdouble( 0.00 , 0.00 )
                            )
                        )
                    self.binInfo.bins.append( 
                        cms.PSet(
                            lowBounds = cms.vdouble( -1*eta_to , 0.00 ) ,
                            upBounds = cms.vdouble( -1*eta_from , min_pt ) ,
                            values = cms.vdouble( 1.00 ) ,
                            uncertainties = cms.vdouble( 0.00 , 0.00 )
                            )
                        )

                pt_bins[ sorted(pt_bins.keys())[-1] ][0].upBounds[1] = float( 'inf' )
                pt_bins[ sorted(pt_bins.keys())[-1] ][1].upBounds[1] = float( 'inf' )
            else:
                print eta_region, "of sf" , sf_name , "from file", file_name , "can not be interpreted correctly"

    def GetPSet(self , name = None , applyCentralValue = True):
        if not name :
            name = self.Name
        self.SystematicsPSET = cms.PSet( MethodName = cms.string("FlashggMuonWeight"),
                                         Label = cms.string(name),
                                         NSigmas = cms.vint32(-1,1),
                                         OverallRange = cms.string("abs(eta)<2.4"),
                                         BinList = self.binInfo,
                                         Debug = cms.untracked.bool(False),
                                         ApplyCentralValue = cms.bool(applyCentralValue)
                                         )
        return self.SystematicsPSET

MUON_ID_JSON_FileName = "$CMSSW_BASE/src/flashgg/Systematics/data/Muon_RunBCDEF_SF_ID.json"
MUON_ID_JSON_FileName_lowPt = "$CMSSW_BASE/src/flashgg/Systematics/data//Muon_RunBCDEF_SF_ID_lowPt.json"
MUON_ID_ScaleFactors = {}
for mu_id in ["Tight", "Medium" , "Loose", "Soft", "HighPt", "MediumPrompt", "TrkHighPt"] :
    MUON_ID_ScaleFactors[mu_id] = MuonSF_JSONReader( MUON_ID_JSON_FileName ,  "NUM_%sID_DEN_genTracks"%mu_id , MUON_ID_JSON_FileName_lowPt )
MUON_ISO_JSON_FileName = "$CMSSW_BASE/src/flashgg/Systematics/data/Muon_RunBCDEF_SF_ISO.json"
MUON_ISO_ScaleFactors = {}
for iso in ["LooseRelIso_DEN_LooseID", "LooseRelIso_DEN_MediumID", "TightRelIso_DEN_MediumID", "LooseRelTkIso_DEN_TrkHighPtID", "TightRelTkIso_DEN_TrkHighPtID", "LooseRelIso_DEN_TightIDandIPCut", "TightRelIso_DEN_TightIDandIPCut", "LooseRelTkIso_DEN_HighPtIDandIPCut", "TightRelTkIso_DEN_HighPtIDandIPCut"] :
    MUON_ISO_ScaleFactors[ iso ] = MuonSF_JSONReader( MUON_ISO_JSON_FileName , "NUM_" + iso , "", 10)

flashggMuonSystematics = cms.EDProducer('FlashggMuonEffSystematicProducer',
					src = cms.InputTag("flashggSelectedMuons"),
					SystMethods2D = cms.VPSet(),
					SystMethods = cms.VPSet()
                                        )

import exceptions
def SetupMuonScaleFactors( process , id_name , iso_name ):


    if os.environ["CMSSW_VERSION"].count("CMSSW_9_4"):
        if id_name in MUON_ID_ScaleFactors.keys():
            process.flashggMuonSystematics.SystMethods.append( MUON_ID_ScaleFactors[id_name].GetPSet("Muon" + id_name + "IDWeight") )
        else :
            raise NameError("%s id for muon is not valid" % id_name )

        Extention = ""
        if id_name in ["Tight" , "HighPt"]:
            Extention = "andIPCut"
        iso_full_name = "%sIso_DEN_%sID%s" % (iso_name , id_name , Extention)
        if iso_full_name in MUON_ISO_ScaleFactors.keys() :
            process.flashggMuonSystematics.SystMethods.append( MUON_ISO_ScaleFactors[iso_full_name].GetPSet("Muon" + iso_name  + "ISOWeight") )
        else :
            raise NameError("%s iso for muon is not valid" % iso_full_name )

    elif os.environ["CMSSW_VERSION"].count("CMSSW_8_0"):
        import flashgg.Systematics.flashggMuonSystematics2016_cfi as muons2016
        process.flashggMuonSystematics.SystMethods.append( muons2016.PSetMuonWeight2016 )
        process.flashggMuonSystematics.SystMethods.append( muons2016.PSetMuonMiniIsoWeight2016 )
        
            
