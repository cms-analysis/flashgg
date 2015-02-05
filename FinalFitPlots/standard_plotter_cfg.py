import ROOT
# -- Parameters ---------------------------------------------------
current_lumi    = 1091.89
scale 		= 100.
file 		= 'Histos_mgg.root'
plottingdir     = 'plots'
data_name	= 'Data'
file_names	= {
		   'QCD30PF'	:[3,1.3]
		  ,'QCD30FF'	:[4,1.]
		  ,'QCD40PF'	:[3,1.3]
		  ,'QCD40FF'	:[4,1.]
		  ,'GJetPF'	:[2,1.3]
		  ,'Box25'	:[1,1.3]
		  ,'DiPhotonJets'	:[0,1.13]
		  ,'DyeeMG'	:[5,1.]
		  ,'GluGlu120'	:[6,1.*scale]
		  }

#print "Using files/type/weight :"
#for k in file_names.keys(): print k, file_names[k] 

# should have one color and title per Channel
titles_colors = {
		'4':['Fake-Fake (QCD)',ROOT.kRed+1				]
		,'3':['Prompt-Fake (QCD)',ROOT.kOrange-3		]
		,'2':['Prompt-Fake (GJet)',ROOT.kOrange-2		]
	 	,'0':['Prompt-Prompt (Madgraph Born+Brem)',ROOT.kGreen-3			]
	 	,'1':['Prompt-Prompt (Pythia Box)',ROOT.kGreen-2			]
		,'5':['DYee (Madgraph)',ROOT.kOrange+2	]
		,'6':['Higgs M-120 x100',ROOT.kBlue-2		]
	 	}

# -- Singal Index indicates which components are signal. -------------------
# -- These wont be included in normalisation		 -------------------
signal_index = [6]
# --------------------------------------------------------------------------

