
#pragma once
#include <dds/DCPS/QOS_XML_Handler/QOS_XML_Loader.h>
#include <dds/DdsDcpsC.h>
#include <string>

DDS::DataWriterQos dw_qos;
DDS::DataReaderQos dr_qos;
DDS::TopicQos tp_qos;
DDS::PublisherQos pub_qos;
DDS::SubscriberQos sub_qos;
DDS::DomainParticipantQos dp_qos;

int loadQosXML (string fileName)
{
  int retval = 0;
  try
    {
      string profileName = fileName + "#UserProfile";
      OpenDDS::DCPS::QOS_XML_Loader xml_loader;
      DDS::ReturnCode_t const retcode = xml_loader.init (ACE_TEXT (profileName.c_str()));
      if (retcode == DDS::RETCODE_OK)
        {
          DDS::ReturnCode_t retcode_qos;
          retcode_qos = xml_loader.get_datawriter_qos (
                                dw_qos,
                                profileName.c_str(),
                                "TopicName");
          if (retcode_qos != DDS::RETCODE_OK)
            {
              ACE_ERROR ((LM_ERROR, "MAIN - "
                "get_datawriter_qos return an error. Retcode <%d>\n",
                retcode_qos));
              ++retval;
            }

          if(dw_qos.reliability.kind)
          {
            std::cout << "DW ReliableQoS = RELIABLE" << std::endl;
          }
          else
          {
            std::cout << "DW ReliableQoS = BEST_EFFORT" << std::endl;
          }

          retcode_qos = xml_loader.get_datareader_qos (
                                dr_qos,
                                profileName.c_str(),
                                "TopicName");
          if (retcode_qos != DDS::RETCODE_OK)
            {
              ACE_ERROR ((LM_ERROR, "MAIN - "
                "get_datareader_qos return an error. Retcode <%d>\n",
                retcode_qos));
              ++retval;
            }
          if(dr_qos.reliability.kind)
          {
            std::cout << "DR ReliableQoS = RELIABLE" << std::endl;
          }
          else
          {
            std::cout << "DR ReliableQoS = BEST_EFFORT" << std::endl;
          }

          retcode_qos = xml_loader.get_topic_qos (
                                tp_qos,
                                profileName.c_str(),
                                "TopicName");
          if (retcode_qos != DDS::RETCODE_OK)
            {
              ACE_ERROR ((LM_ERROR, "MAIN - "
                "get_topic_qos return an error. Retcode <%d>\n",
                retcode_qos));
              ++retval;
            }

          retcode_qos = xml_loader.get_publisher_qos (
                                pub_qos,
                                profileName.c_str());
          if (retcode_qos != DDS::RETCODE_OK)
            {
              ACE_ERROR ((LM_ERROR, "MAIN - "
                "get_publisher_qos return an error. Retcode <%d>\n",
                retcode_qos));
              ++retval;
            }

          retcode_qos = xml_loader.get_subscriber_qos (
                                sub_qos,
                                profileName.c_str());
          if (retcode_qos != DDS::RETCODE_OK)
            {
              ACE_ERROR ((LM_ERROR, "MAIN - "
                "get_subscriber_qos return an error. Retcode <%d>\n",
                retcode_qos));
              ++retval;
            }

          retcode_qos = xml_loader.get_participant_qos (
                                dp_qos,
                                profileName.c_str());
          if (retcode_qos != DDS::RETCODE_OK)
            {
              ACE_ERROR ((LM_ERROR, "MAIN - "
                "get_participant_qos return an error. Retcode <%d>\n",
                retcode_qos));
              ++retval;
            }
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "MAIN - Init return an error. Retcode <%d>\n",
            retcode));
          ++retval;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("QOS_Dump::main\n");
      return -1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unexpected exception\n")));
      return 1;
    }

  return retval;
}
