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

#ifndef IO_STREAM_SUBSCRIBER_H
#define IO_STREAM_SUBSCRIBER_H

#include "simple_message/smpl_msg_connection.h"
#include "simple_message/message_handler.h"
#include "simple_message/messages/io_stream_sub_reply_message.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"

class IOStreamSubscriber : public industrial::message_handler::MessageHandler
{
 // since this class defines a different init(), this helps find the base-class init()
 using industrial::message_handler::MessageHandler::init;
public:
  bool subscribeToRangesFromParameters();

  bool init(industrial::smpl_msg_connection::SmplMsgConnection* connection);

private:

  /*!
   * Tries to convert a simple message to the expected type
   */
  bool internalCB(industrial::simple_message::SimpleMessage& in);

  /*!
   * Handles simple messages of the expected type
   */
  bool internalCB(industrial::io_stream_sub_reply_message::IOStreamSubReplyMessage& inputMessage);

  boost::mutex replyMutex; //! Mutex to protect access to lastReplyMessage
  boost::condition_variable newReplyConditionVariable; //! Condition variable to notify of new reply message
  industrial::io_stream_sub_reply_message::IOStreamSubReplyMessage lastReplyMessage;
  bool lastReplyMessageHasId(industrial::shared_types::shared_int id)
  {
    return lastReplyMessage.message_id == id;
  }
};

#endif
