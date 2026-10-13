#ifndef ARTDAQ_MFEXTENSIONS_ERRORHANDLER_MESSAGEANALYZER_MA_PARSE_H_
#define ARTDAQ_MFEXTENSIONS_ERRORHANDLER_MESSAGEANALYZER_MA_PARSE_H_

#include "ErrorHandler/MessageAnalyzer/ma_types.h"

namespace novadaq {
namespace errorhandler {

class ma_rule;
class ma_cond_test_expr;

bool parse_condition_expr(std::string const& s, ma_rule* rule);

bool parse_condition_test(std::string const& s, ma_cond_test_expr& expr);

}  // end of namespace errorhandler
}  // end of namespace novadaq

#endif  // ARTDAQ_MFEXTENSIONS_ERRORHANDLER_MESSAGEANALYZER_MA_PARSE_H_
