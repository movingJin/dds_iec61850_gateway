/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#include <ace/Log_Msg.h>
#include <ace/OS_NS_stdlib.h>
#include <ace/OS_NS_unistd.h>

#include <dds/DdsDcpsPublicationC.h>
#include <dds/DCPS/WaitSet.h>

#include "Args.h"
#include "MessengerTypeSupportC.h"
#include "RespWriter.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct {
   long  data_type;
   char  command[8];
   struct timespec sent_time;
  } t_data;

const int num_instances_per_writer = 1;

RespWriter::RespWriter(DDS::DataWriter_ptr writer)
  : writer_(DDS::DataWriter::_duplicate(writer)),
    finished_instances_(0)
{
    printf("RespWriter pid: %d\n", getpid());
}

void
RespWriter::start(class DataReaderListenerImpl* mDataReaderListenerImpl)
{
  this->mDataReaderListenerImpl = mDataReaderListenerImpl;
  // Lanuch num_instances_per_writer threads. Each thread writes one
  // instance which uses the thread id as the key value.
  if (activate(THR_NEW_LWP | THR_JOINABLE, num_instances_per_writer) == -1) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("%N:%l: start()")
               ACE_TEXT(" activate failed!\n")));
    ACE_OS::exit(-1);
  }
}

void
RespWriter::end()
{
  wait();
}

int
RespWriter::svc()
{
  DDS::InstanceHandleSeq handles;

  //signal Action when publisher request something
  if ( -1 == ( msgqid = msgget( (key_t) getpid(), IPC_CREAT | 0600)))
  {
        perror( "msgget() failed");
        exit( 1);
  } 
  
  try {
    // Block until Subscriber is available
    DDS::StatusCondition_var condition = writer_->get_statuscondition();
    condition->set_enabled_statuses(DDS::PUBLICATION_MATCHED_STATUS);

    DDS::WaitSet_var ws = new DDS::WaitSet;
    ws->attach_condition(condition);

    DDS::Duration_t timeout =
      { DDS::DURATION_INFINITE_SEC, DDS::DURATION_INFINITE_NSEC };

    DDS::ConditionSeq conditions;
    DDS::PublicationMatchedStatus matches = {0, 0, 0, 0, 0};

    do {
      if (ws->wait(conditions, timeout) != DDS::RETCODE_OK) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("%N:%l: svc()")
                   ACE_TEXT(" ERROR: wait failed!\n")));
        ACE_OS::exit(-1);
      }

      if (writer_->get_publication_matched_status(matches) != ::DDS::RETCODE_OK) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("%N:%l: svc()")
                   ACE_TEXT(" ERROR: get_publication_matched_status failed!\n")));
        ACE_OS::exit(-1);
      }

    } while (matches.current_count < 1);

    ws->detach_condition(condition);

    // Write samples
    Messenger::MessageDataWriter_var message_dw
      = Messenger::MessageDataWriter::_narrow(writer_.in());

    if (CORBA::is_nil(message_dw.in())) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("%N:%l: svc()")
                   ACE_TEXT(" ERROR: _narrow failed!\n")));
        ACE_OS::exit(-1);
    }
    Messenger::Message message;

    DDS::InstanceHandle_t handle = message_dw->register_instance(message);

        /* We are all initialized, just service communications                  */
    message.text         = "";
    message.count        = 0;


    while(true) {

	if ( -1 == msgrcv( msgqid, &sigData, sizeof( t_data) - sizeof( long), 1, 0))
      	{
        	perror( "msgrcv() failed");
         	exit( 1);
      	}

	if(strcmp(sigData.command,"request")==0)
	{

        printf("is61850_Changed: %d\n", this->mDataReaderListenerImpl->is61850_Changed);
        
        int u;
        vector<string> items;
        for(u=1; u<this->mDataReaderListenerImpl->is61850_Changed; u++)
        {
            //printf("IEC61850_ResultData: %s\n",this->mDataReaderListenerImpl->IEC61850_ResultData[u]);
            items.push_back(this->mDataReaderListenerImpl->IEC61850_ResultData[u]);
        }

//        items.push_back("21.7");
//        items.push_back("0.0");

		xmlDocPtr doc = xmlParser->getDataValue_resp_8_2(message, items);

                xmlBufferPtr buffer = xmlBufferCreate();
                int size = xmlNodeDump(buffer, doc, xmlDocGetRootElement(doc), 0, 1);
                message.text= CORBA::string_dup((char *) buffer->content);
                xmlFree(buffer);
                xmlParser->freeXMLDoc(doc);

		message.time_stamp.tv_sec = sigData.sent_time.tv_sec;
		message.time_stamp.tv_nsec = sigData.sent_time.tv_nsec;
        message.count = sigData.count;

      		DDS::ReturnCode_t error;
      		do {
        		error = message_dw->write(message, handle);
      		} while (error == DDS::RETCODE_TIMEOUT);

      		if (error != DDS::RETCODE_OK) {
        		ACE_ERROR((LM_ERROR,
                ACE_TEXT("%N:%l: svc()")
                ACE_TEXT(" ERROR: write returned %d!\n"), error));
      		}

		message.text="";

        memset(this->mDataReaderListenerImpl->IEC61850_ResultData, 0, sizeof(this->mDataReaderListenerImpl->IEC61850_ResultData[0][0]) * 100 * 255);
        this->mDataReaderListenerImpl->is61850_Changed = 0;
		mDataReaderListenerImpl->isReceived = false;
	}
	sleep(0);
/*
	}
	if(strcmp(sigData.command,"stop")==0)
	{
		printf("stop work!\n");
	}
*/
    }

  } catch (const CORBA::Exception& e) {
    e._tao_print_exception("Exception caught in svc():");
  }

  finished_instances_ ++;

  return 0;
}

bool
RespWriter::is_finished() const
{
  return finished_instances_ == num_instances_per_writer;
}

