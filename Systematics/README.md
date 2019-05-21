Systematics
============

1. b-Tagging:

--> For b-tag systematics, the SF are provided by BTV for |eta| < 2.4 for year 2016 and |eta| < 2.5 for year 2017 and 2018. The
eta value is read from MetaData/data/MetaConditions/Era_year.json

--> To read proper csv file one just needs to define b-tagger in the above json file. By default it is kept pfDeepJet since 
it is the most supported Tagger from BTV after DeepCSV with better performance for full Run2 analyses
https://indico.cern.ch/event/788727/contributions/3395836/attachments/1830178/2996964/DeepJet_Integration_flashgg_4Apr2019.pdf
CSVv2 has been removed since it is not suported by BTV for Run2 analyses.

--> In the same JSON file, the bdiscriminator cut values are for medium working point for all the b-taggers. For other WP point cut
value, please have a look at this TWiki https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation




    
