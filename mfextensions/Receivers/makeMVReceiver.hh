#ifndef ARTDAQ_MFEXTENSIONS_MFEXTENSIONS_RECEIVERS_MAKEMVRECEIVER_HH_
#define ARTDAQ_MFEXTENSIONS_MFEXTENSIONS_RECEIVERS_MAKEMVRECEIVER_HH_
// Using LibraryManager, find the correct library and return an instance
// of the specified generator.

#include "fhiclcpp/fwd.h"

#include <memory>
#include <string>

namespace mfviewer {
class MVReceiver;

std::unique_ptr<MVReceiver> makeMVReceiver(std::string const& receiver_plugin_spec, fhicl::ParameterSet const& ps);
}  // namespace mfviewer
#endif  // ARTDAQ_MFEXTENSIONS_MFEXTENSIONS_RECEIVERS_MAKEMVRECEIVER_HH_
