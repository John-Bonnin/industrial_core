#ifndef FLATHEADERS
#include "simple_message/messages/io_write_reply_message.h"
#include "simple_message/joint_data.h"
#include "simple_message/byte_array.h"
#include "simple_message/log_wrapper.h"
#else
#include "io_write_reply_message.h"
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
namespace io_write_reply_message
{

IOWriteReplyMessage::IOWriteReplyMessage(void)
{
  this->init();
}

IOWriteReplyMessage::~IOWriteReplyMessage(void)
{

}

bool IOWriteReplyMessage::init(industrial::simple_message::SimpleMessage & msg)
{
  ByteArray data = msg.getData();

  this->init();
  return this->unload(&data);
}

void IOWriteReplyMessage::init()
{
  this->setMessageType(msg_type);
  this->message_id = 0;
  this->timestamp = 0;
}

bool IOWriteReplyMessage::load(ByteArray *buffer)
{
  throw std::runtime_error("Not implemented");
}

bool IOWriteReplyMessage::unload(ByteArray *buffer)
{
  bool rtn = true;
  LOG_COMM("Executing io write reply message unload");
  rtn &= buffer->unloadFront(message_id);
  if (!rtn) return rtn;
  rtn &= buffer->unloadFront(timestamp);
  if (!rtn) return rtn;
  
  industrial::shared_types::shared_int size;
  rtn &= buffer->unloadFront(size);
  if (!rtn) return rtn;
  
  items.resize(size);
  for (int i = 0; i < size; ++i)
  {
    IOWriteReplyItem item;
    rtn &= buffer->unloadFront(item.type);
    if (!rtn) return rtn;
    rtn &= buffer->unloadFront(item.index);
    if (!rtn) return rtn;
    rtn &= buffer->unloadFront(item.result);
    if (!rtn) return rtn;
    
    items[i] = item;
  }
  
  return rtn;
}

}
}
