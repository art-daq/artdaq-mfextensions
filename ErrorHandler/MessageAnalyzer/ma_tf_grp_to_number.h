#ifndef ARTDAQ_MFEXTENSIONS_ERRORHANDLER_MESSAGEANALYZER_MA_TF_GRP_TO_NUMBER_H_
#define ARTDAQ_MFEXTENSIONS_ERRORHANDLER_MESSAGEANALYZER_MA_TF_GRP_TO_NUMBER_H_

#include "ErrorHandler/MessageAnalyzer/ma_test_function.h"

namespace novadaq {
namespace errorhandler {

class ma_tf_grp_to_number : public ma_test_function
{
public:
	ma_tf_grp_to_number()
	    : group(0) {}

	virtual boost::any
	evaluate(ma_condition const& cond);

	virtual bool
	parse_arguments(anys_t const& args);

private:
	int group;
};

}  // end of namespace errorhandler
}  // end of namespace novadaq

#endif  // ARTDAQ_MFEXTENSIONS_ERRORHANDLER_MESSAGEANALYZER_MA_TF_GRP_TO_NUMBER_H_
