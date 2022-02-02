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

#ifndef IO_TOPIC_HANDLER_H
#define IO_TOPIC_HANDLER_H

#include "simple_message/message_handler.h"
#include "ros/ros.h"
#include <string>

namespace industrial_io_client
{

/*!
 * \class IOTopicHandler
 * \brief A base class for converting between simple message topics and ROS topics.
 * \tparam RosTopicType The type of the ROS topic
 * \tparam SimpleMessageType The simple message type
 *
 * To implement it you have to set a topic name and simple message type using the constructor
 * and override the simpleMessageToRosMessage() functions.
 */
template<typename RosMessageType, typename SimpleMessageType>
class IOTopicHandler: public industrial::message_handler::MessageHandler
{
 // since this class defines a different init(), this helps find the base-class init()
 using industrial::message_handler::MessageHandler::init;
public:
 //Topic name and message id should be set by the subclass
  /*!
  * \brief IOTopicHandler
  * \param topicName Name for the ROS topic. Will be prepended with "industrial_io_"
  * \param msg_type Message type of the simple messages
  *
  * Both params should be set fixed by the subclass.
  */
 IOTopicHandler(std::string topicName, int msg_type) : topicName(topicName), msg_type(msg_type)
 {
 }

 /*!
  * \brief Initialize the handler
  * \param connection The connection to the simple message server
  * \return success
  *
  * Advertises the ROS topic and initializes the handler.
  */
 bool init(industrial::smpl_msg_connection::SmplMsgConnection* connection)
 {
   ros::NodeHandle n;
   rosPublisher = n.advertise<RosMessageType>("industrial_io_" + topicName, 5);
   ROS_INFO_STREAM("Advertised io " << topicName << " topic");
   return init(msg_type, connection);
 }

protected:
 /*!
  * \brief Convert a simple message to a ROS message
  * \param[in] input The simple message
  * \return the ros message
  *
  * Override this method in the subclass.
  */
 virtual RosMessageType simpleMessageToRosMessage(SimpleMessageType& input) = 0;

private:

 /*!
  * Tries to convert a simple message to the expected type
  */
 bool internalCB(industrial::simple_message::SimpleMessage& in)
 {
   SimpleMessageType inputMessage;
   if (!inputMessage.init(in))
   {
     ROS_ERROR_STREAM("Could not parse message in " << topicName << " handler");
     return false;
   }
   return internalCB(inputMessage);
 }

 /*!
  * Handles simple messages of the expected type
  */
 bool internalCB(SimpleMessageType& inputMessage)
 {
   ROS_DEBUG_STREAM("Got input message in " << topicName << " topic handler");
   RosMessageType rosMessage = simpleMessageToRosMessage(inputMessage);
   rosPublisher.publish(rosMessage);
   return true;
 }

 int msg_type; //! The simple message type

 std::string topicName;
 ros::Publisher rosPublisher;



};
}

#endif // IO_TOPIC_HANDLER_H
