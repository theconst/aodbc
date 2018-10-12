#ifndef STATEMENTCOMMANDS_HH
#define STATEMENTCOMMANDS_HH

#include "ConnectionAwareStatement.hh"
#include "arguments.hh"

namespace NC {

using NC::ConnectionAwareStatement;

enum struct StatementCommands {
    prepare,
    execute,
    query,
    close,
    is_open
};

template<StatementCommands c>
struct StatementCommand {
};

template<>
struct StatementCommand<StatementCommands::prepare> {

    static nc_null_t Execute(ConnectionAwareStatement* owner,
          const std::tuple<nc_string_t>& args) {
        owner->Prepare(std::get<0>(args));
        return nc_null_t {};
    }
};

template<>
struct StatementCommand<StatementCommands::query> {

    static nc_result_t Execute(ConnectionAwareStatement* owner,
            const std::tuple<PreparedStatementArguments>& args) {
        const PreparedStatementArguments& psargs = std::get<0>(args);
        return owner->Query(psargs.GetBindings(), psargs.GetBatchSize(),
                psargs.GetTimeout());
    }
};

template<>
struct StatementCommand<StatementCommands::execute> {
    static nc_null_t Execute(
            ConnectionAwareStatement* owner,
            const std::tuple<PreparedStatementArguments>& args) {
        const PreparedStatementArguments& psargs = std::get<0>(args);
        owner->Execute(psargs.GetBindings(), psargs.GetBatchSize(),
        psargs.GetTimeout());
        return nc_null_t {};
    }
};

template<>
struct StatementCommand<StatementCommands::close> {
    static nc_null_t Execute(ConnectionAwareStatement* owner,
            const std::tuple<>&) {
        owner->Close();
        return nc_null_t {};
    }
};

template<>
struct StatementCommand<StatementCommands::is_open> {

    static bool Execute(ConnectionAwareStatement* owner,
            const std::tuple<>&) {
        return owner->IsOpen();
    }
};

}  // namespace NC

#endif  /* STATEMENTCOMMANDS_HH */
