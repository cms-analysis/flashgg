#ifndef flashgg_Stage1BasedClassifier_h
#define flashgg_Stage1BasedClassifier_h

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "CutBasedClassifier.h"

namespace flashgg {
    template <class T>
    class Stage1BasedClassifier : private CutBasedClassifier<T>, private Stage1NameClassifier<T>
    {
    public:
        typedef StringCutObjectSelector<T, true> functor_type;

        Stage1BasedClassifier( const edm::ParameterSet &cfg ) : 
            CutBasedClassifier<T>(cfg),
            Stage1NameClassifier<T>(cfg)
        {
        }

        std::pair<std::string, int> operator()( const T &obj ) const
        {
            auto cutbased = CutBasedClassifier<T>::operator()(obj);
            
            auto stage1based = Stage1NameClassifier<T>::operator()(obj);
            
            std::string cat = stage1based.first;
            if( ! cutbased.first.empty() ) {
                cat += (cat.empty()?"":":")+cutbased.first; // FIXME: define ad-hoc method with dedicated + operator
            }
            return std::make_pair( cat, stage1based.second  );
        }
    };
}

#endif // flashgg_Stage1BasedClassifier_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

