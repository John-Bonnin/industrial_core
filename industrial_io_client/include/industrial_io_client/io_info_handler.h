#ifndef IO_INFO_HANDLER_H
#define IO_INFO_HANDLER_H

#include "simple_message/message_handler.h"
#include "simple_message/messages/io_info_reply_message.h"
#include "simple_message/messages/io_info_request_message.h"
#include "industrial_msgs/IOInfo.h"
#include "industrial_io_client/io_service_handler.h"

namespace industrial_io_client
{
class IOInfoHandler : public IOServiceHandler<industrial_msgs::IOInfo, industrial::io_info_request_message::IOInfoRequestMessage, industrial::io_info_reply_message::IOInfoReplyMessage>
{
public:
  IOInfoHandler();
private:
  virtual industrial::io_info_request_message::IOInfoRequestMessage rosRequestToSimpleMessage(industrial_msgs::IOInfo::Request &req);
  virtual void simpleMessageToRosReply(industrial::io_info_reply_message::IOInfoReplyMessage& reply, industrial_msgs::IOInfo::Response &res_out);
};
}


#endif // IO_INFO_HANDLER_H
