#ifndef FLATHEADERS
#include "simple_message/messages/io_info_reply_message.h"
#include "simple_message/joint_data.h"
#include "simple_message/byte_array.h"
#include "simple_message/log_wrapper.h"
#else
#include "io_info_reply_message.h"
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
namespace io_info_reply_message
{

IOInfoReplyMessage::IOInfoReplyMessage(void)
{
  this->init();
}

IOInfoReplyMessage::~IOInfoReplyMessage(void)
{

}

bool IOInfoReplyMessage::init(industrial::simple_message::SimpleMessage & msg)
{
  ByteArray data = msg.getData();

  this->init();
  return this->unload(&data);
}

void IOInfoReplyMessage::init()
{
  this->setMessageType(msg_type);
  this->message_id = 0;
}

bool IOInfoReplyMessage::load(ByteArray *buffer)
{
  throw std::runtime_error("Not implemented");
}

bool IOInfoReplyMessage::unload(ByteArray *buffer)
{
  bool rtn = true;
  LOG_COMM("Executing io info reply message unload");
  rtn &= buffer->unloadFront(message_id);
  if (!rtn) return rtn;
  rtn &= buffer->unloadFront(ctrlr_feat_mask);
  if (!rtn) return rtn;
  
  industrial::shared_types::shared_int size;
  rtn &= buffer->unloadFront(size);
  if (!rtn) return rtn;
  
  items.resize(size);
  for (int i = 0; i < size; ++i)
  {
    IOInfoReplyItem item;
    rtn &= buffer->unloadFront(item.type);
    if (!rtn) return rtn;
    rtn &= buffer->unloadFront(item.start);
    if (!rtn) return rtn;
    rtn &= buffer->unloadFront(item.len);
    if (!rtn) return rtn;
    rtn &= buffer->unloadFront(item.feat_mask);
    if (!rtn) return rtn;
    
    items[i] = item;
  }
  
  return rtn;
}

}
}
