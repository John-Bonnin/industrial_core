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

#include "industrial_io_client/io_input_handler.h"
#include <iostream>

namespace industrial_io_client
{
bool IOInputHandler::internalCB(industrial::simple_message::SimpleMessage& in)
{
  industrial::io_stream_pub_message::IOStreamPubMessage inputMessage;
  if (!inputMessage.init(in))
  {
    std::cout << "Could not parse input message in io_input_handler" << std::endl;
    return false;
  }
  return internalCB(inputMessage);
}

bool industrial_io_client::IOInputHandler::internalCB(industrial::io_stream_pub_message::IOStreamPubMessage& inputMessage)
{
  std::cout << "Got input message in io_input_handler" << std::endl;
  std::cout << "Timestamp: " << inputMessage.timestamp << std::endl;
  for (int i = 0; i < inputMessage.items.size(); ++i)
  {
    industrial::io_stream_pub_message::IOStreamPubItem& item = inputMessage.items[i];
    std::cout << "Item: Type: " << item.type << " Start: " << item.start << " Len: " << item.values.size() << std::endl;
    for (int j = 0; j < item.values.size(); ++j)
    {
      industrial::shared_types::shared_int& value = item.values[j];
      std::cout << "  Value: " << value << std::endl;
    }
  }

  return true;
}

bool IOInputHandler::init(industrial::smpl_msg_connection::SmplMsgConnection* connection)
{
  return init(industrial::io_stream_pub_message::msg_type, connection);
}

}
