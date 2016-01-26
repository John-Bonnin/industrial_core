#ifndef FLATHEADERS
#include "simple_message/messages/io_stream_pub_message.h"
#include "simple_message/joint_data.h"
#include "simple_message/byte_array.h"
#include "simple_message/log_wrapper.h"
#else
#include "io_stream_pub_message.h"
#include "joint_data.h"
#include "byte_array.h"
#include "log_wrapper.h"
#endif

using namespace industrial::shared_types;
using namespace industrial::byte_array;
using namespace industrial::simple_message;
using namespace industrial::joint_data;

namespace industrial
{
namespace io_stream_pub_message
{

IOStreamPubMessage::IOStreamPubMessage(void)
{
  this->init();
}

IOStreamPubMessage::~IOStreamPubMessage(void)
{

}

bool IOStreamPubMessage::init(industrial::simple_message::SimpleMessage & msg)
{
  ByteArray data = msg.getData();

  this->init();
  return this->unload(&data);
}

void IOStreamPubMessage::init()
{
  this->setMessageType(msg_type);
  this->timestamp = 0;
}

bool IOStreamPubMessage::load(ByteArray *buffer)
{
  throw std::runtime_error("Not implemented");
}

bool IOStreamPubMessage::unload(ByteArray *buffer)
{
  bool rtn = true;
  LOG_COMM("Executing io stream pub message unload");

  rtn &= buffer->unloadFront(this->timestamp);
  if (!rtn) return rtn;
  industrial::shared_types::shared_int num_items; 
  rtn &= buffer->unloadFront(num_items);
  if (!rtn) return rtn;
  
  this->items.resize(num_items);
  for (int i = 0; i < num_items; i++) {
    IOStreamPubItem item;
    
    rtn &= buffer->unloadFront(item.type);
    if (!rtn) return rtn;
    
    rtn &= buffer->unloadFront(item.start);
    if (!rtn) return rtn;
    
    industrial::shared_types::shared_int num_values;
    rtn &= buffer->unloadFront(num_values);
    if (!rtn) return rtn;
    
    item.values.resize(num_values);
    for (int j = 0; j < num_values; j++) {
      industrial::shared_types::shared_int value;
      
      rtn &= buffer->unloadFront(value);
      if (!rtn) return rtn;
      
      item.values[j] = value;
    }

    this->items[i] = item;
  }
  
  return rtn;
}

}
}

