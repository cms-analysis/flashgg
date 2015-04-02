#flashgg/Taggers/MVATraining README

## Quickstart

For the impatient user...

```
export WORKSPACE=${CMSSW_BASE}/src/flashgg/Taggers/test/MVATraining
cd $WORKSPACE
cmsenv
. runAllVBFMVARetrain.sh 1000
```

Details below...

## Introduction

This area has been put together to retrain the MVA relevant to the VBF Tag, although the workflow, scripts, configs and plugins may be amended for other MVAs.

The basic workflow followed for each MVA is : 
* Use Dumpers to generate trees containing the variables which are used as inputs. In general, Flashgg contains a data format for each MVA, with data members the input variables and output, e.g. `flashgg::VBFMVAResult`.
* Create such trees for both signal and background samples.
* Retrain MVA and create new version of object which uses the new weights file.
* Create new Trees using Dumpers for both old and new versions of object, and create comparative ROC cruves.

## VBF MVA Structure and Retraining Strategy

The VBF Tag relies on 3 MVAs:

* VBF DiJet MVA (aka VBFMVA), which determines if an event is VBF-like (ie has a valid dijet).
* DiPhoton MVA, which determines if the event has a valid Hgg-like diphoton.
* VBF DiPhoton-DiJet MVA (aka VBFDiPhoDiJetMVA), which takes the inputs from the two above, and classifies events as VBF-like or not.

The final MVA is used to generate the VBF Tag categories. This folder so far deals with reetraining the VBF DiJet MVA and the VBF DiPhoton-DiJet MVA only.

The startegy for retaining the two is :

1) Generate Training Trees for sig (VBF) and bkg (gamJets):
VBFMVANew (e.g. trees of all input vars + hist of MVA output using dummy weights [not used at this stage])

2) Retrain VBFMVA -> Generate new VBFMVA weight file.

3) Generate Training Trees for sig (VBF) and bkg (gamJets):
VBFDiPhoDiJetMVANew (e.g. trees of inout vars (old dipho MVA weights, new VBFMVA wights) + hist of MVA output using dummy weights [not used at this stage])

4) Retrain VBFDiPhoDiJetMVA -> Generate new VBFDiPhoDiJet weights file

5) Compare Trees  for sig (VBF) and bkg (gamJets):
VBFDiPhoDiJetMVA( e.g. trees of all input vars (legacy dipho MVA weights, old dijet MVA weights) + hist of MVA output using legacy weights)
VBFDiPhoDiJetMVANew( e.g. trees of all input vars (legacy dipho MVA weights, new dijet MVA weights)+ hist of MVA output using new weights)
-> Generate ROC curves.

## JSON, Configs, Macros, and Scripts (plus Usage)

Before you start, please do:
```
export WORKSPACE=${CMSSW_BASE}/src/flashgg/Taggers/test/MVATraining
cd $WORKSPACE
cmsenv
```

### JSON

A JSON file is provided containing the samples of interest in this *basic* retraining. This will obviously become more complicated in future.
```
VBFMVATrainingSamples.json
```

### Configs

There are two `CMSSW` configs avialable in this folder, which will provide all the information needed for Training and Comparison.
These are:
```
VBFDiPhoDiJetMVA_Training.py
VBFDiPhoDiJetMVA_Compare.py
```
The first will create a tree containing the input values of both the VBFMVA and the VBFDiPhoDiJetMVA. (and also contains the output of the MVAs, which is not used)
The second will create a tree containing the output of the VBFMVA and the VBFDiPhoDiJetMVA, but also the output of the VBFMVANew and VBFDiPhoDiJetMVANew (eg the ones with new weights applied). (and also contains the inputs of the MVAs, but is not used)


*Usage*

To generate the Training Trees, simply use the `fggRunJobs.py` tool to run over the required signal and background samples...:
```
fggRunJobs.py --load VBFMVATrainingSamples.json -d /test_diphodijet_training -x cmsRun VBFDiPhoDiJetMVA_Training.py maxEvents=${NEvent}
```
The output trees will be available for you to inspect in `/test_diphodijet_training`... 

For the Comaprison trees, similarly, do:
```
fggRunJobs.py --load VBFMVATrainingSamples.json -d /test_diphodijet_compare -x cmsRun VBFDiPhoDiJetMVA_Compare.py maxEvents=${NEvent}

```
And inspect your output in ` /test_diphodijet_training`...

### Macros

There are two root Macros in the folder,

```
VBFDiPhoDiJetMVA_Training.cc
makeROCs.cc
```

The first will retrain one or other of the MVAs (depending on the options passed), while the second will generate ROC curves comparing old and new trainings for one or other MVA (again, depending on option passsed).

*Usage*


To train the MVA, you need simply generate the required training tree (see above), and do:
```
root -l -q VBFDiPhoDiJetMVA_Training.cc++\(\"${NEvent}\",\"${MVA-TYPE}\"\)
```
`NEvents` must correspond to the number you generated for your training tree, otherwise you will seg fault.
`MVA-TYPE` can be either `"VBF"` or `"VBFDiPhoDiJet"`.
This macro will generate weight files which you can inspect in `/weights`, and root trees with information about the training, which appear in the working directory. The new weight files automatically replace the weights used for the "New" version of the MVA in the Comparison trees. See the inputs for `VBFMVANew` and `VBFDiPhoDiJetMVANew` here `flashgg/Taggers/python/flashggVBFMVA_cff.py`. 

*Warning: do not `rm` the weight files, as they are needed at least as placeholders in the aforementioned python file.*

To generate the ROC curves, you can use `makeROCs.cc` like this:
```
root -l -q makeROCs.cc++\(\"${NEvent}\",\"${MVA-TYPE}\"\)
```
Where the options specified are exactly the same as above.

### Scripts

There are three bash shell scripts which will do all the work for you, if you wish. They also serve as examples of how to use all of the above, so check them out.
```
runDiJetMVARetrain.sh
```
This will retrain the VBF Dijet MVA only, with (by default) 10k events. You can specify the number of events you would liek to retrain with as an argument.

```
runDiPhoDiJetMVARetrain.sh
```
This will only run the DiPho-DiJet MVA retraining.

```
runAllVBFMVARetrain.sh
```
This will retrain both of the MVAs.

*Usage Example*

Very simply, to retrain both MVAs with 1000 events, do
```
. runAllVBFMVARetrain.sh 1000
```
 Then sit back, relax and wait for the ROC curves.
