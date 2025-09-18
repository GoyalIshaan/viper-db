#include "uuid.h"

std::string generateUUID() {
    uuid_t uuid;
    char uuid_str[37];

    uuid_generate(uuid);
    uuid_unparse(uuid, uuid_str);

    return std::string(uuid_str);
}