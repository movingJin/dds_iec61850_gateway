/* *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#include <ace/Log_Msg.h>
#include <ace/OS_NS_stdlib.h>

#include <dds/DdsDcpsSubscriptionC.h>
#include <dds/DCPS/Service_Participant.h>

#include "Args.h"
#include "DataReaderListener.h"
#include "MessengerTypeSupportC.h"
#include "MessengerTypeSupportImpl.h"

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <queue>

FILE *interArrivalFile;

DataReaderListenerImpl::DataReaderListenerImpl()
  : num_reads_(0)
  , valid_(true)
  , reliable_(is_reliable())	//initialize valuable
{
	this->isReceived = false;
    memset(IEC61850_RequestData, 0, sizeof(IEC61850_RequestData[0][0]) * 100 * 255);
	printf("DataReaderListener pid: %d\n",getpid());
	std::cout << "Transport is " << (reliable_ ? "" : "UN-") << "RELIABLE" <<  std::endl;
    interArrivalFile = fopen("interArrivalFile_server.csv","w");
}

DataReaderListenerImpl::~DataReaderListenerImpl()
{
    fclose(interArrivalFile);
}

bool
DataReaderListenerImpl::is_reliable()
{
  OpenDDS::DCPS::TransportConfig_rch gc = TheTransportRegistry->global_config();
  return !(gc->instances_[0]->transport_type_ == "udp");
}

void DataReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader)
throw(CORBA::SystemException)
{
  ++num_reads_;
  if ( -1 == ( msgqid = msgget( (key_t) getpid(), IPC_CREAT | 0600)))
  {
  	perror( "msgget() failed");
  	exit( 1);
  }
 
  try {
    Messenger::MessageDataReader_var message_dr =
      Messenger::MessageDataReader::_narrow(reader);

    if (CORBA::is_nil(message_dr.in())) {
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("%N:%l: on_data_available()")
                 ACE_TEXT(" ERROR: _narrow failed!\n")));
      ACE_OS::exit(-1);
    }

    DDS::SampleInfo si;

    DDS::ReturnCode_t status = message_dr->take_next_sample(message, si) ;

    if (status == DDS::RETCODE_OK) {
	/*
      std::cout << "SampleInfo.sample_rank = " << si.sample_rank << std::endl;
      std::cout << "SampleInfo.instance_state = " << si.instance_state << std::endl;
	*/
      if (si.valid_data) {
        const long long NANOS = 1000000000LL;
        static long long interArrivalTime;

        if (!counts_.insert(message.count).second) {
          std::cout << "ERROR: Repeat ";
          valid_ = false;
        }
	
    std::cout  << "         count      = " << message.count        << std::endl;
	
    xmlDocPtr doc = xmlParseDoc((const xmlChar*) message.text.in());
    xmlNodePtr root = xmlDocGetRootElement(doc);
    xmlNodePtr node = xmlParser->findNodeByName(root, (const xmlChar*)"DOMAIN-SPECIFIC");

	node = node->xmlChildrenNode;
    char data[100][255];
    int i=0;
	while (node != NULL)  {
	    if ((!xmlStrcmp(node->name, (const xmlChar *)"DOMAINID"))) {
            xmlChar* value = xmlNodeGetContent(node);
            strcpy(IEC61850_RequestData[i], (char *) value);
            i++;
 	    }
	    node = node->next;
	}
    char intervalPrintData[255];
/*
    XMLParser *xmlParser= new XMLParser();
    xmlSaveFormatFile("requestXML.xml", doc, 1);
    xmlParser->freeXMLDoc(doc);
*/
    //isReceived = true;
    //    std::cout  << "         message       = " << message.text.in()    << std::endl;
		
	static struct timespec sent_time;
    static struct timespec recieve_time;

	sent_time.tv_sec = message.time_stamp.tv_sec;
	sent_time.tv_nsec = message.time_stamp.tv_nsec;
	sigData.sent_time = sent_time;
    sigData.count = message.count;

    if(clock_gettime(CLOCK_MONOTONIC, &recieve_time)== -1)
    {
        printf("Failed to call clock_gettime\n");
    }

	sigData.data_type=1;
	strcpy(sigData.command,"request");	
	if ( -1 == msgsnd( msgqid, &sigData, sizeof( t_data) - sizeof( long), 0))
        {
		perror( "msgsnd() failed");
		exit( 1);
	}
	
    mQ.push(recieve_time);
    if(message.count > 1)
    {
        mQ.pop();

        interArrivalTime = NANOS * (recieve_time.tv_sec - mQ.front().tv_sec) + (recieve_time.tv_nsec - mQ.front().tv_nsec);
        interArrivalTime = interArrivalTime/1000000;
        sprintf(intervalPrintData,"%d, %lld",message.count, interArrivalTime);
        fprintf(interArrivalFile,"%s\n",intervalPrintData);
        printf("interArrivalTime: %s\n", intervalPrintData);
    
        if(message.count==499999)
        {
            fclose(interArrivalFile);
        }
    }

      } else if (si.instance_state == DDS::NOT_ALIVE_DISPOSED_INSTANCE_STATE) {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: INFO: instance is disposed\n")));

      } else if (si.instance_state == DDS::NOT_ALIVE_NO_WRITERS_INSTANCE_STATE) {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: INFO: instance is unregistered\n")));

      } else {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("%N:%l: on_data_available()")
                   ACE_TEXT(" ERROR: unknown instance state: %d\n"),
                   si.instance_state));
        valid_ = false;
      }

    } else {
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("%N:%l: on_data_available()")
                 ACE_TEXT(" ERROR: unexpected status: %d\n"),
                 status));
      valid_ = false;
    }

  } catch (const CORBA::Exception& e) {
    e._tao_print_exception("Exception caught in on_data_available():");
    ACE_OS::exit(-1);
  }
}

void DataReaderListenerImpl::on_requested_deadline_missed(
  DDS::DataReader_ptr,
  const DDS::RequestedDeadlineMissedStatus &)
throw(CORBA::SystemException)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: INFO: on_requested_deadline_missed()\n")));
}

void DataReaderListenerImpl::on_requested_incompatible_qos(
  DDS::DataReader_ptr,
  const DDS::RequestedIncompatibleQosStatus &)
throw(CORBA::SystemException)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: INFO: on_requested_incompatible_qos()\n")));
}

void DataReaderListenerImpl::on_liveliness_changed(
  DDS::DataReader_ptr,
  const DDS::LivelinessChangedStatus &)
throw(CORBA::SystemException)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: INFO: on_liveliness_changed()\n")));
}

void DataReaderListenerImpl::on_subscription_matched(
  DDS::DataReader_ptr,
  const DDS::SubscriptionMatchedStatus &)
throw(CORBA::SystemException)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: INFO: on_subscription_matched()\n")));
}

void DataReaderListenerImpl::on_sample_rejected(
  DDS::DataReader_ptr,
  const DDS::SampleRejectedStatus&)
throw(CORBA::SystemException)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: INFO: on_sample_rejected()\n")));
}

void DataReaderListenerImpl::on_sample_lost(
  DDS::DataReader_ptr,
  const DDS::SampleLostStatus&)
throw(CORBA::SystemException)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: INFO: on_sample_lost()\n")));
}

void DataReaderListenerImpl::on_budget_exceeded(
  DDS::DataReader_ptr /* reader */,
  const OpenDDS::DCPS::BudgetExceededStatus& /* status */)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) DataListener::on_budget_exceeded\n")));
}

bool DataReaderListenerImpl::is_valid() const
{
  CORBA::Long expected = 0;
  Counts::const_iterator count = counts_.begin();
  bool valid_count = true;
  while (count != counts_.end() && expected < num_messages) {
    if (expected != *count) {
      if (expected < *count) {
        if (reliable_) {
          // if missing multiple
          const bool multi = (expected + 1 < *count);
          std::cout << "ERROR: missing message" << (multi ? "s" : "")
                    << " with count=" << expected;
          if (multi) {
            std::cout << " to count=" << (*count - 1);
          }
          std::cout << std::endl;
          expected = *count;
          // don't increment count;
          valid_count = false;
          continue;
        }
      }
      else {
        bool multi = false;
        while (++count != counts_.end() && *count < expected) {
          multi = true;
        }
        std::cout << "ERROR: received message" << (multi ? "s" : "")
                  << " with a negative count" << std::endl;
        valid_count = false;
        continue;
      }
    }

    ++expected;
    ++count;
  }

  if (count != counts_.end()) {
    std::cout << "ERROR: received messages with count higher than expected values" << std::endl;
    valid_count = false;
  }
  // if didn't receive all the messages (for reliable transport) or didn't receive even get 1/4, then report error
  else if ((int)counts_.size() < num_messages &&
           (reliable_ || (int)(counts_.size() * 4) < num_messages)) {
    std::cout << "ERROR: received " << counts_.size() << " messages, but expected " << num_messages << std::endl;
    valid_count = false;
  }

  return valid_ && valid_count;
}
