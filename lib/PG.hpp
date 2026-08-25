#pragma once

#include <libpq-fe.h>
#include <VM/VM.hpp>

namespace fer
{

class VarPGConn : public Var
{
    PGconn *val;

    void onDestroy(VirtualMachine &vm) override;

public:
    VarPGConn(ModuleLoc loc, PGconn *conn);

    inline const char *getUser() const { return PQuser(val); }
    inline const char *getDb() const { return PQdb(val); }
    inline const char *getPass() const { return PQpass(val); }

    inline int getVersion() const { return PQserverVersion(val); }

    inline int getStatus() const { return PQstatus(val); }
    inline const char *getErrorMsg() const { return PQerrorMessage(val); }

    inline PGconn *getVal() { return val; }
};

} // namespace fer