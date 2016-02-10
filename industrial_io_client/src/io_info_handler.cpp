/*
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2016, FZI Karlsruhe
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 * 	notice, this list of conditions and the following disclaimer.
 * 	* Redistributions in binary form must reproduce the above copyright
 * 	notice, this list of conditions and the following disclaimer in the
 * 	documentation and/or other materials provided with the distribution.
 * 	* Neither the name of the Southwest Research Institute, nor the names
 *	of its contributors may be used to endorse or promote products derived
 *	from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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
