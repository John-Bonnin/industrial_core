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

#include "industrial_io_client/io_stream_subscriber.h"
#include "ros/ros.h"
#include "simple_message/messages/io_stream_sub_request_message.h"
#include "XmlRpcException.h"

bool IOStreamSubscriber::subscribeToRangesFromParameters()
{
  ros::NodeHandle nh("~");
  XmlRpc::XmlRpcValue subscribe_ranges;

  if (!nh.getParam("subscribe_ranges", subscribe_ranges))
  {
    ROS_ERROR("Could not load subscribe_ranges parameter");
    return false;
  }

  industrial::io_stream_sub_request_message::IOStreamSubRequestMessage requestMessage;

  try
  {
    ROS_INFO_STREAM("Got " << subscribe_ranges.size() << " io ranges to subscribe to from parameters");

    requestMessage.items.resize(subscribe_ranges.size());
    requestMessage.message_id = 123;

    for (int i = 0; i < subscribe_ranges.size(); ++i)
    {
      XmlRpc::XmlRpcValue range = subscribe_ranges[i];

      int type = range["type"];
      int start = range["start"];
      int len = range["len"];

      ROS_INFO_STREAM("Range: Type: " << type << ", Start: " << start << ", Len: " << len);

      requestMessage.items[i].type = type;
      requestMessage.items[i].start = start;
      requestMessage.items[i].len = len;
    }
  }
  catch (XmlRpc::XmlRpcException& ex)
  {
    ROS_ERROR("subscribe_ranges in invalid format");
    return false;
  }

  //Send simple message
  if (!getConnection()->isConnected())
  {
    ROS_ERROR("Connection is not connected");
    return false;
  }

  industrial::simple_message::SimpleMessage simMess;
  requestMessage.toRequest(simMess);
  getConnection()->sendMsg(simMess);
  ROS_INFO("io stream sub request message sent");

  //Wait for reply
  boost::mutex::scoped_lock lock(replyMutex);
  if (newReplyConditionVariable.wait_for(
        lock, boost::chrono::seconds(1),
        boost::bind(&IOStreamSubscriber::lastReplyMessageHasId, this, requestMessage.message_id)))
  {
    bool success = true;
    for (int i = 0; i < lastReplyMessage.items.size(); ++i)
    {
      if (lastReplyMessage.items[i].result != 1) {
        ROS_WARN_STREAM("Could not subscribe to range type: " << lastReplyMessage.items[i].type <<
                        ", start: " << lastReplyMessage.items[i].start <<
                        " because of error " << lastReplyMessage.items[i].result);
        success = false;
      }
    }

    if (success) {
      ROS_INFO("Successfully subscribed to io ranges");
    }

    return success;
  }
  ROS_ERROR_STREAM("Timeout - Did not receive io stream sub reply message");
  return false;
}

bool IOStreamSubscriber::init(industrial::smpl_msg_connection::SmplMsgConnection *connection)
{
  return init(industrial::io_stream_sub_request_message::msg_type, connection);
}

bool IOStreamSubscriber::internalCB(industrial::io_stream_sub_reply_message::IOStreamSubReplyMessage &inputMessage)
{
  ROS_INFO("Got reply message in io stream subscriber reply_handler");
  boost::mutex::scoped_lock lock(replyMutex);

  lastReplyMessage = inputMessage;
  newReplyConditionVariable.notify_one();
  return true;
}

bool IOStreamSubscriber::internalCB(industrial::simple_message::SimpleMessage &in)
{
  industrial::io_stream_sub_reply_message::IOStreamSubReplyMessage inputMessage;
  if (!inputMessage.init(in))
  {
    ROS_ERROR("Could not parse reply message in io stream subscriber reply_handler");
    return false;
  }
  return internalCB(inputMessage);
}
