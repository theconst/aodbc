#ifndef STATEMENTCOMMANDS_HH
#define STATEMENTCOMMANDS_HH

namespace NC {

enum struct StatementCommands {
    prepare,
    execute,
    query,
    close
};

template<StatementCommands tag>
struct StatementMethodTag {
    static constexpr StatementCommands value = tag;
};

}

#endif  /* STATEMENTCOMMANDS_HH */
