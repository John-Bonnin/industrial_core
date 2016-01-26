#include <iostream>
#include "industrial_io_client/io_input_handler.h"
#include "simple_message/message_manager.h"
#include "simple_message/socket/tcp_client.h"
#include "ros/ros.h"

using namespace std;
using namespace industrial_io_client;
using namespace industrial::message_manager;
using namespace industrial::tcp_client;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "industrial_io_node");
  
  TcpClient default_tcp_connection_;
  default_tcp_connection_.init("127.0.0.1", 11003);
  
  IOInputHandler inputHandler;
  inputHandler.init(&default_tcp_connection_);
  
  MessageManager messageManager;
  messageManager.init(&default_tcp_connection_);
  messageManager.add(&inputHandler);
  
  LOG_INFO("IO Node setup done");
  messageManager.spin();
  LOG_INFO("IO Node ended");
  
  return 0;
}