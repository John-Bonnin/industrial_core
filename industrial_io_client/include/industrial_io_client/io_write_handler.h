#ifndef IO_WRITE_HANDLER_H
#define IO_WRITE_HANDLER_H

#include "simple_message/message_handler.h"
#include "simple_message/messages/io_write_reply_message.h"
#include "simple_message/messages/io_write_request_message.h"
#include "industrial_msgs/IOWrite.h"
#include "industrial_io_client/io_service_handler.h"

namespace industrial_io_client
{
class IOWriteHandler : public IOServiceHandler<industrial_msgs::IOWrite, industrial::io_write_request_message::IOWriteRequestMessage, industrial::io_write_reply_message::IOWriteReplyMessage>
{
public:
  IOWriteHandler();
private:
  virtual industrial::io_write_request_message::IOWriteRequestMessage rosRequestToSimpleMessage(industrial_msgs::IOWrite::Request &req);
  virtual void simpleMessageToRosReply(industrial::io_write_reply_message::IOWriteReplyMessage& reply, industrial_msgs::IOWrite::Response &res_out);
};
}


#endif // IO_WRITE_HANDLER_H
