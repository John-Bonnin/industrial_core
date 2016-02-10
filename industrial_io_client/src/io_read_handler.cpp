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
