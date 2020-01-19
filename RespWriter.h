/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#ifndef WRITER_H
#define WRITER_H

#include <ace/Task.h>
#include <dds/DdsDcpsPublicationC.h>
#include "XMLParser.h"
#include "DataReaderListener.h"
#include <time.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class RespWriter : public ACE_Task_Base {
public:

  RespWriter(DDS::DataWriter_ptr writer);

  void start(class DataReaderListenerImpl*);

  void end();

  /** Lanch a thread to write. **/
  virtual int svc();

  bool is_finished() const;
  
  XMLParser *xmlParser= new XMLParser();

private:
  typedef struct {
   long  data_type;
   char  command[8];
   struct timespec sent_time;
   long count;
  } t_data;
  t_data sigData;

  DDS::DataWriter_var writer_;
  ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> finished_instances_;
  
  int msgqid;
  class DataReaderListenerImpl* mDataReaderListenerImpl;
};

#endif /* WRITER_H */
