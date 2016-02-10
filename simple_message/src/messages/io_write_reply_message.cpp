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

#ifndef FLATHEADERS
#include "simple_message/messages/io_write_reply_message.h"
#include "simple_message/joint_data.h"
#include "simple_message/byte_array.h"
#include "simple_message/log_wrapper.h"
#else
#include "io_write_reply_message.h"
#include "joint_data.h"
#include "byte_array.h"
#include "log_wrapper.h"
#endif

using namespace industrial::shared_types;
using namespace industrial::byte_array;
using namespace industrial::simple_message;
using namespace industrial::joint_data;

namespace industrial
{
namespace io_write_reply_message
{

IOWriteReplyMessage::IOWriteReplyMessage(void)
{
  this->init();
}

IOWriteReplyMessage::~IOWriteReplyMessage(void)
{

}

bool IOWriteReplyMessage::init(industrial::simple_message::SimpleMessage & msg)
{
  ByteArray data = msg.getData();

  this->init();
  return this->unload(&data);
}

void IOWriteReplyMessage::init()
{
  this->setMessageType(msg_type);
  this->message_id = 0;
  this->timestamp = 0;
}

bool IOWriteReplyMessage::load(ByteArray *buffer)
{
  throw std::runtime_error("Not implemented");
}

bool IOWriteReplyMessage::unload(ByteArray *buffer)
{
  bool rtn = true;
  LOG_COMM("Executing io write reply message unload");
  rtn &= buffer->unloadFront(message_id);
  if (!rtn) return rtn;
  rtn &= buffer->unloadFront(timestamp);
  if (!rtn) return rtn;
  
  industrial::shared_types::shared_int size;
  rtn &= buffer->unloadFront(size);
  if (!rtn) return rtn;
  
  items.resize(size);
  for (int i = 0; i < size; ++i)
  {
    IOWriteReplyItem item;
    rtn &= buffer->unloadFront(item.type);
    if (!rtn) return rtn;
    rtn &= buffer->unloadFront(item.index);
    if (!rtn) return rtn;
    rtn &= buffer->unloadFront(item.result);
    if (!rtn) return rtn;
    
    items[i] = item;
  }
  
  return rtn;
}

}
}
