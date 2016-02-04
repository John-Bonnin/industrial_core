#include "industrial_io_client/io_write_handler.h"
#include "simple_message/messages/io_read_request_message.h"
#include <iostream>
#include <boost/chrono.hpp>

namespace industrial_io_client
{

IOWriteHandler::IOWriteHandler() : IOServiceHandler("write", industrial::io_write_reply_message::msg_type)
{

}

industrial::io_write_request_message::IOWriteRequestMessage IOWriteHandler::rosRequestToSimpleMessage(industrial_msgs::IOWrite::Request &req)
{
  industrial::io_write_request_message::IOWriteRequestMessage message;
  message.message_id = getNextMessageId();
  for (int i = 0; i < req.items.size(); ++i)
  {
    industrial::io_write_request_message::IOWriteRequestItem item;
    item.type = req.items[i].type;
    item.index = req.items[i].index;
    item.value = req.items[i].value;
    message.items.push_back(item);
  }
  return message;
}

void IOWriteHandler::simpleMessageToRosReply(industrial::io_write_reply_message::IOWriteReplyMessage& reply, industrial_msgs::IOWrite::Response &res_out)
{
  res_out.items.resize(reply.items.size());
  for (int i = 0; i < reply.items.size(); ++i)
  {
    industrial_msgs::IOWriteReplyItem item;
    item.type = reply.items[i].type;
    item.index = reply.items[i].index;
    item.result = reply.items[i].result;
    res_out.items[i] = item;
  }
}


}
