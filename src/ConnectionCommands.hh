#ifndef CONNECTIONCOMMANDS_HH
#define CONNECTIONCOMMANDS_HH

namespace NC {

enum struct ConnectionCommands {
    dbms_name,
    is_connected,
    dbms_version,
    catalog_name,
    database_name,
    driver_name,
    connect,
    disconnect,
    query,
    execute
};

template<ConnectionCommands tag>
struct ConnectionMethodTag {
    static constexpr ConnectionCommands value = tag;
};

}  // namespace NC


#endif /* CONNECTIONCOMMANDS_HH */
