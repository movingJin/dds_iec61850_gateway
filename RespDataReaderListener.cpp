
   /*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#include <ace/Log_Msg.h>
#include <ace/OS_NS_stdlib.h>

#include <dds/DdsDcpsSubscriptionC.h>
#include <dds/DCPS/Service_Participant.h>

#include "Args.h"
#include "RespDataReaderListener.h"
#include "MessengerTypeSupportC.h"
#include "MessengerTypeSupportImpl.h"

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>

FILE *file;
FILE *interArrivalFile;
//FILE *interArrivalFiles;
RespDataReaderListenerImpl::RespDataReaderListenerImpl()
  : num_reads_(0)
  , valid_(true)
  , reliable_(is_reliable())	//initialize valuable
{
	std::cout << "Transport is " << (reliable_ ? "" : "UN-") << "RELIABLE" <<  std::endl;
    rtt_file_count=0;
    sprintf(fileName,"RTT_DDS%d.csv",rtt_file_count);
	file = fopen(fileName,"w");
        interArrivalFile = fopen("interArrivalFile_client.csv","w");
	//	interArrivalFiles = fopen("interArrivalFiles_client.csv","w");
}

RespDataReaderListenerImpl::~RespDataReaderListenerImpl()
{
    fclose(file);
}

bool
RespDataReaderListenerImpl::is_reliable()
{
  OpenDDS::DCPS::TransportConfig_rch gc = TheTransportRegistry->global_config();
  return !(gc->instances_[0]->transport_type_ == "udp");
}

void RespDataReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader)
throw(CORBA::SystemException)
{
  ++num_reads_;

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
    char printData[255];
    char intervalPrintData[255];

    if (status == DDS::RETCODE_OK) {
/*
      std::cout << "SampleInfo.sample_rank = " << si.sample_rank << std::endl;
      std::cout << "SampleInfo.instance_state = " << si.instance_state << std::endl;
*/
      if (si.valid_data) {
      	const long long NANOS = 1000000000LL;
      	static struct timespec end;
      	static long long  rtt = 0;
        static long long interArrivalTime;

      	if(clock_gettime(CLOCK_MONOTONIC, &end)== -1)
      	{
      		printf("Failed to call clock_gettime\n");
      	}
        rtt = NANOS*(end.tv_sec - message.time_stamp.tv_sec) + (end.tv_nsec - message.time_stamp.tv_nsec);
	rtt = rtt/1000000;
	///Calculate RTT

    mQ.push(end);
    if(message.count > 1)
    {
        mQ.pop();

        interArrivalTime = NANOS * (end.tv_sec - mQ.front().tv_sec) + (end.tv_nsec - mQ.front().tv_nsec);
        interArrivalTime = interArrivalTime/1000000;
        sprintf(intervalPrintData,"%d, %lld",message.count, interArrivalTime);
        fprintf(interArrivalFile,"%s\n",intervalPrintData);
    }

    if(message.count%10000==0)
    {
        fclose(file);
        rtt_file_count++;
        sprintf(fileName,"DDS_RTT%d.csv",rtt_file_count);
        printf("%s\n",fileName);
	    file = fopen(fileName,"w");
    }


    sprintf(printData,"%d, %lld",message.count, rtt);

    fprintf(file,"%s\n",printData);
	printf("%s milli second, inter: %s\n",printData, intervalPrintData);
	///
    if(message.count==499999)
    {
        fclose(file);
        fclose(interArrivalFile);
    }

    if (!counts_.insert(message.count).second) {
        //std::cout << "ERROR: Repeat ";
        //valid_ = false;
    }

/*
	XMLParser *xmlParser= new XMLParser();
	xmlDocPtr doc = xmlParseDoc((const xmlChar*) message.text.in());
	xmlSaveFormatFile("testXML.xml", doc, 1);
	xmlParser->freeXMLDoc(doc);
	
	std::cout << "  **************************************** " << std::endl;
	std::cout << "         count  33    = " << message.count        << std::endl;
    std::cout        << "         message  33     = " << message.text.in()    << std::endl;
    std::cout << "message.subject ="<<  message.subject<<std::endl;
    //	*/
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

void RespDataReaderListenerImpl::on_requested_deadline_missed(
  DDS::DataReader_ptr,
  const DDS::RequestedDeadlineMissedStatus &)
throw(CORBA::SystemException)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: INFO: on_requested_deadline_missed()\n")));
}

void RespDataReaderListenerImpl::on_requested_incompatible_qos(
  DDS::DataReader_ptr,
  const DDS::RequestedIncompatibleQosStatus &)
throw(CORBA::SystemException)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: INFO: on_requested_incompatible_qos()\n")));
}

void RespDataReaderListenerImpl::on_liveliness_changed(
  DDS::DataReader_ptr,
  const DDS::LivelinessChangedStatus &)
throw(CORBA::SystemException)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: INFO: on_liveliness_changed()\n")));
}

void RespDataReaderListenerImpl::on_subscription_matched(
  DDS::DataReader_ptr,
  const DDS::SubscriptionMatchedStatus &)
throw(CORBA::SystemException)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: INFO: on_subscription_matched()\n")));
}

void RespDataReaderListenerImpl::on_sample_rejected(
  DDS::DataReader_ptr,
  const DDS::SampleRejectedStatus&)
throw(CORBA::SystemException)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: INFO: on_sample_rejected()\n")));
}

void RespDataReaderListenerImpl::on_sample_lost(
  DDS::DataReader_ptr,
  const DDS::SampleLostStatus&)
throw(CORBA::SystemException)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: INFO: on_sample_lost()\n")));
}

bool RespDataReaderListenerImpl::is_valid() const
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

