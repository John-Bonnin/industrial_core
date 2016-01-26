#ifndef IO_STREAM_PUB_MESSAGE_H
#define IO_STREAM_PUB_MESSAGE_H

#ifndef FLATHEADERS
#include "simple_message/typed_message.h"
#include "simple_message/simple_message.h"
#include "simple_message/shared_types.h"
#include "simple_message/joint_data.h"
#else
#include "typed_message.h"
#include "simple_message.h"
#include "shared_types.h"
#include "joint_data.h"
#endif
#include <vector>

namespace industrial
{
namespace io_stream_pub_message
{

const int msg_type = 65206;

struct IOStreamPubItem
{
  industrial::shared_types::shared_int type;
  industrial::shared_types::shared_int start;
  std::vector<industrial::shared_types::shared_int> values;
  
  unsigned int byteLength()
  {
    return (2 + values.size()) * sizeof(industrial::shared_types::shared_int);
  }
};

class IOStreamPubMessage : public industrial::typed_message::TypedMessage
{
public:
  /**
   * \brief Default constructor
   *
   * This method creates an empty message.
   *
   */
  IOStreamPubMessage(void);
  /**
   * \brief Destructor
   *
   */
  ~IOStreamPubMessage(void);
  /**
   * \brief Initializes message from a simple message
   *
   * \param simple message to construct from
   *
   * \return true if message successfully initialized, otherwise false
   */
  bool init(industrial::simple_message::SimpleMessage & msg);

  /**
   * \brief Initializes a new io stream pub message
   *
   */
  void init();

  // Overrides - SimpleSerialize
  bool load(industrial::byte_array::ByteArray *buffer);
  bool unload(industrial::byte_array::ByteArray *buffer);

  unsigned int byteLength()
  {
    unsigned int size = 2 * sizeof(industrial::shared_types::shared_int);
    for(int i = 0; i < items.size(); ++i)
    {
      IOStreamPubItem& item = items[i];
      size += item.byteLength();
    }
    return size;
  }

  industrial::shared_types::shared_int timestamp;
  std::vector<IOStreamPubItem> items;

};

}
}

#endif
