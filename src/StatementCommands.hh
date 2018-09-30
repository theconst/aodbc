#ifndef STATEMENTCOMMANDS_HH
#define STATEMENTCOMMANDS_HH

namespace NC {

enum struct StatementCommands {
    prepare,
    execute,
    query,
    close,
    is_open
};

template<StatementCommands tag>
struct StatementMethodTag {
    static constexpr StatementCommands value = tag;
};

}  // namespace NC

#endif  /* STATEMENTCOMMANDS_HH */
