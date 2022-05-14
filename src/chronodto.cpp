#include "chronodto.h"
#include "string.h"

namespace chronoscopist
{
    // chrmessage::chrmessage(const chrmessage &msg) : version(msg.version), type(msg.type)
    // {
    //     strncpy(text, msg.text, sizeof(chronoscopist::messagetype)-1);
    //     text[sizeof(text)-1] = 0;
    // }

    chrmessage chrmessage::generate_message(const chronoscopist::messagetype msgtype, const char* text)
    {
            chronoscopist::chrmessage msg;
            msg.type = msgtype;
            msg.version = VERSION_1;
            strncpy(msg.text, text, sizeof(chronoscopist::chrmessage::text)-1);
            msg.text[sizeof(chronoscopist::chrmessage::text)-1] = 0;
            return msg;
    }
};
