#ifndef CONNECTIONAWARETRANSACTION_HH
#define CONNECTIONAWARETRANSACTION_HH

#include <memory>

#include "nanodbc/nanodbc.h"

#include "UVMonitor.hh"
#include "nctypes.hh"

#include "errors.hh"


namespace NC {

using NC::UVMonitor;


class ConnectionAwareTransaction final {
    std::shared_ptr<UVMonitor<nanodbc::connection>> connection;
    std::unique_ptr<nanodbc::transaction> transaction;

    void CheckTransaction() {
        if (!transaction) {
            throw Error("Transaction not initialized");
        }
    }

    template<bool committed>
    void DoFinish() {
        connection->Synchronized([&] {
            CheckTransaction();

            // free transaction at the end of the block
            std::unique_ptr<nanodbc::transaction> finished_tx {
                std::move(transaction) };
            if (committed) {
                finished_tx->commit();
            } else {
                finished_tx->rollback();
            }
        });
    }

 public:
    ConnectionAwareTransaction(
            std::shared_ptr<UVMonitor<nanodbc::connection>> m) :
        connection(m),
        transaction() {
    }

    ConnectionAwareTransaction(const ConnectionAwareTransaction&) = delete;
    ConnectionAwareTransaction(ConnectionAwareTransaction&&) = delete;
    virtual ~ConnectionAwareTransaction() = default;

    void Begin() {
        connection->Synchronized([&](const nanodbc::connection& c) {
            transaction.reset(new nanodbc::transaction(c));
        });
    }

    void Commit() {
        DoFinish<true>();
    }

    void Rollback() {
        DoFinish<false>();
    }

};

}  // namespace NC

#endif /* CONNECTIONAWARETRANSACTION_HH */
