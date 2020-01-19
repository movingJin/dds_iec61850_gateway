/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#ifndef DATAREADER_LISTENER_IMPL
#define DATAREADER_LISTENER_IMPL

#include <dds/DdsDcpsSubscriptionC.h>
#include "XMLParser.h"
#include <time.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class DataReaderListenerImpl
  : public virtual OpenDDS::DCPS::LocalObject<DDS::DataReaderListener> {
public:
  DataReaderListenerImpl();

  virtual ~DataReaderListenerImpl();

  virtual void on_requested_deadline_missed(
    DDS::DataReader_ptr reader,
    const DDS::RequestedDeadlineMissedStatus& status)
  throw(CORBA::SystemException);

  virtual void on_requested_incompatible_qos(
    DDS::DataReader_ptr reader,
    const DDS::RequestedIncompatibleQosStatus& status)
  throw(CORBA::SystemException);

  virtual void on_liveliness_changed(
    DDS::DataReader_ptr reader,
    const DDS::LivelinessChangedStatus& status)
  throw(CORBA::SystemException);

  virtual void on_subscription_matched(
    DDS::DataReader_ptr reader,
    const DDS::SubscriptionMatchedStatus& status)
  throw(CORBA::SystemException);

  virtual void on_sample_rejected(
    DDS::DataReader_ptr reader,
    const DDS::SampleRejectedStatus& status)
  throw(CORBA::SystemException);

  virtual void on_data_available(
    DDS::DataReader_ptr reader)
  throw(CORBA::SystemException);

  virtual void on_sample_lost(
    DDS::DataReader_ptr reader,
    const DDS::SampleLostStatus& status)
  throw(CORBA::SystemException);
  
  virtual void on_budget_exceeded(
    DDS::DataReader_ptr reader,
    const OpenDDS::DCPS::BudgetExceededStatus& status);

  long num_reads() const {
    return num_reads_;
  }

  static bool is_reliable();

  bool is_valid() const;

  XMLParser *xmlParser= new XMLParser();

  bool isReceived;
  char IEC61850_RequestData[100][255];
  char IEC61850_ResultData[100][255];
  int is61850_Changed;


private:
  Messenger::Message message;
  queue<timespec> mQ;

  typedef std::set<CORBA::Long> Counts;
  typedef struct {
   long  data_type;
   char  command[8];
   struct timespec sent_time;
   long count;
  } t_data;
  t_data sigData;

  DDS::DataReader_var reader_;
  long                num_reads_;
  Counts              counts_;
  bool                valid_;
  const bool          reliable_;
  int msgqid;
};

#endif /* DATAREADER_LISTENER_IMPL  */
