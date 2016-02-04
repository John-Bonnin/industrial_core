#ifndef IO_INFO_REQUEST_MESSAGE_H
#define IO_INFO_REQUEST_MESSAGE_H

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

namespace industrial
{
namespace io_info_request_message
{

const int msg_type = 65200;

class IOInfoRequestMessage : public industrial::typed_message::TypedMessage
{
public:
  /**
   * \brief Default constructor
   *
   * This method creates an empty message.
   *
   */
  IOInfoRequestMessage(void);
  /**
   * \brief Destructor
   *
   */
  ~IOInfoRequestMessage(void);
  /**
   * \brief Initializes message from a simple message
   *
   * \param simple message to construct from
   *
   * \return true if message successfully initialized, otherwise false
   */
  bool init(industrial::simple_message::SimpleMessage & msg);

  /**
   * \brief Initializes a io info request message
   *
   */
  void init();

  // Overrides - SimpleSerialize
  bool load(industrial::byte_array::ByteArray *buffer);
  bool unload(industrial::byte_array::ByteArray *buffer);

  unsigned int byteLength()
  {
    unsigned int size = sizeof(industrial::shared_types::shared_int);
    return size;
  }

  industrial::shared_types::shared_int message_id;
};

}
}

#endif
