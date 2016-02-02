#ifndef IO_SERVICE_HANDLER_H
#define IO_SERVICE_HANDLER_H

#include "simple_message/message_handler.h"
#include "ros/ros.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include <string>

namespace industrial_io_client
{

/*!
 * \class IOServiceHandler
 * \brief A base class for converting between ROS services and simple message requests/replys.
 * \tparam RosServiceType The type of the ROS service (without the trailing ::Request / ::Reply)
 * \tparam SimpleMessageRequestType The simple message request type
 * \tparam SimpleMessageReplyType The simple message reply type
 *
 * To implement it you have to set a service name and simple message type using the constructor
 * and override the rosRequestToSimpleMessage() and simpleMessageToRosReply() functions.
 */
template<typename RosServiceType, typename SimpleMessageRequestType, typename SimpleMessageReplyType>
class IOServiceHandler: public industrial::message_handler::MessageHandler
{
 // since this class defines a different init(), this helps find the base-class init()
 using industrial::message_handler::MessageHandler::init;
public:
 //Service name and message id should be set by the subclass
  /*!
  * \brief IOServiceHandler
  * \param serviceName Name for the ROS service. Will be prepended with "industrial_io_"
  * \param msg_type Message type of the simple messages
  *
  * Both params should be set fixed by the subclass.
  */
 IOServiceHandler(std::string serviceName, int msg_type) : serviceName(serviceName), msg_type(msg_type)
 {
   message_id_counter = 1; //Has to be larger than zero so that it is different from not intitalized ReplyMessage
 }

 /*!
  * \brief Initialize the handler
  * \param connection The connection to the simple message server
  * \return success
  *
  * Advertises the ROS service and initializes the handler.
  */
 bool init(industrial::smpl_msg_connection::SmplMsgConnection* connection)
 {
   ros::NodeHandle n;
   rosService = n.advertiseService("industrial_io_" + serviceName, &IOServiceHandler::serviceCallback, this);
   ROS_INFO_STREAM("Advertised io " << serviceName << " service");
   return init(msg_type, connection);
 }

protected:
 //These methods have to be implemented by the subclass
 /*!
  * \brief Convert a ROS request to a simple message request
  * \param req The ROS service request
  * \return the simple message
  *
  * Override this method in the subclass. You should use getNextMessageId() to get a unique id for the simple message.
  */
 virtual SimpleMessageRequestType rosRequestToSimpleMessage(typename RosServiceType::Request &req) = 0;

 /*!
  * \brief Convert a simple message reply to a ROS service response
  * \param[in] reply The simple message reply
  * \param[out] res_out The ROS reply created from the simple message
  *
  * Override this method in the subclass.
  */
 virtual void simpleMessageToRosReply(SimpleMessageReplyType& reply, typename RosServiceType::Response &res_out) = 0;


 /*!
  * \brief Get the next unique message id
  * \return the message id
  */
 int getNextMessageId()
 {
   return message_id_counter++;
 }

private:
 /*!
  * \brief Callback for the ROS service
  * \param req Service request
  * \param res Service response
  * \return success
  *
  * Converts the ROS service request to a simple message request using rosRequestToSimpleMessage(),
  * sends it to the simple message server,
  * waits for the reply with the correct message id
  * and converts that back to a ROS service response using simpleMessageToRosReply()
  */
 bool serviceCallback(typename RosServiceType::Request &req, typename RosServiceType::Response &res)
 {
   ROS_INFO_STREAM(serviceName << " service called");

   //Convert ros request to simple message
   SimpleMessageRequestType message = rosRequestToSimpleMessage(req);

   //Send simple message
   if (!getConnection()->isConnected())
   {
     return false;
   }
   industrial::simple_message::SimpleMessage simMess;
   message.toRequest(simMess);
   getConnection()->sendMsg(simMess);
   ROS_INFO_STREAM(serviceName << " request message sent");

   //Wait for reply
   boost::mutex::scoped_lock lock(replyMutex);
   if (newReplyConditionVariable.wait_for(
         lock, boost::chrono::seconds(1),
         boost::bind(&IOServiceHandler::lastReplyMessageHasId, this, message.message_id)))
   {
     ROS_INFO_STREAM("Got correct reply message id " << lastReplyMessage.message_id);

     //Convert simple message to ros reply
     simpleMessageToRosReply(lastReplyMessage, res);

     return true;
   }
   ROS_ERROR_STREAM("Timeout - Did not receive " << serviceName << " reply message");
   return false;
 }

 /*!
  * Tries to convert a simple message to the expected type
  */
 bool internalCB(industrial::simple_message::SimpleMessage& in)
 {
   SimpleMessageReplyType inputMessage;
   if (!inputMessage.init(in))
   {
     ROS_ERROR_STREAM("Could not parse reply message in " << serviceName << " reply_handler");
     return false;
   }
   return internalCB(inputMessage);
 }

 /*!
  * Handles simple messages of the expected type
  */
 bool internalCB(SimpleMessageReplyType& inputMessage)
 {
   ROS_INFO_STREAM("Got input message in " << serviceName << " service handler - id: " << inputMessage.message_id);
   boost::mutex::scoped_lock lock(replyMutex);

   lastReplyMessage = inputMessage;
   newReplyConditionVariable.notify_one();
   return true;
 }

 int msg_type; //! The simple message type

 std::string serviceName;
 ros::ServiceServer rosService;

 boost::mutex replyMutex; //! Mutex to protect access to lastReplyMessage
 boost::condition_variable newReplyConditionVariable; //! Condition variable to notify of new reply message
 SimpleMessageReplyType lastReplyMessage;

 industrial::shared_types::shared_int message_id_counter;
 bool lastReplyMessageHasId(industrial::shared_types::shared_int id)
 {
   return lastReplyMessage.message_id == id;
 }



};
}

#endif // IO_SERVICE_HANDLER_H
