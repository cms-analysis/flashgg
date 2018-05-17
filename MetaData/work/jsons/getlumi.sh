fggManageSamples.py -C RunIIFall17-3_0_X getlumi "/DoubleMuon*/*/*" output=DoubleMuon_2017_30X_sofar_17May.json
fggManageSamples.py -C RunIIFall17-3_0_X getlumi "/DoubleEG*/*/*" output=DoubleEG_2017_30X_sofar_17May.json
fggManageSamples.py -C RunIIFall17-3_0_X getlumi "/SingleElectron*/*/*" output=SingleElectron_2017_30X_sofar_17May.json
compareJSON.py --sub Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt DoubleEG_2017_30X_sofar_17May.json > ToRun_DoubleEG_2017_303_17May.json
compareJSON.py --sub Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt DoubleMuon_2017_30X_sofar_17May.json > ToRun_DoubleMuon_2017_303_17May.json
compareJSON.py --sub Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt SingleElectron_2017_30X_sofar_17May.json > ToRun_SingleElectron_2017_303_17May.json

