#include "cetlib/PluginTypeDeducer.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/ConfigurationTable.h"

#include "cetlib/compiler_macros.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "messagefacility/MessageService/ELdestination.h"
#include "messagefacility/Utilities/ELseverityLevel.h"
#include "messagefacility/Utilities/exception.h"

// C/C++ includes
#include <librdkafka/rdkafkacpp.h>
#include <chrono>
#include <fstream>

#define TRACE_NAME "Kafka_mfPlugin"
#include "trace.h"

namespace mfplugins {
using mf::ErrorObj;
using mf::service::ELdestination;

/// <summary>
/// Message Facility Kafka Streamer Destination
/// Sends messages via Kafka
/// </summary>
class ELKafka : public ELdestination
{
public:
	/**
	 * \brief Configuration Parameters for ELKafka
	 */
	struct Config
	{
		/// ELDestination common config parameters
		fhicl::TableFragment<ELdestination::Config> elDestConfig;
		/// "kafka_key" (Default: "artdaq"): Key for grouping log messages in Kafka
		fhicl::Atom<std::string> kafka_key = fhicl::Atom<std::string>{fhicl::Name{"kafka_key"}, fhicl::Comment{"Key for grouping log messages in Kafka"}, "artdaq"};
		/// "kafka_server" (Default: "localhost:30092"): Address of the Kafka service
		fhicl::Atom<std::string> kafka_server = fhicl::Atom<std::string>{fhicl::Name{"kafka_server"}, fhicl::Comment{"Address of the Kafka service"}, "localhost:30092"};
		/// "kafka_client" (Default: Application name from /proc/<pid>/cmdline): Name of this Kafka client
		fhicl::Atom<std::string> kafka_client = fhicl::Atom<std::string>{fhicl::Name{"kafka_client"}, fhicl::Comment{"Name of this Kafka client"}, ""};
	};
	/// Used for ParameterSet validation
	using Parameters = fhicl::WrappedTable<Config>;

public:
	/// <summary>
	/// ELKafka Constructor
	/// </summary>
	/// <param name="pset">ParameterSet used to configure ELKafka</param>
	ELKafka(Parameters const& pset);

	/**
	 * \brief Serialize a MessageFacility message to the output
	 * \param o Stringstream object containing message data
	 * \param e MessageFacility object containing header information
	 */
	void routePayload(const std::ostringstream& o, const ErrorObj& e) override;

private:
	// Parameters
	std::unique_ptr<RdKafka::Producer> kafka_producer_;
	std::string kafka_key_;
};

// END DECLARATION
//======================================================================
// BEGIN IMPLEMENTATION

//======================================================================
// ELKafka c'tor
//======================================================================

ELKafka::ELKafka(Parameters const& pset)
    : ELdestination(pset().elDestConfig())
    , kafka_key_(pset().kafka_key())
{
	// get process name from '/proc/pid/cmdline'
	std::stringstream ss;
	ss << "//proc//" << getpid() << "//cmdline";
	std::ifstream procfile{ss.str().c_str()};

	std::string procinfo;

	if (procfile.is_open())
	{
		procfile >> procinfo;
		procfile.close();
	}

	size_t end = procinfo.find('\0');
	size_t start = procinfo.find_last_of('/', end);

	std::string app = procinfo.substr(start + 1, end - start - 1);

	RdKafka::Conf* k_conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
	std::string errstr;

	k_conf->set("bootstrap.servers", pset().kafka_server(), errstr);
	if (errstr != "")
	{
		throw std::runtime_error(errstr);
	}

	std::string client_id = pset().kafka_client();
	if (client_id == "")
	{
		client_id = app;
	}

	k_conf->set("client.id", client_id, errstr);
	if (errstr != "")
	{
		throw std::runtime_error(errstr);
	}

	// Create producer instance
	kafka_producer_.reset(RdKafka::Producer::create(k_conf, errstr));

	if (errstr != "")
	{
		throw std::runtime_error(errstr);
	}
}

//======================================================================
// Message router ( overriddes ELdestination::routePayload )
//======================================================================
void ELKafka::routePayload(const std::ostringstream& oss, const ErrorObj& msg)
{
	const auto& xid = msg.xid();
	// get the topic
	auto topic = xid.id();

	auto key = kafka_key_;

	std::string binary = oss.str();
	uint64_t timestamp_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	//      RdKafka::Producer::RK_MSG_COPY to be investigated
	RdKafka::ErrorCode err = kafka_producer_->produce(topic,
	                                                  RdKafka::Topic::PARTITION_UA,
	                                                  RdKafka::Producer::RK_MSG_COPY,
	                                                  const_cast<char*>(binary.c_str()), binary.size(),
	                                                  key.c_str(),
	                                                  key.size(),
	                                                  timestamp_ms,
	                                                  nullptr);
	if (err != RdKafka::ERR_NO_ERROR)
	{
		TLOG(TLVL_ERROR) << "Error sending message to Kafka: " << RdKafka::err2str(err);
	}
}
}  // end namespace mfplugins

//======================================================================
//
// makePlugin function
//
//======================================================================

#ifndef EXTERN_C_FUNC_DECLARE_START
#define EXTERN_C_FUNC_DECLARE_START extern "C" {
#endif

EXTERN_C_FUNC_DECLARE_START
auto makePlugin(const std::string& /*unused*/, const fhicl::ParameterSet& pset)
{
	return std::make_unique<mfplugins::ELKafka>(pset);
}
}

DEFINE_BASIC_PLUGINTYPE_FUNC(mf::service::ELdestination)
