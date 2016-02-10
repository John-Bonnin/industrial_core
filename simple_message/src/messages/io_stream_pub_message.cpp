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
#include "simple_message/messages/io_stream_pub_message.h"
#include "simple_message/joint_data.h"
#include "simple_message/byte_array.h"
#include "simple_message/log_wrapper.h"
#else
#include "io_stream_pub_message.h"
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
namespace io_stream_pub_message
{

IOStreamPubMessage::IOStreamPubMessage(void)
{
  this->init();
}

IOStreamPubMessage::~IOStreamPubMessage(void)
{

}

bool IOStreamPubMessage::init(industrial::simple_message::SimpleMessage & msg)
{
  ByteArray data = msg.getData();

  this->init();
  return this->unload(&data);
}

void IOStreamPubMessage::init()
{
  this->setMessageType(msg_type);
  this->timestamp = 0;
}

bool IOStreamPubMessage::load(ByteArray *buffer)
{
  throw std::runtime_error("Not implemented");
}

bool IOStreamPubMessage::unload(ByteArray *buffer)
{
  bool rtn = true;
  LOG_COMM("Executing io stream pub message unload");

  rtn &= buffer->unloadFront(this->timestamp);
  if (!rtn) return rtn;
  industrial::shared_types::shared_int num_items; 
  rtn &= buffer->unloadFront(num_items);
  if (!rtn) return rtn;
  
  this->items.resize(num_items);
  for (int i = 0; i < num_items; i++) {
    IOStreamPubItem item;
    
    rtn &= buffer->unloadFront(item.type);
    if (!rtn) return rtn;
    
    rtn &= buffer->unloadFront(item.start);
    if (!rtn) return rtn;
    
    industrial::shared_types::shared_int num_values;
    rtn &= buffer->unloadFront(num_values);
    if (!rtn) return rtn;
    
    item.values.resize(num_values);
    for (int j = 0; j < num_values; j++) {
      industrial::shared_types::shared_int value;
      
      rtn &= buffer->unloadFront(value);
      if (!rtn) return rtn;
      
      item.values[j] = value;
    }

    this->items[i] = item;
  }
  
  return rtn;
}

}
}

