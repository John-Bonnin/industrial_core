#ifndef IO_WRITE_REQUEST_MESSAGE_H
#define IO_WRITE_REQUEST_MESSAGE_H

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
namespace io_write_request_message
{

const int msg_type = 65202;

struct IOWriteRequestItem
{
  industrial::shared_types::shared_int type;
  industrial::shared_types::shared_int index;
  industrial::shared_types::shared_int value;
  
  unsigned int byteLength()
  {
    return 3 * sizeof(industrial::shared_types::shared_int);
  }
};

class IOWriteRequestMessage : public industrial::typed_message::TypedMessage
{
public:
  /**
   * \brief Default constructor
   *
   * This method creates an empty message.
   *
   */
  IOWriteRequestMessage(void);
  /**
   * \brief Destructor
   *
   */
  ~IOWriteRequestMessage(void);
  /**
   * \brief Initializes message from a simple message
   *
   * \param simple message to construct from
   *
   * \return true if message successfully initialized, otherwise false
   */
  bool init(industrial::simple_message::SimpleMessage & msg);

  /**
   * \brief Initializes a new io write request message
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
      IOWriteRequestItem& item = items[i];
      size += item.byteLength();
    }
    return size;
  }

  industrial::shared_types::shared_int message_id;
  std::vector<IOWriteRequestItem> items;

};

}
}

#endif
