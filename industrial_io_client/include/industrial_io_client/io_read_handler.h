#ifndef IO_READ_HANDLER_H
#define IO_READ_HANDLER_H

#include "simple_message/message_handler.h"
#include "simple_message/messages/io_read_reply_message.h"
#include "simple_message/messages/io_read_request_message.h"
#include "industrial_msgs/IORead.h"
#include "industrial_io_client/io_service_handler.h"

namespace industrial_io_client
{
class IOReadHandler : public IOServiceHandler<industrial_msgs::IORead, industrial::io_read_request_message::IOReadRequestMessage, industrial::io_read_reply_message::IOReadReplyMessage>
{
public:
  IOReadHandler();
private:
  virtual industrial::io_read_request_message::IOReadRequestMessage rosRequestToSimpleMessage(industrial_msgs::IORead::Request &req);
  virtual void simpleMessageToRosReply(industrial::io_read_reply_message::IOReadReplyMessage& reply, industrial_msgs::IORead::Response &res_out);
};
}


#endif // IO_READ_HANDLER_H
