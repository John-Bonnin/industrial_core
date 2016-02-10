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
