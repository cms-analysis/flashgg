#ifndef flashgg_StageOneBasedClassifier_h
#define flashgg_StageOneBasedClassifier_h

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "CutBasedClassifier.h"

namespace flashgg {
    template <class T>
    class StageOneBasedClassifier : private CutBasedClassifier<T>, private StageOneNameClassifier<T>
    {
    public:
        typedef StringCutObjectSelector<T, true> functor_type;

        StageOneBasedClassifier( const edm::ParameterSet &cfg ) : 
            CutBasedClassifier<T>(cfg),
            StageOneNameClassifier<T>(cfg)
        {
        }

        std::pair<std::string, int> operator()( const T &obj ) const
        {
            auto cutbased = CutBasedClassifier<T>::operator()(obj);
            
            auto stage1based = StageOneNameClassifier<T>::operator()(obj);
            
            std::string cat = stage1based.first;
            if( ! cutbased.first.empty() ) {
                cat += (cat.empty()?"":":")+cutbased.first; // FIXME: define ad-hoc method with dedicated + operator
            }
            return std::make_pair( cat, stage1based.second  );
        }
    };
}

#endif // flashgg_StageOneBasedClassifier_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

