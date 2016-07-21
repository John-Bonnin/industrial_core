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

#include <iostream>
#include "industrial_io_client/io_stream_pub_handler.h"
#include "industrial_io_client/io_read_handler.h"
#include "industrial_io_client/io_write_handler.h"
#include "industrial_io_client/io_info_handler.h"
#include "industrial_io_client/io_stream_subscriber.h"
#include "simple_message/message_manager.h"
#include "simple_message/socket/tcp_client.h"
#include "ros/ros.h"
#include "boost/thread.hpp"
#include <string>

using namespace std;
using namespace industrial_io_client;
using namespace industrial::message_manager;
using namespace industrial::tcp_client;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "industrial_io_node");
  ros::NodeHandle n;

  string robot_ip_address;

  if (!n.getParam("robot_ip_address", robot_ip_address)) {
      LOG_WARN("Did not get robot_ip_address param. Using 127.0.0.1.");
      robot_ip_address = "127.0.0.1";
  }

  //Port for all service calls
  int servicePort;
  if (!n.getParam("service_port", servicePort)) {
    LOG_WARN("Did not get service_port param. Using 11003");
    servicePort = 11003;
  }

  //Port for the streaming topic coming from the server
  int streamingPort;
  if (!n.getParam("streaming_port", streamingPort)) {
    LOG_WARN("Did not get streaming_port. Using 11003");
    streamingPort = 11003;
  }

  bool useSeparateStreamingPort = servicePort != streamingPort;
  ROS_INFO_STREAM("Use separate streaming port " << useSeparateStreamingPort);
  
  TcpClient default_tcp_connection_;
  default_tcp_connection_.init(const_cast<char*>(robot_ip_address.c_str()), servicePort);

  TcpClient streaming_tcp_connection_;
  if (useSeparateStreamingPort) {
    streaming_tcp_connection_.init(const_cast<char*>(robot_ip_address.c_str()), streamingPort);
  }

  IOReadHandler readHandler;
  readHandler.init(&default_tcp_connection_);

  IOWriteHandler writeHandler;
  writeHandler.init(&default_tcp_connection_);

  IOInfoHandler infoHandler;
  infoHandler.init(&default_tcp_connection_);

  IOStreamSubscriber streamSubscriber;
  bool streamSubscriptionSent = false;
  streamSubscriber.init(&default_tcp_connection_);
  
  MessageManager defaultMessageManager;
  defaultMessageManager.init(&default_tcp_connection_);
  defaultMessageManager.add(&readHandler);
  defaultMessageManager.add(&writeHandler);
  defaultMessageManager.add(&infoHandler);
  defaultMessageManager.add(&streamSubscriber);

  MessageManager streamingMessageManager;
  IOStreamPubHandler streamPubHandler;
  if (useSeparateStreamingPort) {
    streamingMessageManager.init(&streaming_tcp_connection_);
    streamPubHandler.init(&streaming_tcp_connection_);
    streamingMessageManager.add(&streamPubHandler);
  } else {
    streamPubHandler.init(&default_tcp_connection_);
    defaultMessageManager.add(&streamPubHandler);
  }

  LOG_INFO("IO Node setup done");

  boost::thread defaultManagerThread(boost::bind(&MessageManager::spin, &defaultMessageManager));
  boost::thread streamingManagerThread;
  if (useSeparateStreamingPort) {
    streamingManagerThread = boost::thread(boost::bind(&MessageManager::spin, &streamingMessageManager));
  }
  
  ros::Rate r(30);
  while(ros::ok()) {

    //Wait for the connection to be established and send subscription
    if (default_tcp_connection_.isConnected() && !streamSubscriptionSent) {
      streamSubscriptionSent = true;
      if (!streamSubscriber.subscribeToRangesFromParameters())
      {
        ROS_ERROR("Could not subscribe to io ranges");
      }
    }
    //Resend subscriptions if connection is lost
    if (!default_tcp_connection_.isConnected()) {
      streamSubscriptionSent = false;
    }

    ros::spinOnce();
    r.sleep();
  }
  LOG_INFO("IO Node ended");
  
  return 0;
}
