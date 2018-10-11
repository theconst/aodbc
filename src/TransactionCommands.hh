#ifndef TRANSACTIONCOMMANDS_HH
#define TRANSACTIONCOMMANDS_HH

namespace NC {

enum struct TransactionCommands {
    commit,
    rollback,
    begin
};

template<TransactionCommands tag>
struct TransactionMethodTag {
    static constexpr TransactionCommands value = tag;
};

}  // namespace NC

#endif  /* TRANSACTIONCOMMANDS_HH */
