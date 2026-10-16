#ifndef ARTDAQ_MFEXTENSIONS_ERRORHANDLER_MESSAGEANALYZER_MA_COND_TEST_ANDEXPR_H_
#define ARTDAQ_MFEXTENSIONS_ERRORHANDLER_MESSAGEANALYZER_MA_COND_TEST_ANDEXPR_H_

#include "ErrorHandler/MessageAnalyzer/ma_cond_test_primary.h"

#include <list>

namespace novadaq {
namespace errorhandler {

class ma_cond_test_andexpr
{
public:
	ma_cond_test_andexpr()
	    : primaries() {}

	bool evaluate(ma_condition const* cond) const;

	void insert(ma_cond_test_primary const& primary)
	{
		primaries.push_back(primary);
	}

private:
	test_primaries_t primaries;
};

typedef std::list<ma_cond_test_andexpr> test_andexprs_t;

}  // end of namespace errorhandler
}  // end of namespace novadaq

#endif  // ARTDAQ_MFEXTENSIONS_ERRORHANDLER_MESSAGEANALYZER_MA_COND_TEST_ANDEXPR_H_
