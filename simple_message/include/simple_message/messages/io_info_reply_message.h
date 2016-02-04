#ifndef IO_INFO_REPLY_MESSAGE_H
#define IO_INFO_REPLY_MESSAGE_H

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
namespace io_info_reply_message
{

const int msg_type = 65200;

struct IOInfoReplyItem
{
  industrial::shared_types::shared_int type;
  industrial::shared_types::shared_int start;
  industrial::shared_types::shared_int len;
  industrial::shared_types::shared_int feat_mask;
  
  unsigned int byteLength()
  {
    return 4 * sizeof(industrial::shared_types::shared_int);
  }
};

class IOInfoReplyMessage : public industrial::typed_message::TypedMessage
{
public:
  /**
   * \brief Default constructor
   *
   * This method creates an empty message.
   *
   */
  IOInfoReplyMessage(void);
  /**
   * \brief Destructor
   *
   */
  ~IOInfoReplyMessage(void);
  /**
   * \brief Initializes message from a simple message
   *
   * \param simple message to construct from
   *
   * \return true if message successfully initialized, otherwise false
   */
  bool init(industrial::simple_message::SimpleMessage & msg);

  /**
   * \brief Initializes a new io info reply message
   *
   */
  void init();

  // Overrides - SimpleSerialize
  bool load(industrial::byte_array::ByteArray *buffer);
  bool unload(industrial::byte_array::ByteArray *buffer);

  unsigned int byteLength()
  {
    unsigned int size = 3 * sizeof(industrial::shared_types::shared_int);
    for(int i = 0; i < items.size(); ++i)
    {
      IOInfoReplyItem& item = items[i];
      size += item.byteLength();
    }
    return size;
  }

  industrial::shared_types::shared_int message_id;
  industrial::shared_types::shared_int ctrlr_feat_mask;
  std::vector<IOInfoReplyItem> items;

};

}
}

#endif
