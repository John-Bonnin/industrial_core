#include "industrial_io_client/io_info_handler.h"
#include "simple_message/messages/io_info_request_message.h"
#include <iostream>
#include <boost/chrono.hpp>

namespace industrial_io_client
{

IOInfoHandler::IOInfoHandler() : IOServiceHandler("info", industrial::io_info_reply_message::msg_type)
{

}

industrial::io_info_request_message::IOInfoRequestMessage IOInfoHandler::rosRequestToSimpleMessage(industrial_msgs::IOInfo::Request &req)
{
  industrial::io_info_request_message::IOInfoRequestMessage message;
  message.message_id = getNextMessageId();
  return message;
}

void IOInfoHandler::simpleMessageToRosReply(industrial::io_info_reply_message::IOInfoReplyMessage& reply, industrial_msgs::IOInfo::Response &res_out)
{
  res_out.controller_local_timestamps_support = (reply.ctrlr_feat_mask >> 0) & 1;
  res_out.items.resize(reply.items.size());
  for (int i = 0; i < reply.items.size(); ++i)
  {
    industrial_msgs::IOInfoReplyItem item;
    item.type = reply.items[i].type;
    item.start = reply.items[i].start;
    item.len = reply.items[i].len;

    item.io_reset_support = (reply.items[i].feat_mask >> 0) & 1;
    item.io_streaming_support = (reply.items[i].feat_mask >> 1) & 1;

    res_out.items[i] = item;
  }
}


}
