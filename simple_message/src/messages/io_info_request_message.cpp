#ifndef FLATHEADERS
#include "simple_message/messages/io_info_request_message.h"
#include "simple_message/joint_data.h"
#include "simple_message/byte_array.h"
#include "simple_message/log_wrapper.h"
#else
#include "io_info_request_message.h"
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
namespace io_info_request_message
{

IOInfoRequestMessage::IOInfoRequestMessage(void)
{
  this->init();
}

IOInfoRequestMessage::~IOInfoRequestMessage(void)
{

}

bool IOInfoRequestMessage::init(industrial::simple_message::SimpleMessage & msg)
{
  ByteArray data = msg.getData();

  this->init();
  return this->unload(&data);
}

void IOInfoRequestMessage::init()
{
  this->setMessageType(msg_type);
  this->message_id = 0;
}

bool IOInfoRequestMessage::load(ByteArray *buffer)
{
  bool rtn = true;
  LOG_COMM("Executing io info request message load");
  rtn &= buffer->load(message_id);
  return rtn;
}

bool IOInfoRequestMessage::unload(ByteArray *buffer)
{
  throw std::runtime_error("Not implemented");
}

}
}

