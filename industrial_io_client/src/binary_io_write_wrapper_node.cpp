/*!
  This node translates service calls to the on and off service
  to industrial_io binary output on a port specified via parameters (io_type, io_index).
  You can then remap on and off to something meaningful in your situation.
*/
#include "ros/ros.h"
#include "std_srvs/Trigger.h"
#include "industrial_msgs/IOWrite.h"

int ioType;
int ioIndex;

bool handle(std_srvs::Trigger::Request& req, std_srvs::Trigger::Response& res, int value) {
  ros::NodeHandle nh;
  ros::ServiceClient client = nh.serviceClient<industrial_msgs::IOWrite>("industrial_io_write");

  industrial_msgs::IOWrite io_write;
  industrial_msgs::IOWriteRequestItem io_req_item;
  io_req_item.type = ioType;
  io_req_item.index = ioIndex;
  io_req_item.value = value;
  io_write.request.items.push_back(io_req_item);

  if (client.call(io_write)) {
    if (io_write.response.items.size() == 1)
    {
      res.success = io_write.response.items[0].result == 1;
      std::stringstream ss;
      ss << "io write response code: " << io_write.response.items[0].result;
      res.message = ss.str();
    }
    else
    {
      res.success = false;
      res.message = "Industrial io write answered with incorrect number of reply items";
    }
  }
  else
  {
    res.success = false;
    res.message = "Service call returned false";
  }
  return true;
}

bool on(std_srvs::Trigger::Request& req, std_srvs::Trigger::Response& res)
{
  handle(req, res, 1);
}

bool off(std_srvs::Trigger::Request& req, std_srvs::Trigger::Response& res)
{
  handle(req, res, 0);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "binary_io_write_wrapper");
  ros::NodeHandle nh;
  ros::NodeHandle ph("~");

  if (!ph.getParam("io_type", ioType)) {
    ROS_ERROR("No io_type parameter given");
    return 1;
  }

  if (!ph.getParam("io_index", ioIndex)) {
    ROS_ERROR("No io_index parameter given");
    return 1;
  }

  ros::ServiceServer onServer = nh.advertiseService("on", on);
  ros::ServiceServer offServer = nh.advertiseService("off", off);

  ros::spin();

  return 0;
}
