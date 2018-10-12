#ifndef TRANSACTIONCOMMANDS_HH
#define TRANSACTIONCOMMANDS_HH

#include "nctypes.hh"
#include "ConnectionAwareTransaction.hh"

namespace NC {

enum struct TransactionCommands {
    commit,
    rollback,
    begin
};

template<TransactionCommands c>
struct TransactionCommand {
};

template<>
struct TransactionCommand<TransactionCommands::begin> {
    static nc_null_t Execute(ConnectionAwareTransaction* owner,
            const std::tuple<>&) {
        owner->Begin();
        return nc_null_t {};
    }
};

template<>
struct TransactionCommand<TransactionCommands::commit> {
    static nc_null_t Execute(ConnectionAwareTransaction* owner,
            const std::tuple<>&) {
        owner->Commit();
        return nc_null_t {};
    }
};

template<>
struct TransactionCommand<TransactionCommands::rollback> {
    static nc_null_t Execute(ConnectionAwareTransaction* owner,
            const std::tuple<>&) {
        owner->Rollback();
        return nc_null_t {};
    }
};

}  // namespace NC

#endif  /* TRANSACTIONCOMMANDS_HH */
