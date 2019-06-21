#ifndef __FOXWOLFRAM_H__
#define __FOXWOLFRAM_H__

#include "Math/SpecFuncMathMore.h"
#include "TLorentzVector.h"
#include <cmath>

//loosely based on http://arxiv.org/abs/1212.4436

class FoxWolfram
{
    protected:
        std::vector<TLorentzVector> _eventVectors;
        double totalEnergy;
    public:
        enum WeightType
        {
            SHAT,PT,ETA,PSUM,PZ,ONE
        };
    
        FoxWolfram(const std::vector<TLorentzVector>& eventVectors):
            _eventVectors(eventVectors)
        {
        }
        
        
        
        double getMoment(WeightType wType, unsigned int order)
        {
            switch (wType)
            {
                case SHAT:
                {
                    return getMomentShat(order);
                }
                case PT:
                {
                    return getMomentPt(order);
                }
                case ETA:
                {
                    return getMomentEta(order);
                }
                case PSUM:
                {
                    return getMomentPsum(order);
                }
                case PZ:
                {
                    return getMomentPz(order);
                }
                case ONE:
                {
                    return getMomentOne(order);
                } 
            }
            return -1;
        }
        
        double cosTheta(const TLorentzVector& v1, const TLorentzVector& v2)
        {
	  //return std::cos(v1.getTheta())*std::cos(v2.getTheta())+std::sin(v1.getTheta())*std::sin(v2.getTheta())*std::cos(v1.getPhi()-v2.getPhi());
	  double ret = (v1.X()*v2.X()+v1.Y()*v2.Y()+v1.Z()*v2.Z())/sqrt(v1.Vect().Mag2()*v2.Vect().Mag2());
	  //cout << ret << endl;
	  return ret;
        }
       
        
        double getMomentShat(unsigned int order) 
        {
            double sum = 0.0;
            TLorentzVector shat(0,0,0,0);
            for (unsigned int i = 0; i < _eventVectors.size(); ++i)
            {  
                shat+=_eventVectors[i];
                for (unsigned int j = 0; j < _eventVectors.size(); ++j)
                {
                    double angle = cosTheta(_eventVectors[i],_eventVectors[j]);
                    sum+=_eventVectors[i].P()*_eventVectors[j].P()*ROOT::Math::legendre(order,angle);
                }
            }
            
            return sum/shat.P()/shat.P();
        }
        
        double getMomentPt(unsigned int order) 
        {
            double sum = 0.0;
            double norm = 0.0;
            for (unsigned int i = 0; i < _eventVectors.size(); ++i)
            {  
                norm+=_eventVectors[i].Pt();
                for (unsigned int j = 0; j < _eventVectors.size(); ++j)
                {
                    double angle = cosTheta(_eventVectors[i],_eventVectors[j]);
                    sum+=_eventVectors[i].Pt()*_eventVectors[j].Pt()*ROOT::Math::legendre(order,angle);
                }
            }
            
            return sum/norm/norm;
        }
        
        double getMomentEta(unsigned int order) 
        {
            double avgEta = 0.0;
            for (unsigned int i = 0; i < _eventVectors.size(); ++i)
            {  
                avgEta+=_eventVectors[i].Eta();
            }
            avgEta/= _eventVectors.size();
        
            double norm = 0.0;
            double sum = 0.0;
            for (unsigned int i = 0; i < _eventVectors.size(); ++i)
            {  
                norm+=1.0/(fabs(_eventVectors[i].Eta()-avgEta));
                for (unsigned int j = 0; j < _eventVectors.size(); ++j)
                {
                    double angle = cosTheta(_eventVectors[i],_eventVectors[j]);
                    sum+=1.0/(fabs(_eventVectors[i].Eta()-avgEta)*fabs(_eventVectors[j].Eta()-avgEta))*ROOT::Math::legendre(order,angle);
                }
            }
            return sum/norm/norm;
        }
        
        double getMomentPsum(unsigned int order) 
        {
            double sum = 0.0;
            double norm = 0.0;
            for (unsigned int i = 0; i < _eventVectors.size(); ++i)
            {  
                norm+=_eventVectors[i].P();
                for (unsigned int j = 0; j < _eventVectors.size(); ++j)
                {
                    double angle = cosTheta(_eventVectors[i],_eventVectors[j]);
                    sum+=_eventVectors[i].P()*_eventVectors[j].P()*ROOT::Math::legendre(order,angle);
                }
            }
            
            return sum/norm/norm;
        }
        
        double getMomentPz(unsigned int order) 
        {
            double sum = 0.0;
            double norm = 0.0;
            for (unsigned int i = 0; i < _eventVectors.size(); ++i)
            {  
                norm+=_eventVectors[i].Pz();
                for (unsigned int j = 0; j < _eventVectors.size(); ++j)
                {
                    double angle = cosTheta(_eventVectors[i],_eventVectors[j]);
                    sum+=_eventVectors[i].Pz()*_eventVectors[j].Pz()*ROOT::Math::legendre(order,angle);
                }
            }

            return sum/norm/norm;
        }
        
        double getMomentOne(unsigned int order) 
        {
            double sum = 0.0;
            for (unsigned int i = 0; i < _eventVectors.size(); ++i)
            {
                for (unsigned int j = 0; j < _eventVectors.size(); ++j)
                {
                    double angle = cosTheta(_eventVectors[i],_eventVectors[j]);
                    sum+=ROOT::Math::legendre(order,angle);
                }
            }
            
            return sum;
        }
};

#endif

