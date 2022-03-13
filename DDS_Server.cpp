/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */


#include <dds/DdsDcpsInfrastructureC.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/SubscriberImpl.h>
#include <dds/DCPS/WaitSet.h>

#include "dds/DCPS/StaticIncludes.h"
#ifdef ACE_AS_STATIC_LIBS
# ifndef OPENDDS_SAFETY_PROFILE
#include <dds/DCPS/transport/udp/Udp.h>
#include <dds/DCPS/transport/multicast/Multicast.h>
#include <dds/DCPS/RTPS/RtpsDiscovery.h>
#include <dds/DCPS/transport/shmem/Shmem.h>
# endif
#include <dds/DCPS/transport/rtps_udp/RtpsUdp.h>
#endif

#include <dds/DCPS/transport/framework/TransportRegistry.h>
#include <dds/DCPS/transport/framework/TransportConfig.h>
#include <dds/DCPS/transport/framework/TransportInst.h>

#include "DataReaderListener.h"
#include "MessengerTypeSupportImpl.h"
#include "Args.h"

#include "RespWriter.h"
#include <thread>
#include "MMS_client.h"
#include "QosDump.h"

const char auth_ca_file[] = "file:./certs/identity_ca_cert.pem";
const char perm_ca_file[] = "file:./certs/permissions_ca_cert.pem";
const char id_cert_file[] = "file:./certs/test_participant_02_cert.pem";
const char id_key_file[] = "file:./certs/test_participant_02_private_key.pem";
const char governance_file[] = "file:./governance_signed.p7s";
const char permissions_file[] = "file:./permissions_2_signed.p7s";

const char DDSSEC_PROP_IDENTITY_CA[] = "dds.sec.auth.identity_ca";
const char DDSSEC_PROP_IDENTITY_CERT[] = "dds.sec.auth.identity_certificate";
const char DDSSEC_PROP_IDENTITY_PRIVKEY[] = "dds.sec.auth.private_key";
const char DDSSEC_PROP_PERM_CA[] = "dds.sec.access.permissions_ca";
const char DDSSEC_PROP_PERM_GOV_DOC[] = "dds.sec.access.governance";
const char DDSSEC_PROP_PERM_DOC[] = "dds.sec.access.permissions";

bool reliable = false;
bool wait_for_acks = false;
bool use_mmslite = false;

void append(DDS::PropertySeq& props, const char* name, const char* value)
{
  const DDS::Property_t prop = {name, value, false /*propagate*/};
  const unsigned int len = props.length();
  props.length(len + 1);
  props[len] = prop;
}

bool dw_reliable() {
	  OpenDDS::DCPS::TransportConfig_rch gc = TheTransportRegistry->global_config();
	    return !(gc->instances_[0]->transport_type_ == "udp");
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
    std::cout << "Last Edit 2018.11.02 by DJ" << std::endl;

  int status = 0;

  try {
    // Initialize DomainParticipantFactory
    DDS::DomainParticipantFactory_var dpf =
      TheParticipantFactoryWithArgs(argc, argv);

    int error;
    if ((error = parse_args(argc, argv)) != 0) {
      return error;
    }
    loadQosXML("qosxmls/MGMS_qos");

#if defined(OPENDDS_SECURITY)
    if (TheServiceParticipant->get_security()) {
      DDS::PropertySeq& props = dp_qos.property.value;
      append(props, DDSSEC_PROP_IDENTITY_CA, auth_ca_file);
      append(props, DDSSEC_PROP_IDENTITY_CERT, id_cert_file);
      append(props, DDSSEC_PROP_IDENTITY_PRIVKEY, id_key_file);
      append(props, DDSSEC_PROP_PERM_CA, perm_ca_file);
      append(props, DDSSEC_PROP_PERM_GOV_DOC, governance_file);
      append(props, DDSSEC_PROP_PERM_DOC, permissions_file);
    }
#endif

    // Create DomainParticipant
    DDS::DomainParticipant_var participant =
      dpf->create_participant(4,
                              dp_qos,
                              DDS::DomainParticipantListener::_nil(),
                              OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (CORBA::is_nil(participant.in())) {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("%N:%l main()")
                        ACE_TEXT(" ERROR: create_participant() failed!\n")), -1);
    }

    // Register Type (Messenger::Message)
    Messenger::MessageTypeSupport_var ts =
      new Messenger::MessageTypeSupportImpl();

    if (ts->register_type(participant.in(), "") != DDS::RETCODE_OK) {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("%N:%l main()")
                        ACE_TEXT(" ERROR: register_type() failed!\n")), -1);
    }
	CORBA::String_var type_name = ts->get_type_name();

	//=================================================================================================================//
	// Create RespTopic (Movie Discussion List)
    DDS::Topic_var respTopic =
      participant->create_topic("Response Topic",
                                type_name.in(),
                                TOPIC_QOS_DEFAULT,
                                DDS::TopicListener::_nil(),
                                OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (CORBA::is_nil(respTopic.in())) {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("%N:%l main()")
                        ACE_TEXT(" ERROR: create_topic() failed!\n")), -1);
    }

	// Create response Publisher
	DDS::Publisher_var pub =
        participant->create_publisher(pub_qos,
                                      DDS::PublisherListener::_nil(),
                                      OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	if (CORBA::is_nil(pub.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("%N:%l: main()")
                          ACE_TEXT(" ERROR: create_publisher failed!\n")),
                         -1);
    }

	// Create response DataWriter
	DDS::DataWriter_var dw =
        pub->create_datawriter(respTopic.in(),
                               dw_qos,
                               DDS::DataWriterListener::_nil(),
                               OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	if (CORBA::is_nil(dw.in())) {
    	ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("%N:%l: main()")
                          ACE_TEXT(" ERROR: create_datawriter failed!\n")),
                         -1);
	}

	// Start writing threads
	std::cout << "Creating Writer" << std::endl;
	RespWriter* respWriter = new RespWriter(dw.in());

	//=================================================================================================================//

    // Create Topic (Movie Discussion List)
    DDS::Topic_var topic =
      participant->create_topic("Request Topic",
                                type_name.in(),
                                TOPIC_QOS_DEFAULT,
                                DDS::TopicListener::_nil(),
                                OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (CORBA::is_nil(topic.in())) {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("%N:%l main()")
                        ACE_TEXT(" ERROR: create_topic() failed!\n")), -1);
    }

    // Create Subscriber
    DDS::Subscriber_var sub =
      participant->create_subscriber(sub_qos,
                                     DDS::SubscriberListener::_nil(),
                                     OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (CORBA::is_nil(sub.in())) {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("%N:%l main()")
                        ACE_TEXT(" ERROR: create_subscriber() failed!\n")), -1);
    }


    // Create DataReader
    DataReaderListenerImpl* const listener_servant = new DataReaderListenerImpl();
    DDS::DataReaderListener_var listener(listener_servant);
    
    DDS::DataReader_var reader =
      sub->create_datareader(topic.in(),
                             dr_qos,
                             listener.in(),
                             OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (CORBA::is_nil(reader.in())) {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("%N:%l main()")
                        ACE_TEXT(" ERROR: create_datareader() failed!\n")), -1);
    }

    // Block until Publisher completes
    DDS::StatusCondition_var condition = reader->get_statuscondition();
    condition->set_enabled_statuses(DDS::SUBSCRIPTION_MATCHED_STATUS);

    DDS::WaitSet_var ws = new DDS::WaitSet;
    ws->attach_condition(condition);

    DDS::Duration_t timeout =
      { DDS::DURATION_INFINITE_SEC, DDS::DURATION_INFINITE_NSEC };

    DDS::ConditionSeq conditions;
    
    DDS::SubscriptionMatchedStatus matches = { 0, 0, 0, 0, 0 };

    std::thread IEC61850_thread;
    if(use_mmslite==true)
    {
        IEC61850_thread = std::thread(client_run, listener_servant->IEC61850_RequestData, listener_servant->IEC61850_ResultData, &listener_servant->is61850_Changed);
    }
    //=================================================================================================================//
    std::cout << "Starting Writer" << std::endl;
    respWriter->start(listener_servant);

    while (!respWriter->is_finished()) {
		ACE_Time_Value small_time(0, 250000);
    	ACE_OS::sleep(small_time);
    }

    std::cout << "Writer finished " << std::endl;
    respWriter->end();

    if (wait_for_acks) {
    	std::cout << "Writer wait for ACKS" << std::endl;
        DDS::Duration_t timeout =
        { DDS::DURATION_INFINITE_SEC, DDS::DURATION_INFINITE_NSEC };
        dw->wait_for_acknowledgments(timeout);
    } else {
    	// let any missed multicast/rtps messages get re-delivered
        std::cout << "Writer wait small time" << std::endl;
        ACE_Time_Value small_time(0, 250000);
        ACE_OS::sleep(small_time);
    }

    std::cerr << "deleting DW" << std::endl;
    delete respWriter;
	//=================================================================================================================//

	while (true) {
      if (reader->get_subscription_matched_status(matches) != DDS::RETCODE_OK) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("%N:%l main()")
                          ACE_TEXT(" ERROR: get_subscription_matched_status() failed!\n")), -1);
      } 

      if (ws->wait(conditions, timeout) != DDS::RETCODE_OK) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("%N:%l main()")
                          ACE_TEXT(" ERROR: wait() failed!\n")), -1);
      }
	  
    }

    status = listener_servant->is_valid() ? 0 : -1;

    //if(use_mmslite==true)
#if defined(MMS_CLIENT)
        IEC61850_thread.join();
#endif
    ws->detach_condition(condition);
    
    // Clean-up!
    participant->delete_contained_entities();
    dpf->delete_participant(participant.in());
    std::cout << "deleting participant" << std::endl;
    TheServiceParticipant->shutdown();
    std::cout << "shutdown" << std::endl;

  } catch (const CORBA::Exception& e) {
    e._tao_print_exception("Exception caught in main():");
    status = -1;
  }
	exit(0);
  return status;
}
