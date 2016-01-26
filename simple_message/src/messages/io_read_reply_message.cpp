#ifndef FLATHEADERS
#include "simple_message/messages/io_read_reply_message.h"
#include "simple_message/joint_data.h"
#include "simple_message/byte_array.h"
#include "simple_message/log_wrapper.h"
#else
#include "io_read_reply_message.h"
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
namespace io_read_reply_message
{

IOReadReplyMessage::IOReadReplyMessage(void)
{
  this->init();
}

IOReadReplyMessage::~IOReadReplyMessage(void)
{

}

bool IOReadReplyMessage::init(industrial::simple_message::SimpleMessage & msg)
{
  ByteArray data = msg.getData();

  this->init();
  return this->unload(&data);
}

void IOReadReplyMessage::init()
{
  this->setMessageType(msg_type);
  this->message_id = 0;
  this->timestamp = 0;
}

bool IOReadReplyMessage::load(ByteArray *buffer)
{
  throw std::runtime_error("Not implemented");
}

bool IOReadReplyMessage::unload(ByteArray *buffer)
{
  bool rtn = true;
  LOG_COMM("Executing io io read reply message unload");
  rtn &= buffer->unloadFront(message_id);
  if (!rtn) return rtn;
  rtn &= buffer->unloadFront(timestamp);
  if (!rtn) return rtn;
  
  industrial::shared_types::shared_int size;
  rtn &= buffer->unload(size);
  if (!rtn) return rtn;
  
  items.resize(size);
  for (int i = 0; i < size; ++i)
  {
    IOReadReplyItem item;
    rtn &= buffer->unload(item.type);
    if (!rtn) return rtn;
    rtn &= buffer->unload(item.index);
    if (!rtn) return rtn;
    rtn &= buffer->unload(item.result);
    if (!rtn) return rtn;
    rtn &= buffer->unload(item.value);
    if (!rtn) return rtn;
    
    items[i] = item;
  }
  
  return rtn;
}

}
}
