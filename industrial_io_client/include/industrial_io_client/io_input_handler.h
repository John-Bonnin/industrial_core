#ifndef IO_INPUT_HANDLER_H
#define IO_INPUT_HANDLER_H

#include "simple_message/message_handler.h"
#include "simple_message/messages/io_stream_pub_message.h"

namespace industrial_io_client
{
class IOInputHandler : public industrial::message_handler::MessageHandler
{
  // since this class defines a different init(), this helps find the base-class init()
  using industrial::message_handler::MessageHandler::init;
public:
  bool init(industrial::smpl_msg_connection::SmplMsgConnection* connection);
private:
  bool internalCB(industrial::simple_message::SimpleMessage& in);
  bool internalCB(industrial::io_stream_pub_message::IOStreamPubMessage& inputMessage);
};
}

#endif
