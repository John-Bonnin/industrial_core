/*!
This node filters a single io input value from the industrial_io stream_pub topic
and publishes the value on a separate topic.
You have to set the io type and index via the io_type and io_index params,
and the output message type (int/bool) via the output_type param.
*/

#include "ros/ros.h"
#include "industrial_msgs/IOStreamPub.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Bool.h"

ros::Publisher publisher;
ros::Subscriber subscriber;

int ioType;
int ioIndex;

enum OUTPUT_TYPE {
  INT,
  BOOL
};

OUTPUT_TYPE output_type;

void handle(industrial_msgs::IOStreamPub::ConstPtr msg) {
  for(int i = 0; i < msg->items.size(); ++i) {
    industrial_msgs::IOStreamPubItem item = msg->items[i];
    if (item.type == ioType) {
      //Item is positioned in list relative to start
      int index = ioIndex - item.start;
      if (index >= 0 && index < item.values.size()) {
        if (output_type == INT) {
          std_msgs::Int32 outMsg;
          outMsg.data = item.values[index];
          publisher.publish(outMsg);
        } else if (output_type == BOOL) {
          std_msgs::Bool outMsg;
          outMsg.data = item.values[index] > 0;
          publisher.publish(outMsg);
        }
      }
    }
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "io_stream_filter");
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

  std::string output_type_str;
  if (!ph.getParam("output_type", output_type_str)) {
    ROS_ERROR("No output_type parameter given. Possible values are bool, int.");
    return 1;
  }

  if (output_type_str == "int") {
    output_type = INT;
  } else if (output_type_str == "bool") {
    output_type = BOOL;
  } else {
    ROS_ERROR_STREAM("Unknown output type " << output_type_str << ". Possible values are bool, int;");
  }

  subscriber = nh.subscribe("industrial_io_stream_pub", 10, handle);
  if (output_type == INT) {
    publisher = nh.advertise<std_msgs::Int32>("filtered_io", 10);
  } else if (output_type == BOOL) {
    publisher = nh.advertise<std_msgs::Bool>("filtered_io", 10);
  }

  ros::spin();

  return 0;
}
