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
