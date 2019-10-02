import FWCore.ParameterSet.Config as cms

import os, json, re, math

class MuonSF_JSONReader :
    def __init__(self , file_name , sf_name , lowPt_file_name="" , lowPt_sf_name="", extendPt=0 ):
        self.Name = sf_name
        self.JSONFileName = os.path.expanduser( os.path.expandvars(str('$CMSSW_BASE/src/')+file_name) )

        with open( self.JSONFileName , 'r' ) as f :
            j = json.load( f ) 
            self.Info = j[ sf_name ]

        self.binInfo = cms.PSet( 
            variables = cms.vstring( "eta" , "pt" ),
            bins = cms.VPSet() 
            )

        sub_branch_name = self.Info.keys()[0]

        #First loop to get min pT
        format_bins = r'^(?P<VarName>.*):\[(?P<From>-?\d*.\d*),(?P<To>-?\d*.\d*)\]'
        minPt = 1000
        for eta_region in self.Info[sub_branch_name] :
            for pt_region in self.Info[sub_branch_name][eta_region] :
                pt_values = re.match( format_bins , pt_region , re.M|re.I )
                if pt_values and len( pt_values.groups() ) == 3 :
                    pt_from = float( pt_values.group("From" ) )
                    if pt_from < minPt :
                        minPt = pt_from
            
        if lowPt_file_name!="" and lowPt_sf_name!="NUM_HighPtID_DEN_genTracks" and lowPt_sf_name!="NUM_MediumPromptID_DEN_genTracks" and lowPt_sf_name!="NUM_TrkHighPtID_DEN_genTracks" :
            self.JSONFileNameForLowPt = os.path.expanduser( os.path.expandvars(str('$CMSSW_BASE/src/')+lowPt_file_name) )
            with open( self.JSONFileNameForLowPt , 'r' ) as f_lowPt :
                j_lowPt = json.load( f_lowPt ) 
                self.Info_lowPt = j_lowPt[ lowPt_sf_name ]
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
                    pt_values = re.match( format_bins , pt_region , re.M|re.I )
                    if pt_values and len( pt_values.groups() ) == 3 :
                        pt_from = float( pt_values.group("From" ) )
                        pt_to = float( pt_values.group("To" ) )
                        if pt_from == minPt :
                            minPtBin = pt_region


                for pt_region in self.Info[sub_branch_name][eta_region] :
                    pt_values = re.match( format_bins , pt_region , re.M|re.I )
                    if pt_values and len( pt_values.groups() ) == 3 :
                        pt_from = float( pt_values.group("From" ) )
                        pt_to = float( pt_values.group("To" ) )
                        sf_value = self.Info[sub_branch_name][eta_region][pt_region]["value"]
                        if "stat" in self.Info[sub_branch_name][eta_region][pt_region] : 
                            sf_stat = self.Info[sub_branch_name][eta_region][pt_region]["stat"]
                            sf_syst = self.Info[sub_branch_name][eta_region][pt_region]["syst"]
                        else :
                            sf_stat = self.Info[sub_branch_name][eta_region][pt_region]["error"]
                            sf_syst = 0

                        sf_error = math.sqrt(sf_stat*sf_stat + sf_syst*sf_syst)

                        self.binInfo.bins.append( 
                            cms.PSet(
                                lowBounds = cms.vdouble( eta_from , pt_from ) ,
                                upBounds = cms.vdouble( eta_to , pt_to ) ,
                                values = cms.vdouble( sf_value ) ,
                                uncertainties = cms.vdouble( sf_error , sf_error )
                                )
                            )

                        if not file_name.count("2016") : 
                            self.binInfo.bins.append( 
                                cms.PSet(
                                    lowBounds = cms.vdouble( -1*eta_to , pt_from ) ,
                                    upBounds = cms.vdouble( -1*eta_from , pt_to ) ,
                                    values = cms.vdouble( sf_value ) ,
                                    uncertainties = cms.vdouble( sf_error , sf_error )
                                ) 
                            )
    
                        if not file_name.count("2016") :
                            pt_bins[ pt_from ] = [ self.binInfo.bins[-1], self.binInfo.bins[-2] ]
                        else :
                            pt_bins[ pt_from ] =  self.binInfo.bins[-1]
                    else :
                        print pt_region, "of sf" , sf_name , "from file", file_name , "can not be interpreted correctly"
                min_pt = sorted( pt_bins.keys() )[0]
                if extendPt!=0 and min_pt>extendPt: #create one bin from extendPt to min_pt with next bin SF and doubled uncertainty
                    minSF = self.Info[sub_branch_name][eta_region][minPtBin]["value"]
                    if "stat" in self.Info[sub_branch_name][eta_region][minPtBin] :
                        minStat = self.Info[sub_branch_name][eta_region][minPtBin]["stat"]
                        minSyst = self.Info[sub_branch_name][eta_region][minPtBin]["syst"]
                    else :
                        minStat = self.Info[sub_branch_name][eta_region][minPtBin]["error"]
                        minSyst = 0
                    minErr = math.sqrt(minSyst*minSyst +minStat*minStat )
                    self.binInfo.bins.append( 
                        cms.PSet(
                            lowBounds = cms.vdouble( eta_from , 0.00 ) ,
                            upBounds = cms.vdouble( eta_to , extendPt ) ,
                            values = cms.vdouble( 1.00 ) ,
                            uncertainties = cms.vdouble( 0.00 , 0.00 )
                            )
                        )
                    if not file_name.count("2016") :
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
                    if not file_name.count("2016") :
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
                    if not file_name.count("2016") :
                        self.binInfo.bins.append( 
                            cms.PSet(
                                lowBounds = cms.vdouble( -1*eta_to , 0.00 ) ,
                                upBounds = cms.vdouble( -1*eta_from , min_pt ) ,
                                values = cms.vdouble( 1.00 ) ,
                                uncertainties = cms.vdouble( 0.00 , 0.00 )
                                )
                             )

                if not file_name.count("2016") : 
                    pt_bins[ sorted(pt_bins.keys())[-1] ][0].upBounds[1] = float( 'inf' )
                    pt_bins[ sorted(pt_bins.keys())[-1] ][1].upBounds[1] = float( 'inf' )                
                else:
                    pt_bins[ sorted(pt_bins.keys())[-1] ].upBounds[1] = float( 'inf' )
                
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


flashggMuonSystematics = cms.EDProducer('FlashggMuonEffSystematicProducer',
					src = cms.InputTag("flashggSelectedMuons"),
					SystMethods2D = cms.VPSet(),
					SystMethods = cms.VPSet()
                                        )

import exceptions
def SetupMuonScaleFactors( process , id_file_name, id_lowpt_file_name, iso_file_name, id_name , iso_name , id_ref_tracks, id_lowpt_ref_tracks):

    minAnaPt = 5
    extendPtValID = 0
    extendPtValISO = minAnaPt

    if id_file_name.count("2016") :
        extendPtValID = minAnaPt 

    MUON_ID_ScaleFactors = {}
    for mu_id in ["Tight", "Medium" , "Loose", "HighPt"] :
        MUON_ID_ScaleFactors[mu_id] = MuonSF_JSONReader( id_file_name ,  "NUM_%sID_DEN_%s"%(mu_id,id_ref_tracks) , id_lowpt_file_name, "NUM_%sID_DEN_%s"%(mu_id,id_lowpt_ref_tracks), extendPtValID )
    MUON_ISO_ScaleFactors = {}
    for iso in ["LooseRelIso_DEN_LooseID", "LooseRelIso_DEN_MediumID", "TightRelIso_DEN_MediumID", "LooseRelIso_DEN_TightIDandIPCut", "TightRelIso_DEN_TightIDandIPCut", "LooseRelTkIso_DEN_HighPtIDandIPCut"] :
        MUON_ISO_ScaleFactors[ iso ] = MuonSF_JSONReader( iso_file_name , "NUM_" + iso , "", "", extendPtValISO)

    if id_name in MUON_ID_ScaleFactors.keys():
        process.flashggMuonSystematics.SystMethods.append( MUON_ID_ScaleFactors[id_name].GetPSet(str("Muon" + id_name + "IDWeight")) )
    else :
        raise NameError("%s id for muon is not valid" % id_name )

    Extention = ""
    if id_name in ["Tight" , "HighPt"]:
        Extention = "andIPCut"
    iso_full_name = "%sIso_DEN_%sID%s" % (iso_name , id_name , Extention)
    if iso_full_name in MUON_ISO_ScaleFactors.keys() :
        process.flashggMuonSystematics.SystMethods.append( MUON_ISO_ScaleFactors[iso_full_name].GetPSet(str("Muon" + iso_name  + "ISOWeight")) )
    else :
        raise NameError("%s iso for muon is not valid" % iso_full_name )

        
            
