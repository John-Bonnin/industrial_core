#include <iostream>
#include "industrial_io_client/io_input_handler.h"
#include "industrial_io_client/io_read_handler.h"
#include "industrial_io_client/io_write_handler.h"
#include "simple_message/message_manager.h"
#include "simple_message/socket/tcp_client.h"
#include "ros/ros.h"
#include "boost/thread.hpp"

using namespace std;
using namespace industrial_io_client;
using namespace industrial::message_manager;
using namespace industrial::tcp_client;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "industrial_io_node");
  ros::NodeHandle n;
  
  TcpClient default_tcp_connection_;
  default_tcp_connection_.init("127.0.0.1", 11003);
  
  IOInputHandler inputHandler;
  inputHandler.init(&default_tcp_connection_);

  IOReadHandler readHandler;
  readHandler.init(&default_tcp_connection_);

  IOWriteHandler writeHandler;
  writeHandler.init(&default_tcp_connection_);
  
  MessageManager messageManager;
  messageManager.init(&default_tcp_connection_);
  messageManager.add(&inputHandler);
  messageManager.add(&readHandler);
  messageManager.add(&writeHandler);

  LOG_INFO("IO Node setup done");

  boost::thread managerThread(boost::bind(&MessageManager::spin, &messageManager));
  
  ros::Rate r(30);
  while(ros::ok()) {
    ros::spinOnce();
    r.sleep();
  }
  LOG_INFO("IO Node ended");
  
  return 0;
}
