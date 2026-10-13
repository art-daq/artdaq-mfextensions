#ifndef ARTDAQ_MFEXTENSIONS_ERRORHANDLER_MESSAGEANALYZER_MA_ACTION_SCRIPT_H_
#define ARTDAQ_MFEXTENSIONS_ERRORHANDLER_MESSAGEANALYZER_MA_ACTION_SCRIPT_H_

#include "ErrorHandler/MessageAnalyzer/ma_action.h"
#include "ErrorHandler/MessageAnalyzer/ma_richmsg.h"

#include <string>

namespace novadaq {
namespace errorhandler {

class ma_action_script : public ma_action
{
public:
	ma_action_script(ma_rule const* rule, pset_t const& pset);
	virtual ~ma_action_script() {}

	virtual bool exec();

private:
	std::string script_name;
	std::string script_para;

	ma_richmsg param;
};

}  // end of namespace errorhandler
}  // end of namespace novadaq

#endif  // ARTDAQ_MFEXTENSIONS_ERRORHANDLER_MESSAGEANALYZER_MA_ACTION_SCRIPT_H_
