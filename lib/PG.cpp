#include "PG.hpp"

namespace fer
{

//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// VarPGConn /////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

VarPGConn::VarPGConn(ModuleLoc loc, PGconn *conn) : Var(loc), val(conn) {}

void VarPGConn::onDestroy(VirtualMachine &vm) { PQfinish(val); }

//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// Functions /////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

FERAL_FUNC(pqGetVersionNative, 0, false,
           "  fn() -> Int\n"
           "Fetch the version of the PQ library.")
{ return vm.makeVar<VarInt>(loc, PQlibVersion()); }

FERAL_FUNC(pqConnectDb, 1, false,
           "  fn(connStr) -> Result<PQConnection>\n"
           "Connect to the Postgres server using the connection string `connStr`.")
{
    const char *connStr = as<VarStr>(args[1])->cStr();
    PGconn *c           = PQconnectdb(connStr);
    if(PQstatus(c) != CONNECTION_OK) {
        VarError *e = vm.makeVar<VarError>(loc, PQstatus(c), PQerrorMessage(c));
        PQfinish(c);
        return vm.makeVar<VarResult>(loc, e, false);
    }
    VarPGConn *conn = vm.makeVar<VarPGConn>(loc, c);
    return vm.makeVar<VarResult>(loc, conn, true);
}

FERAL_FUNC(connGetVersionNative, 0, false,
           "  PGConn.fn() -> Str\n"
           "Fetch the database password from the database connection `PGConn`.")
{
    VarPGConn *c = as<VarPGConn>(args[0]);
    return vm.makeVar<VarInt>(loc, c->getVersion());
}

FERAL_FUNC(connGetDb, 0, false,
           "  PGConn.fn() -> Str\n"
           "Fetch the database name from the database connection `PGConn`.")
{
    VarPGConn *c = as<VarPGConn>(args[0]);
    return vm.makeVar<VarStr>(loc, c->getDb());
}

FERAL_FUNC(connGetUser, 0, false,
           "  PGConn.fn() -> Str\n"
           "Fetch the username from the database connection `PGConn`.")
{
    VarPGConn *c = as<VarPGConn>(args[0]);
    return vm.makeVar<VarStr>(loc, c->getUser());
}

FERAL_FUNC(connGetPass, 0, false,
           "  PGConn.fn() -> Str\n"
           "Fetch the database password from the database connection `PGConn`.")
{
    VarPGConn *c = as<VarPGConn>(args[0]);
    return vm.makeVar<VarStr>(loc, c->getPass());
}

INIT_DLL(PG)
{
    vm.addLocalType<VarPGConn>(loc, "PGConn", "PG database connection object");

    vm.addLocal(loc, "getVersionNative", pqGetVersionNative);
    vm.addLocal(loc, "new", pqConnectDb);

    vm.addTypeFn<VarPGConn>(loc, "getVersionNative", connGetVersionNative);
    vm.addTypeFn<VarPGConn>(loc, "getDb", connGetDb);
    vm.addTypeFn<VarPGConn>(loc, "getUser", connGetUser);
    vm.addTypeFn<VarPGConn>(loc, "getPass", connGetPass);

    return true;
}

} // namespace fer