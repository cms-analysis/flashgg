Validation plots
================

- Draw simple comparison plots between GLOBE flattrees and FLASHgg globe-like ntuples. 

- produce comparison plots with: 

   ```
   cd $CMSSW_BASE/src/flashgg/GlobeComparison/
   scram b
   MakeValidationPlots cfg/MakeValidationPlot.py 
   ```

- Plots options can be tuned through a python config file:
  - the mc samples for which comparison plots are drawn can be selected with the corresponding flags in the cfg:
   
   ``` 
   process.samplesOpt = cms.PSet( 
        ## turn sample processing on/off
        GGH = cms.bool(True) 
   ```
  - Category splitting is implemented but disabled by default
  
  - histograms configuration is performed through strings:
   ``` 
   'mass 100 90 200' 
   ```
    each string specify:
      1. the variable to be plotted
      2. the number of bins
      3. the histogram range (min / max)

- note that by default plots are store in flashgg/GlobeComparison/plots/

