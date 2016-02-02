#include "industrial_io_client/io_read_handler.h"
#include "simple_message/messages/io_read_request_message.h"
#include <iostream>
#include <boost/chrono.hpp>

namespace industrial_io_client
{

IOReadHandler::IOReadHandler() : IOServiceHandler("read", industrial::io_read_reply_message::msg_type)
{

}

industrial::io_read_request_message::IOReadRequestMessage IOReadHandler::rosRequestToSimpleMessage(industrial_msgs::IORead::Request &req)
{
  industrial::io_read_request_message::IOReadRequestMessage message;
  message.message_id = getNextMessageId();
  for (int i = 0; i < req.items.size(); ++i)
  {
    industrial::io_read_request_message::IOReadRequestItem item;
    item.type = req.items[i].type;
    item.index = req.items[i].index;
    message.items.push_back(item);
  }
  return message;
}

void IOReadHandler::simpleMessageToRosReply(industrial::io_read_reply_message::IOReadReplyMessage& reply, industrial_msgs::IORead::Response &res_out)
{
  res_out.items.resize(reply.items.size());
  for (int i = 0; i < reply.items.size(); ++i)
  {
    industrial_msgs::IOReadReplyItem item;
    item.type = reply.items[i].type;
    item.index = reply.items[i].index;
    item.result = reply.items[i].result;
    item.value = reply.items[i].value;
    res_out.items[i] = item;
  }
}


}
