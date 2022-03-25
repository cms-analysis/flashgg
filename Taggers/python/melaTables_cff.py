import FWCore.ParameterSet.Config as cms

melaTable = cms.EDProducer("MELAGenMatrixElementTableProducer",
        lheInfo = cms.VInputTag(cms.InputTag("externalLHEProducer"), cms.InputTag("source")),
        name = cms.string("MELA"),
        year = cms.int32(2018),
        sqrts = cms.double(13.0),
        mH = cms.double(125.0),
        #candVVmode = cms.string("undecayed"),
        decayVVmode = cms.int32(0),
        #melaSetup = cms.string("ZH_NLO"),
        mode = cms.string("VBF_NLO"),
        normalize = cms.bool(True),
        matrixElements = cms.vstring(),
        #   "Name:SM Couplings:ghz1=1,0;ghw1=1,0",
        #   "Name:SM2 Couplings:ghz1=2,0;ghw1=2,0",
        #   "Name:ghz2a Couplings:ghz1=1,0;ghw1=1,0;ghz2=0.01,0",
        #   "Name:ghz2b Couplings:ghz1=1,0;ghw1=1,0;ghz2=-0.01,0", 
        #   "Name:ghzgs2a Couplings:ghz1=1,0;ghw1=1,0;ghzgs2=0.01,0",
        #   "Name:ghzgs2b Couplings:ghz1=1,0;ghw1=1,0;ghzgs2=-0.01,0",
        #   "Name:ghw2a Couplings:ghz1=1,0;ghw1=1,0;ghw2=0.01,0",
        #   "Name:ghw2b Couplings:ghz1=1,0;ghw1=1,0;ghw2=-0.01,0", 
)


def addMelaTable_ACJHU(process):

    # =================================================================================================
    # ================================== ANOMALOUS COUPLINGS POINTS ===================================
    # =================================================================================================
    ### AC fa_i(s) ###
    JHU_points = ["Name:SM Couplings:ghz1=1,0;ghw1=1,0",
                  "Name:SM2 Couplings:ghz1=2,0;ghw1=2,0",
                  "Name:ghz2 Couplings:ghz1=1,0;ghw1=1,0;ghz2=1,0",
                  "Name:ghz2_up Couplings:ghz1=1,0;ghw1=1,0;ghz2=0.1,0",
                  "Name:ghz2_dn Couplings:ghz1=1,0;ghw1=1,0;ghz2=-0.1,0",
                  "Name:ghw2 Couplings:ghz1=1,0;ghw1=1,0;ghw2=1,0",
                  "Name:ghw2_up Couplings:ghz1=1,0;ghw1=1,0;ghw2=0.1,0",
                  "Name:ghw2_dn Couplings:ghz1=1,0;ghw1=1,0;ghw2=-0.1,0",
                  "Name:ghz4 Couplings:ghz1=1,0;ghw1=1,0;ghz4=1,0",
                  "Name:ghz4_up Couplings:ghz1=1,0;ghw1=1,0;ghz4=0.1,0",
                  "Name:ghz4_dn Couplings:ghz1=1,0;ghw1=1,0;ghz4=-0.1,0",
                  "Name:ghw4 Couplings:ghz1=1,0;ghw1=1,0;ghw4=1,0",
                  "Name:ghw4_up Couplings:ghz1=1,0;ghw1=1,0;ghw4=0.1,0",
                  "Name:ghw4_dn Couplings:ghz1=1,0;ghw1=1,0;ghw4=-0.1,0",
                  "Name:ghzgs2 Couplings:ghz1=1,0;ghw1=1,0;ghzgs2=1,0",
                  "Name:ghzgs2_up Couplings:ghz1=1,0;ghw1=1,0;ghzgs2=0.1,0",
                  "Name:ghzgs2_dn Couplings:ghz1=1,0;ghw1=1,0;ghzgs2=-0.1,0",
              ]
    
    process.melaGenMatrixElementACTableJHU = melaTable.clone(name = "MEWeight_prodJHUGen", matrixElements = JHU_points)
    process.tablesAC = cms.Sequence(process.melaGenMatrixElementACTableJHU)
    print "Anomalous couplings\n", "\n".join(JHU_points)


from MelaAnalytics.GenericMEComputer.couplingUtils import *

def addMelaTables_EFT(process,bases):
    # ==================================================================================
    # ================================== HIGGS BASIS ===================================
    # ==================================================================================

    Higgs_SCAN = byhand_hig_strings([
          ('SM',make_free_higlike(dict())), 
          ('cZA_up', make_free_higlike(dict(cZA=-0.1))),
          ('cZA_dn', make_free_higlike(dict(cZA=+0.1))),
          ('cAA_up', make_free_higlike(dict(cAA=-0.1))),
          ('cAA_dn', make_free_higlike(dict(cAA=+0.1))),
          ('cZZ_up', make_free_higlike(dict(cZZ=0.01))),
          ('cZZ_dn', make_free_higlike(dict(cZZ=-0.01))),
          ('cWW_up', make_free_higlike(dict(cWW=0.01))),
          ('cWW_dn', make_free_higlike(dict(cWW=-0.01))),
          #('cWBox_up', make_free_higlike(dict(cWBox=0.001))),
          #('cWBox_dn', make_free_higlike(dict(cWBox=-0.001))), ])
          ], FlipZG=True)

    if bases in ("any", "Higgs"):
        print "Higgs SCAN\n", "\n".join(Higgs_SCAN)

        process.melaGenMatrixElementHiggsTable = melaTable.clone(name = "MEWeight_Higgs", matrixElements = Higgs_SCAN)
        process.tablesHiggs = cms.Sequence(process.melaGenMatrixElementHiggsTable)

    # ==================================================================================
    # ================================== SILH HEL BASIS ================================
    # ==================================================================================
    HEL_points = [
         ('SM',hel_to_higgs(dict())), 
         ('H_up', hel_to_higgs(dict(cH=0.1))),
         ('H_dn', hel_to_higgs(dict(cH=-0.1))),
         ('WWmB_up', hel_to_higgs(dict(cWW=+0.01,cB=-0.01))),
         ('WWmB_dn', hel_to_higgs(dict(cWW=-0.01,cB=+0.01))),
         ('HW_up', hel_to_higgs(dict(cHW=+0.01))),
         ('HW_dn', hel_to_higgs(dict(cHW=-0.01))),
         ('HB_up', hel_to_higgs(dict(cHB=+0.01))),
         ('HB_dn', hel_to_higgs(dict(cHB=-0.01))),
         ('NoBox_up', hel_to_higgs(dict(cHW=+0.01,cHB=-0.01,cWW=-0.01,cB=+0.01))),
         ('NoBox_dn', hel_to_higgs(dict(cHW=-0.01,cHB=+0.01,cWW=+0.01,cB=-0.01))),
         ('A_up', hel_to_higgs(dict(cA=+0.01))),
         ('A_dn', hel_to_higgs(dict(cA=-0.01))),
         ('NoZA_up', hel_to_higgs(dict(cHW=+0.01,cHB=-0.01,cWW=-0.01,cB=+0.01,cA=-0.010703646430953565))),
         ('NoZA_dn', hel_to_higgs(dict(cHW=-0.01,cHB=+0.01,cWW=+0.01,cB=-0.01,cA=+0.010703646430953565))),
         ('NoZZ_up', hel_to_higgs(dict(cHW=+0.01,cHB=-0.01,cWW=-0.01,cB=+0.01,cA=+0.02441992590563888))),
         ('NoZZ_dn', hel_to_higgs(dict(cHW=-0.01,cHB=+0.01,cWW=+0.01,cB=-0.01,cA=-0.02441992590563888))),
        #('WWmBSmall_up', hel_to_higgs(dict(cWW=+0.001,cB=-0.001))),
        #('WWmBSmall_dn', hel_to_higgs(dict(cWW=-0.001,cB=+0.001))),
        #('HWSmall_up', hel_to_higgs(dict(cHW=+0.001))),
        #('HWSmall_dn', hel_to_higgs(dict(cHW=-0.001))),
        #('HBSmall_up', hel_to_higgs(dict(cHB=+0.001))),
        #('HBSmall_dn', hel_to_higgs(dict(cHB=-0.001))),
        #('NoBoxSmall_up', hel_to_higgs(dict(cHW=+0.001,cHB=-0.001,cWW=-0.001,cB=+0.001))),
        #('NoBoxSmall_dn', hel_to_higgs(dict(cHW=-0.001,cHB=+0.001,cWW=+0.001,cB=-0.001))),
        #('ASmall_up', hel_to_higgs(dict(cA=+0.001))),
        #('ASmall_dn', hel_to_higgs(dict(cA=-0.001))),
        #('NoZASmall_up', hel_to_higgs(dict(cHW=+0.001,cHB=-0.001,cWW=-0.001,cB=+0.001,cA=-0.0010703646430953565))),
        #('NoZASmall_dn', hel_to_higgs(dict(cHW=-0.001,cHB=+0.001,cWW=+0.001,cB=-0.001,cA=+0.0010703646430953565))),
        #('NoZZSmall_up', hel_to_higgs(dict(cHW=+0.001,cHB=-0.001,cWW=-0.001,cB=+0.001,cA=+0.002441992590563888))),
        #('NoZZSmall_dn', hel_to_higgs(dict(cHW=-0.001,cHB=+0.001,cWW=+0.001,cB=-0.001,cA=-0.002441992590563888))),
         ]

    #HEL_SCAN = byhand_hig_strings(HEL_points, FlipZG=False)
    #HELEff_SCAN = byhand_hig_strings(HEL_points, FlipZG=False, SMLoop=True)
    HELFlip_SCAN = byhand_hig_strings(HEL_points, FlipZG=True)
    HELFlipEff_SCAN = byhand_hig_strings(HEL_points, FlipZG=True, SMLoop=True)
    HELNoG_SCAN = byhand_hig_strings(HEL_points, noGamma=True)

    if bases in ("any", "HEL"):
        #print "HEL SCAN\n", "\n".join(HEL_SCAN)
        #print "HEL SCAN with Eff SM Loops\n", "\n".join(HELEff_SCAN)
        print "HEL SCAN flipped \n", "\n".join(HELFlip_SCAN)
        print "HEL SCAN flipped with Eff SM Loops\n", "\n".join(HELFlipEff_SCAN)
        print "HEL SCAN with no gamma\n", "\n".join(HELNoG_SCAN)

    #process.melaGenMatrixElementHELTable = melaTable.clone( name = "MEWeight_HEL", matrixElements = HEL_SCAN)
    #process.melaGenMatrixElementHELEffTable = melaTable.clone( name = "MEWeight_HELEff", matrixElements = HELEff_SCAN)
    process.melaGenMatrixElementHELFlipTable = melaTable.clone( name = "MEWeight_HELFlip", matrixElements = HELFlip_SCAN)
    process.melaGenMatrixElementHELFlipEffTable = melaTable.clone( name = "MEWeight_HELFlipEff", matrixElements = HELFlipEff_SCAN)
    process.melaGenMatrixElementHELNoGTable = melaTable.clone( name = "MEWeight_HELNoG", matrixElements = HELNoG_SCAN)
    process.tablesHEL = cms.Sequence(
        #  process.melaGenMatrixElementHELTable 
        #+ process.melaGenMatrixElementHELEffTable 
        process.melaGenMatrixElementHELFlipTable 
        + process.melaGenMatrixElementHELFlipEffTable 
        + process.melaGenMatrixElementHELNoGTable 
    )


    # ==================================================================================
    # ================================== SILH HELatNLO BASIS ===========================
    # ==================================================================================
    HELatNLO_points = [
        ('SM',(dict())), 
        ('HW_up', dict(cHW=+0.1)),
        ('HW_dn', dict(cHW=-0.1)),
        ('HB_up', dict(cHB=+0.1)),
        ('HB_dn', dict(cHB=-0.1)),
        ('B_up',  dict(cB=+0.1)),
        ('B_dn',  dict(cB=-0.1)),
        ('BB_up', dict(cBB=+0.1)),
        ('BB_dn', dict(cBB=-0.1)),
        ('WW_up', dict(cWW=+0.1)),
        ('WW_dn', dict(cWW=-0.1)),
        ('halfWWpB_up', dict(cWW=+0.05,cB=+0.1)),
        ('halfWWpB_dn', dict(cWW=-0.05,cB=-0.1)),
        ('halfWWmB_up', dict(cWW=+0.05,cB=-0.1)),
        ('halfWWmB_dn', dict(cWW=-0.05,cB=+0.1)),
    
        ('WWphalfB_up', dict(cWW=+0.1,cB=+0.05)), # no ABox (only ZBox, WBox)
        ('WWphalfB_dn', dict(cWW=-0.1,cB=-0.05)),
        ('WWmhalfB_up', dict(cWW=+0.1,cB=-0.05)),
        ('WWmhalfB_dn', dict(cWW=-0.1,cB=+0.05)),
    
        ('HW1_up', dict(cHW=+1.)),
        ('HW1_dn', dict(cHW=-1.)),
        ('HB1_up', dict(cHB=+1.)),
        ('HB1_dn', dict(cHB=-1.)),
        ('B1_up',  dict(cB=+1.)),
        ('B1_dn',  dict(cB=-1.)),
        ('BB1_up', dict(cBB=+1.)),
        ('BB1_dn', dict(cBB=-1.)),
        ('WW1_up', dict(cWW=+1.)),
        ('WW1_dn', dict(cWW=-1.)),
        ('WWphalfB1_up', dict(cWW=+1.,cB=+0.5)), # no ABox (only ZBox, WBox)
        ('WWphalfB1_dn', dict(cWW=-1.,cB=-0.5)),
        ('WWmhalfB1_up', dict(cWW=+1.,cB=-0.5)),
        ('WWmhalfB1_dn', dict(cWW=-1.,cB=+0.5)),
    
        ('HWmWW1_up', dict(cHW=+1,cWW=-1)),  # cWW w/o cWBox
        ('HWmWW1_dn', dict(cHW=-1,cWW=+1)),  # cWW w/o cWBox
    
        ('WWm1701B_up', dict(cWW=+1,cB=-1.701)),   # ABox-like (tiny cZBox)
        ('WWm1701B_dn', dict(cWW=-1,cB=+1.701)),
    
        ('WWm16057B_up', dict(cWW=+0.1,cB=-0.16057)),   # ABox-like (tiny cZBox)
        ('WWm16057B_dn', dict(cWW=-0.1,cB=+0.16057)),
        ('WWm16057B1_up', dict(cWW=+1,cB=-1.6057)),   # ABox-like (tiny cZBox)
        ('WWm16057B1_dn', dict(cWW=-1,cB=+1.6057)),
    ]

    HELatNLO_points_fixBox   = [ (k, helatnlo_to_higgs(v, fixBoxes=True )) for (k,v) in HELatNLO_points ]
    #HELatNLO_points_noFixBox = [ (k, helatnlo_to_higgs(v, fixBoxes=False)) for (k,v) in HELatNLO_points ]

    #HELatNLO_NoG_noFixBox_SCAN = byhand_hig_strings(HELatNLO_points_noFixBox, noGamma=True) # Bad for Wbox in WH
    HELatNLO_NoG_fixBox_SCAN = byhand_hig_strings(HELatNLO_points_fixBox, noGamma=True)
    #HELatNLO_NoFlip_fixBox_SCAN = byhand_hig_strings(HELatNLO_points_fixBox, FlipZG=False)
    HELatNLO_FlipZG_fixBox_SCAN = byhand_hig_strings(HELatNLO_points_fixBox, FlipZG=True)
    #HELatNLO_NoFlip_noFixBox_SCAN = byhand_hig_strings(HELatNLO_points_noFixBox, FlipZG=False)
    #HELatNLO_FlipZG_noFixBox_SCAN = byhand_hig_strings(HELatNLO_points_noFixBox, FlipZG=True)
    HELatNLO_FlipZGEff_fixBox_SCAN = byhand_hig_strings(HELatNLO_points_fixBox, FlipZG=True, SMLoop=True)
    #HEL_SCAN = byhand_hig_strings(HEL_points, FlipZG=False)
    #HELEff_SCAN = byhand_hig_strings(HEL_points, FlipZG=False, SMLoop=True)
    #HELFlip_SCAN = byhand_hig_strings(HEL_points, FlipZG=True)
    #HELFlipEff_SCAN = byhand_hig_strings(HEL_points, FlipZG=True, SMLoop=True)

    if bases in ("any", "HELatNLO"):
        #print "HELatNLO SCAN (Boxes as per YR4), no gamma\n", "\n".join(HELatNLO_NoG_noFixBox_SCAN)  # BAD for cWbox in WH
        print "HELatNLO SCAN (Boxes as per ArXiv), no gamma\n", "\n".join(HELatNLO_NoG_fixBox_SCAN)
        #print "HELatNLO SCAN (Boxes as per ArXiv), as is\n", "\n".join(HELatNLO_NoFlip_fixBox_SCAN)
        #print "HELatNLO SCAN (Boxes as per YR4), as is\n", "\n".join(HELatNLO_NoFlip_noFixBox_SCAN)
        print "HELatNLO SCAN (Boxes as per ArXiv), flip Zgamma\n", "\n".join(HELatNLO_FlipZG_fixBox_SCAN)
        #print "HELatNLO SCAN (Boxes as per YR4), flip Zgamma\n", "\n".join(HELatNLO_FlipZG_noFixBox_SCAN)
        print "HELatNLO SCAN (Boxes as per ArXiv), flip Zgamma, with Eff SM Loops\n", "\n".join(HELatNLO_FlipZGEff_fixBox_SCAN)
        #print "HELatNLO SCAN with no gamma\n", "\n".join(HELNoG_SCAN)


    process.melaGenMatrixElementHELatNLONoGFixTable   = melaTable.clone( name = "MEWeight_HELatNLO_NoG_Fix", matrixElements = HELatNLO_NoG_fixBox_SCAN)
    #process.melaGenMatrixElementHELatNLONoGNoFixTable = melaTable.clone( name = "MEWeight_HELatNLO_NoG_NoFix", matrixElements = HELatNLO_NoG_noFixBox_SCAN) # BAD for cWbox in WH
    #process.melaGenMatrixElementHELatNLONoFlipFixTable   = melaTable.clone( name = "MEWeight_HELatNLO_NoFlip_Fix", matrixElements = HELatNLO_NoFlip_fixBox_SCAN)
    process.melaGenMatrixElementHELatNLOFlipZGFixTable   = melaTable.clone( name = "MEWeight_HELatNLO_FlipZG_Fix", matrixElements = HELatNLO_FlipZG_fixBox_SCAN)
    #process.melaGenMatrixElementHELatNLONoFlipNoFixTable   = melaTable.clone( name = "MEWeight_HELatNLO_NoFlip_NoFix", matrixElements = HELatNLO_NoFlip_noFixBox_SCAN)
    #process.melaGenMatrixElementHELatNLOFlipZGNoFixTable   = melaTable.clone( name = "MEWeight_HELatNLO_FlipZG_NoFix", matrixElements = HELatNLO_FlipZG_noFixBox_SCAN)
    process.melaGenMatrixElementHELatNLOFlipZGEffFixTable   = melaTable.clone( name = "MEWeight_HELatNLO_FlipZGEff_Fix", matrixElements = HELatNLO_FlipZGEff_fixBox_SCAN)
    #process.melaGenMatrixElementHELatNLOEffTable = melaTable.clone( name = "MEWeight_HELEff", matrixElements = HELEff_SCAN)
    #process.melaGenMatrixElementHELatNLOFlipTable = melaTable.clone( name = "MEWeight_HELFlip", matrixElements = HELFlip_SCAN)
    #process.melaGenMatrixElementHELatNLOFlipEffTable = melaTable.clone( name = "MEWeight_HELFlipEff", matrixElements = HELFlipEff_SCAN)
    #process.melaGenMatrixElementHELatNLONoGTable = melaTable.clone( name = "MEWeight_HELNoG", matrixElements = HELNoG_SCAN)
    process.tablesHELatNLO = cms.Sequence(
        process.melaGenMatrixElementHELatNLONoGFixTable  +
        #process.melaGenMatrixElementHELatNLONoGNoFixTable +
        #process.melaGenMatrixElementHELatNLONoFlipFixTable +
        process.melaGenMatrixElementHELatNLOFlipZGFixTable +
        #process.melaGenMatrixElementHELatNLONoFlipNoFixTable +
        #process.melaGenMatrixElementHELatNLOFlipZGNoFixTable +
        process.melaGenMatrixElementHELatNLOFlipZGEffFixTable 
    )


    # ==================================================================================
    # ================================== WARSAW BASIS ==================================
    # ==================================================================================
    War_points = [
             ('SM',warsaw_smeftsim_MW_to_higgs(dict())), 
             ('HBox_up', warsaw_smeftsim_MW_to_higgs(dict(cHbox=+1))),
             ('HBox_dn', warsaw_smeftsim_MW_to_higgs(dict(cHbox=-1))),
             ('HWHB_up', warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.1,cHB=+0.1))),
             ('HWHB_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.1,cHB=-0.1))),
             ('HW_up', warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.1))),
             ('HW_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.1))),
             ('HB_up', warsaw_smeftsim_MW_to_higgs(dict(cHB=+0.1))),
             ('HB_dn', warsaw_smeftsim_MW_to_higgs(dict(cHB=-0.1))),
             ('NoZZ_up', warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.469333471898234, cHB=-1.6326252710332367))),
             ('NoZZ_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.469333471898234, cHB=+1.6326252710332367))),
             ('HWtil_up', warsaw_smeftsim_MW_to_higgs(dict(cHWtil=+1))),
             ('HWtil_dn', warsaw_smeftsim_MW_to_higgs(dict(cHWtil=-1))),
             ('HWtil_half', warsaw_smeftsim_MW_to_higgs(dict(cHWtil=+0.5))),
             ('HBtil_up', warsaw_smeftsim_MW_to_higgs(dict(cHBtil=+1))),
             ('HBtil_dn', warsaw_smeftsim_MW_to_higgs(dict(cHBtil=-1))),
             ('HBtil_half', warsaw_smeftsim_MW_to_higgs(dict(cHBtil=+0.5))),
             ('HWBtil_up', warsaw_smeftsim_MW_to_higgs(dict(cHWBtil=+1))),
             ('HWBtil_dn', warsaw_smeftsim_MW_to_higgs(dict(cHWBtil=-1))),
             ('HWBtil_half', warsaw_smeftsim_MW_to_higgs(dict(cHWBtil=+0.5))),
             ]
    War_points += [
             ('HWHBBig_up',warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.5,cHB=+0.5))),
             ('HWHBBig_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.5,cHB=-0.5))),
             ('HWBig_up', warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.5))),
             ('HWBig_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.5))),
             ('HBBig_up', warsaw_smeftsim_MW_to_higgs(dict(cHB=+0.5))),
             ('HBBig_dn', warsaw_smeftsim_MW_to_higgs(dict(cHB=-0.5))),
             ('HWHBSmall_up', warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.02,cHB=+0.02))),
             ('HWHBSmall_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.02,cHB=-0.02))),
             ('HWSmall_up', warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.02))),
             ('HWSmall_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.02))),
             ('HBSmall_up', warsaw_smeftsim_MW_to_higgs(dict(cHB=+0.02))),
             ('HBSmall_dn', warsaw_smeftsim_MW_to_higgs(dict(cHB=-0.02))),
             ('NoZZSmall_up', warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.2*0.469333471898234, cHB=-0.2*1.6326252710332367))),
             ('NoZZSmall_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.2*0.469333471898234, cHB=+0.2*1.6326252710332367))),
    ]

    #War_SCAN = byhand_hig_strings(War_points)
    #WarEff_SCAN = byhand_hig_strings(War_points, SMLoop=True)
    WarFlip_SCAN = byhand_hig_strings(War_points, FlipZG=True)
    WarFlipEff_SCAN = byhand_hig_strings(War_points, FlipZG=True, SMLoop=True)
    #WarFlipEffF1_SCAN = byhand_hig_strings(War_points, FlipZG=True, SMLoop="flipGG")
    #WarFlipEffF2_SCAN = byhand_hig_strings(War_points, FlipZG=True, SMLoop="flipZG")
    #WarFlipGG_SCAN = byhand_hig_strings(War_points, FlipZG=True, FlipGG=True)
    #WarFlipGGEff_SCAN = byhand_hig_strings(War_points, FlipZG=True, FlipGG=True, SMLoop=True)
    WarNoG_SCAN = byhand_hig_strings(War_points, noGamma=True)

    if bases in ("any", "Warsaw"):
        print "Warsaw SCAN\n", "\n".join(WarFlip_SCAN)
        print "Warsaw SCAN with Eff SM Loops\n", "\n".join(WarFlipEff_SCAN)
        #print "Warsaw SCAN (GG flip)\n", "\n".join(WarFlipGG_SCAN)
        #print "Warsaw SCAN (GG flip) with Eff SM Loops\n", "\n".join(WarFlipGGEff_SCAN)
        print "Warsaw SCAN with no gammas\n", "\n".join(WarNoG_SCAN)

    #process.melaGenMatrixElementWarTable = melaTable.clone( name = "MEWeight_War", matrixElements = War_SCAN)
    #process.melaGenMatrixElementWarEffTable = melaTable.clone( name = "MEWeight_WarEff", matrixElements = WarEff_SCAN)
    process.melaGenMatrixElementWarFlipTable = melaTable.clone( name = "MEWeight_WarFlip", matrixElements = WarFlip_SCAN)
    process.melaGenMatrixElementWarFlipEffTable = melaTable.clone( name = "MEWeight_WarFlipEff", matrixElements = WarFlipEff_SCAN)
    #process.melaGenMatrixElementWarFlipEffF1Table = melaTable.clone( name = "MEWeight_WarFlipEffF1", matrixElements = WarFlipEffF1_SCAN)
    #process.melaGenMatrixElementWarFlipEffF2Table = melaTable.clone( name = "MEWeight_WarFlipEffF2", matrixElements = WarFlipEffF2_SCAN)
    #process.melaGenMatrixElementWarFlipGGTable = melaTable.clone( name = "MEWeight_WarFlipGG", matrixElements = WarFlipGG_SCAN)
    #process.melaGenMatrixElementWarFlipGGEffTable = melaTable.clone( name = "MEWeight_WarFlipGGEff", matrixElements = WarFlipGGEff_SCAN)
    process.melaGenMatrixElementWarNoGTable = melaTable.clone( name = "MEWeight_WarNoG", matrixElements = WarNoG_SCAN)
    process.tablesWarsaw = cms.Sequence(
      #  process.melaGenMatrixElementWarTable
      #  process.melaGenMatrixElementWarEffTable +
        process.melaGenMatrixElementWarFlipTable 
      + process.melaGenMatrixElementWarFlipEffTable 
      #+ process.melaGenMatrixElementWarFlipEffF1Table 
      #+ process.melaGenMatrixElementWarFlipEffF2Table 
      #+ process.melaGenMatrixElementWarFlipGGTable 
      #+ process.melaGenMatrixElementWarFlipGGEffTable 
      + process.melaGenMatrixElementWarNoGTable 
    )


    # ==================================================================================
    # ================================== COMMON ========================================
    # ==================================================================================
    if bases == "ac":
        process.tables = cms.Sequence( process.tablesAC )
    elif bases == "Warsaw":
        process.tables = cms.Sequence( process.tablesWarsaw )
    elif bases == "Higgs":
        process.tables = cms.Sequence( process.tablesHiggs )
    elif bases == "HEL":
        process.tables = cms.Sequence( process.tablesHEL )
    elif bases == "HELatNLO":
        process.tables = cms.Sequence( process.tablesHELatNLO )
    elif bases == "any":
        process.tables = cms.Sequence( process.tablesWarsaw + process.tablesHiggs + process.tablesHELatNLO + process.tablesAC )
    else:
        raise RuntimeError("Unsupported bases %r" % bases)

    # # Decay
    # if "Decay" in melaTable.mode.value():
    #     process.higgsDecayTable = cms.EDProducer("HiggsDecayLevelVariablesProducer",
    #            genParticles = 'genParticles'
    #     )
    #     process.tables += process.higgsDecayTable
    #     #process.maxEvents.input = 20
    
def getMelaEFT_variables(bases):
    prefix  = "LHE_MEWeight"
    postfixes = ["up","dn"]

    higgs_vars = [
        "Higgs_cZA",
        "Higgs_cAA",
        "Higgs_cZZ",
        "Higgs_cWW",
    ]

    higgs_vars_all = [prefix + '_' + v + '_' + pf for v in higgs_vars for pf in postfixes]

    hel_vars = [
        "HELatNLO_NoG_Fix_HW",
        "HELatNLO_NoG_Fix_HB",
        "HELatNLO_NoG_Fix_B",
        "HELatNLO_NoG_Fix_BB",
        "HELatNLO_NoG_Fix_WW",
        "HELatNLO_NoG_Fix_halfWWpB",
        "HELatNLO_NoG_Fix_halfWWmB",
        "HELatNLO_NoG_Fix_WWphalfB",
        "HELatNLO_NoG_Fix_WWmhalfB",
        "HELatNLO_NoG_Fix_HW1",
        "HELatNLO_NoG_Fix_HB1",
        "HELatNLO_NoG_Fix_B1",
        "HELatNLO_NoG_Fix_BB1",
        "HELatNLO_NoG_Fix_WW1",
        "HELatNLO_NoG_Fix_WWphalfB1",
        "HELatNLO_NoG_Fix_WWmhalfB1",
        "HELatNLO_NoG_Fix_HWmWW1",
        "HELatNLO_NoG_Fix_WWm1701B",
        "HELatNLO_NoG_Fix_WWm16057B",
        "HELatNLO_NoG_Fix_WWm16057B1",
        "HELatNLO_FlipZG_Fix_HW",
        "HELatNLO_FlipZG_Fix_HB",
        "HELatNLO_FlipZG_Fix_B",
        "HELatNLO_FlipZG_Fix_BB",
        "HELatNLO_FlipZG_Fix_WW",
        "HELatNLO_FlipZG_Fix_halfWWpB",
        "HELatNLO_FlipZG_Fix_halfWWmB",
        "HELatNLO_FlipZG_Fix_WWphalfB",
        "HELatNLO_FlipZG_Fix_WWmhalfB",
        "HELatNLO_FlipZG_Fix_HW1",
        "HELatNLO_FlipZG_Fix_HB1",
        "HELatNLO_FlipZG_Fix_B1",
        "HELatNLO_FlipZG_Fix_BB1",
        "HELatNLO_FlipZG_Fix_WW1",
        "HELatNLO_FlipZG_Fix_WWphalfB1",
        "HELatNLO_FlipZG_Fix_WWmhalfB1",
        "HELatNLO_FlipZG_Fix_HWmWW1",
        "HELatNLO_FlipZG_Fix_WWm1701B",
        "HELatNLO_FlipZG_Fix_WWm16057B",
        "HELatNLO_FlipZG_Fix_WWm16057B1",
        "HELatNLO_FlipZGEff_Fix_HW",
        "HELatNLO_FlipZGEff_Fix_HB",
        "HELatNLO_FlipZGEff_Fix_B",
        "HELatNLO_FlipZGEff_Fix_BB",
        "HELatNLO_FlipZGEff_Fix_WW",
        "HELatNLO_FlipZGEff_Fix_halfWWpB",
        "HELatNLO_FlipZGEff_Fix_halfWWmB",
        "HELatNLO_FlipZGEff_Fix_WWphalfB",
        "HELatNLO_FlipZGEff_Fix_WWmhalfB",
        "HELatNLO_FlipZGEff_Fix_HW1",
        "HELatNLO_FlipZGEff_Fix_HB1",
        "HELatNLO_FlipZGEff_Fix_B1",
        "HELatNLO_FlipZGEff_Fix_BB1",
        "HELatNLO_FlipZGEff_Fix_WW1",
        "HELatNLO_FlipZGEff_Fix_WWphalfB1",
        "HELatNLO_FlipZGEff_Fix_WWmhalfB1",
        "HELatNLO_FlipZGEff_Fix_HWmWW1",
        "HELatNLO_FlipZGEff_Fix_WWm1701B",
        "HELatNLO_FlipZGEff_Fix_WWm16057B",
        "HELatNLO_FlipZGEff_Fix_WWm16057B1",
    ]

    hel_vars_all = [prefix + '_' + v + '_' + pf for v in hel_vars for pf in postfixes]

    war_vars = [
        "WarFlip_HBox",
        "WarFlip_HWHB",
        "WarFlip_HW",
        "WarFlip_HB",
        "WarFlip_NoZZ",
        "WarFlip_HWtil",
        "WarFlip_HBtil",
        "WarFlip_HWBtil",
        "WarFlip_HWHBBig",
        "WarFlip_HWBig",
        "WarFlip_HBBig",
        "WarFlip_HWHBSmall",
        "WarFlip_HWSmall",
        "WarFlip_HBSmall",
        "WarFlip_NoZZSmall",
        "WarFlipEff_HBox",
        "WarFlipEff_HWHB",
        "WarFlipEff_HW",
        "WarFlipEff_HB",
        "WarFlipEff_NoZZ",
        "WarFlipEff_HWtil",
        "WarFlipEff_HBtil",
        "WarFlipEff_HWBtil",
        "WarFlipEff_HWHBBig",
        "WarFlipEff_HWBig",
        "WarFlipEff_HBBig",
        "WarFlipEff_HWHBSmall",
        "WarFlipEff_HWSmall",
        "WarFlipEff_HBSmall",
        "WarFlipEff_NoZZSmall",
        "WarNoG_HBox",
        "WarNoG_HWHB",
        "WarNoG_HW",
        "WarNoG_HB",
        "WarNoG_NoZZ",
        "WarNoG_HWtil",
        "WarNoG_HBtil",
        "WarNoG_HWBtil",
        "WarNoG_HWHBBig",
        "WarNoG_HWBig",
        "WarNoG_HBBig",
        "WarNoG_HWHBSmall",
        "WarNoG_HWSmall",
        "WarNoG_HBSmall",
        "WarNoG_NoZZSmall",
    ]

    war_vars_all = [prefix + '_' + v + '_' + pf for v in war_vars for pf in postfixes]

    jhu_vars = [
        "prodJHUGen_ghz2",
        "prodJHUGen_ghw2",
        "prodJHUGen_ghz4",
        "prodJHUGen_ghw4",
        "prodJHUGen_ghzgs2",
    ]

    postfixes_jhu = ["_up","_dn",""]
    jhu_vars_all = [prefix+"prodJHUGen_SM2"] + [prefix + '_' + v + pf for v in jhu_vars for pf in postfixes_jhu]

    if bases == "ac":
        return jhu_vars_all
    elif bases == "Warsaw":
        return war_vars_all
    elif bases == "Higgs":
        return higgs_vars_all
    elif bases == "HEL":
        return hel_vars_all
    elif bases == "any":
        return jhu_vars_all + war_vars_all + higgs_vars_all + hel_vars_all 
    else:
        raise RuntimeError("Unsupported bases %r" % bases)

