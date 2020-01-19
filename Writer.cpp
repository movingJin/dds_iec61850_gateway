         

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
#include "Writer.h"

#include <stdio.h>
#include <time.h>

const int num_instances_per_writer = 1;
bool reliable = true;
bool wait_for_acks = true;
bool use_mmslite = true;

Writer::Writer(DDS::DataWriter_ptr writer)
  : writer_(DDS::DataWriter::_duplicate(writer)),
    finished_instances_(0)
{
    this->isReceived = true;
}


void
Writer::start()
{
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
Writer::end()
{
  wait();
}

int
Writer::svc()
{
  DDS::InstanceHandleSeq handles;

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

    char userInput[255];
    static struct timespec start;

    message.text	="";
    message.count        = 0;

    vector<string> itemIds;
     vector<string> itemId1s;
    //itemIds.push_back("DRCT$SP$MaxWLim$setMag");
      itemIds.push_back("DRCS$MX$Amp$mag$f");
    //itemIds.push_back("MMDC1$MX$Vol$mag$f");
     itemId1s.push_back("500");
     // itemId1s.push_back("10");
 int i=2;
int k;
    //while(true) {
	 while (message.count < 1000000) {

/*
		 std::cout << " LNode: ";
		 fgets(userInput, sizeof(userInput), stdin);
		 userInput[strlen(userInput)-1]='\0';
		 message.LNode= CORBA::string_dup(userInput);

	 if(strcmp(message.LNode.in(),"DPVM")==0)
	 */

	   {
	      if ((i % 2) == 0)
	     //  if(1)
			 {
				 message.subject = 'g';
				 xmlDocPtr doc = xmlParser->getDataValue_8_2(message, itemIds);

				 xmlBufferPtr buffer = xmlBufferCreate();
				 int size = xmlNodeDump(buffer, doc, xmlDocGetRootElement(doc), 0, 1);
				 message.text = CORBA::string_dup((char *)buffer->content);
				 xmlFree(buffer);
				 xmlParser->freeXMLDoc(doc);
					k=300000;

			 }

			  //usleep(50000);	// 0.5 second sleep

		   else if ((i % 2) != 0)
			 {
				message.subject = 's';
				 xmlDocPtr doc = xmlParser->setDataValue_8_2(message,itemId1s);

				 xmlBufferPtr buffer = xmlBufferCreate();
				 int size = xmlNodeDump(buffer, doc, xmlDocGetRootElement(doc), 0, 1);
				 message.text = CORBA::string_dup((char *)buffer->content);
				 xmlFree(buffer);
				 xmlParser->freeXMLDoc(doc);
					 k=500000;
				 /*	 message.subject = 's';
				 xmlDocPtr doc = xmlParser->setDataValue_resp_8_2(message, item1s);

				 xmlBufferPtr buffer = xmlBufferCreate();
				 int size = xmlNodeDump(buffer, doc, xmlDocGetRootElement(doc), 0, 1);
				 message.text = CORBA::string_dup((char *)buffer->content);
				 xmlFree(buffer);
				 xmlParser->freeXMLDoc(doc);*/
			 }

			 if (clock_gettime(CLOCK_MONOTONIC, &sent_time) == -1)
			 {
				 printf("Failed to call clock_gettime\n");
			 }
			 message.time_stamp.tv_sec = sent_time.tv_sec;
			 message.time_stamp.tv_nsec = sent_time.tv_nsec;
			 //}

			 DDS::ReturnCode_t error;
			 do {
				 error = message_dw->write(message, handle);
			 } while (error == DDS::RETCODE_TIMEOUT);

			 if (error != DDS::RETCODE_OK) {
				 ACE_ERROR((LM_ERROR,
					 ACE_TEXT("%N:%l: svc()")
					 ACE_TEXT(" ERROR: write returned %d!\n"), error));
			 }

			 message.text = "";
			 message.count++;
			 std::cout  << "         count writer    = " << message.count        << std::endl;
			 i++;
			 std:: cout<<"iw=  "  << i  <<   std::endl;



	 }
	   	 usleep(k);	// 0.5 second sleep
	 }

  } catch (const CORBA::Exception& e) {
    e._tao_print_exception("Exception caught in svc():");
  }

  finished_instances_ ++;

  return 0;
}

bool
Writer::is_finished() const
{
  return finished_instances_ == num_instances_per_writer;
}
