#include "industrial_io_client/io_input_handler.h"
#include <iostream>

namespace industrial_io_client
{
bool IOInputHandler::internalCB(industrial::simple_message::SimpleMessage& in)
{
  industrial::io_stream_pub_message::IOStreamPubMessage inputMessage;
  if (!inputMessage.init(in))
  {
    std::cout << "Could not parse input message in io_input_handler" << std::endl;
    return false;
  }
  return internalCB(inputMessage);
}

bool industrial_io_client::IOInputHandler::internalCB(industrial::io_stream_pub_message::IOStreamPubMessage& inputMessage)
{
  std::cout << "Got input message in io_input_handler" << std::endl;
  std::cout << "Timestamp: " << inputMessage.timestamp << std::endl;
  for (int i = 0; i < inputMessage.items.size(); ++i)
  {
    industrial::io_stream_pub_message::IOStreamPubItem& item = inputMessage.items[i];
    std::cout << "Item: Type: " << item.type << " Start: " << item.start << " Len: " << item.values.size() << std::endl;
    for (int j = 0; j < item.values.size(); ++j)
    {
      industrial::shared_types::shared_int& value = item.values[j];
      std::cout << "  Value: " << value << std::endl;
    }
  }

  return true;
}

bool IOInputHandler::init(industrial::smpl_msg_connection::SmplMsgConnection* connection)
{
  return init(industrial::io_stream_pub_message::msg_type, connection);
}

}
