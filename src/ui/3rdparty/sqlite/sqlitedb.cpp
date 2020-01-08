#include "sqlitedb.h"

#include <QDateTime>
#include <QDir>
#include <QLoggingCategory>
#include <QSet>

#include <sqlite3.h>

#include "sqlitestmt.h"

Q_DECLARE_LOGGING_CATEGORY(CLOG_SQLITEDB)
Q_LOGGING_CATEGORY(CLOG_SQLITEDB, "fort.sqlitedb")

#define dbWarning() qCWarning(CLOG_SQLITEDB,)
#define dbCritical() qCCritical(CLOG_SQLITEDB,)

namespace {

bool removeDbFile(const QString &filePath)
{
    if (!filePath.startsWith(QLatin1Char(':'))
            && QFile::exists(filePath)
            && !QFile::remove(filePath)) {
        dbCritical() << "Cannot remove file:" << filePath;
        return false;
    }
    return true;
}

bool renameDbFile(const QString &filePath, const QString &newFilePath)
{
    removeDbFile(newFilePath);

    if (!QFile::rename(filePath, newFilePath)) {
        dbCritical() << "Cannot rename file" << filePath << "to" << newFilePath;
        return false;
    }
    return true;
}

}

SqliteDb::SqliteDb(const QString &filePath) :
    m_db(nullptr),
    m_filePath(filePath)
{
    sqlite3_initialize();
}

SqliteDb::~SqliteDb()
{
    close();

    sqlite3_shutdown();
}

bool SqliteDb::open(const QString &filePath)
{
    if (!filePath.isEmpty()) {
        m_filePath = filePath;
    }

    return sqlite3_open16(m_filePath.utf16(), &m_db) == SQLITE_OK;
}

void SqliteDb::close()
{
    if (m_db != nullptr) {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

bool SqliteDb::attach(const QString &schemaName, const QString &filePath)
{
    bool ok = false;
    executeEx("ATTACH DATABASE ?1 AS ?2;", {filePath, schemaName}, 0, &ok);
    return ok;
}

bool SqliteDb::detach(const QString &schemaName)
{
    bool ok = false;
    executeEx("DETACH DATABASE ?1;", {schemaName}, 0, &ok);
    return ok;
}

bool SqliteDb::execute(const char *sql)
{
    return sqlite3_exec(m_db, sql, nullptr, nullptr, nullptr) == SQLITE_OK;
}

bool SqliteDb::execute16(const ushort *sql)
{
    sqlite3_stmt *stmt = nullptr;
    int res = sqlite3_prepare16_v2(db(), sql, -1, &stmt, nullptr);
    if (stmt != nullptr) {
        res = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    return res;
}

bool SqliteDb::executeStr(const QString &sql)
{
    return execute16(sql.utf16());
}

QVariant SqliteDb::executeEx(const char *sql,
                             const QVariantList &vars,
                             int resultCount,
                             bool *ok)
{
    QVariantList list;

    SqliteStmt stmt;
    bool success = false;

    if (stmt.prepare(db(), sql)
            && stmt.bindVars(vars)) {
        const auto stepRes = stmt.step();
        success = (stepRes != SqliteStmt::StepError);

        // Get result
        if (stepRes == SqliteStmt::StepRow) {
            for (int i = 0; i < resultCount; ++i) {
                const QVariant v = stmt.columnVar(i);
                list.append(v);
            }
        }
    }

    if (ok != nullptr) {
        *ok = success;
    }

    const int listSize = list.size();
    return (listSize == 0) ? QVariant()
                           : (listSize == 1 ? list.at(0) : list);
}

qint64 SqliteDb::lastInsertRowid() const
{
    return sqlite3_last_insert_rowid(m_db);
}

int SqliteDb::changes() const
{
    return sqlite3_changes(m_db);
}

bool SqliteDb::beginTransaction()
{
    return execute("BEGIN;");
}

bool SqliteDb::endTransaction(bool ok)
{
    return ok ? commitTransaction()
              : rollbackTransaction();
}

bool SqliteDb::commitTransaction()
{
    return execute("COMMIT;");
}

bool SqliteDb::rollbackTransaction()
{
    return execute("ROLLBACK;");
}

bool SqliteDb::beginSavepoint(const char *name)
{
    return (name == nullptr)
            ? execute("SAVEPOINT _;")
            : executeStr(QString("SAVEPOINT %1;").arg(name));
}

bool SqliteDb::releaseSavepoint(const char *name)
{
    return (name == nullptr)
            ? execute("RELEASE _;")
            : executeStr(QString("RELEASE %1;").arg(name));
}

bool SqliteDb::rollbackSavepoint(const char *name)
{
    return (name == nullptr)
            ? execute("ROLLBACK TO _;")
            : executeStr(QString("ROLLBACK TO %1;").arg(name));
}

QString SqliteDb::errorMessage() const
{
    const char *text = sqlite3_errmsg(m_db);

    return QString::fromUtf8(text);
}

int SqliteDb::userVersion()
{
    return executeEx("PRAGMA user_version;").toInt();
}

QString SqliteDb::entityName(const QString &schemaName,
                             const QString &objectName)
{
    return schemaName.isEmpty() ? objectName
                                : schemaName + '.' + objectName;
}

QStringList SqliteDb::tableNames(const QString &schemaName)
{
    QStringList list;

    const auto masterTable = entityName(schemaName, "sqlite_master");
    const auto sql = QString(
                "SELECT name FROM %1"
                "  WHERE type = 'table' AND name NOT LIKE 'sqlite_%';"
                ).arg(masterTable);

    SqliteStmt stmt;
    if (stmt.prepare(db(), sql.toLatin1())) {
        while (stmt.step() == SqliteStmt::StepRow) {
            list.append(stmt.columnText());
        }
    }

    return list;
}

QStringList SqliteDb::columnNames(const QString &tableName,
                                  const QString &schemaName)
{
    QStringList list;

    const auto schemaTableName = entityName(schemaName, tableName);
    const auto sql = QString(
                "SELECT * FROM %1 WHERE 0 = 1 LIMIT 1;"
                ).arg(schemaTableName);

    SqliteStmt stmt;
    if (stmt.prepare(db(), sql.toLatin1())) {
        const int n = stmt.columnCount();
        for (int i = 0; i < n; ++i) {
            list.append(stmt.columnName(i));
        }
    }

    return list;
}

bool SqliteDb::migrate(const QString &sqlDir, int version,
                       bool recreate, bool importOldData,
                       SQLITEDB_MIGRATE_FUNC migrateFunc,
                       void *migrateContext)
{
    // Check version
    int userVersion = this->userVersion();
    if (userVersion == version)
        return true;

    if (userVersion > version) {
        dbWarning() << "Cannot open new DB" << userVersion
                    << "from old application" << version;
        return false;
    }

    // Re-create the DB
    QString tempFilePath;
    if (recreate && userVersion != 0) {
        close();

        tempFilePath = m_filePath + ".temp";

        if (!(renameDbFile(m_filePath, tempFilePath)
              && open(m_filePath))) {
            dbWarning() << "Cannot re-create the DB" << m_filePath;
            renameDbFile(tempFilePath, m_filePath);
            return false;
        }

        userVersion = 0;
    }

    // Run migration SQL scripts
    QDir dir(sqlDir);
    bool success = true;

    beginTransaction();

    for (int i = userVersion + 1; i <= version; ++i) {
        const QString filePath = dir.filePath(QString::number(i) + ".sql");

        QFile file(filePath);
        if (!file.exists())
            continue;

        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            dbWarning() << "Cannot open migration file" << filePath
                        << file.errorString();
            success = false;
            break;
        }

        const QByteArray data = file.readAll();
        if (data.isEmpty()) {
            dbWarning() << "Migration file is empty" << filePath;
            success = false;
            break;
        }

        beginSavepoint();
        if (!execute(data.constData())
                || !(migrateFunc == nullptr
                     || migrateFunc(this, i, migrateContext))) {
            dbCritical() << "Migration error:" << filePath << errorMessage();
            success = false;
            rollbackSavepoint();
            break;
        }
        releaseSavepoint();
    }

    commitTransaction();

    // Re-create the DB: End
    if (recreate && !tempFilePath.isEmpty()) {
        // Re-import the DB
        if (success && importOldData) {
            success = importDb(tempFilePath);
        }

        // Remove the old DB
        if (success) {
            removeDbFile(tempFilePath);
        } else {
            renameDbFile(tempFilePath, m_filePath);
        }
    }

    return success;
}

bool SqliteDb::importDb(const QString &sourceFilePath)
{
    const QLatin1String srcSchema("src");
    const QLatin1String dstSchema("main");

    if (!attach(srcSchema, sourceFilePath)) {
        dbWarning() << "Cannot attach the DB" << sourceFilePath;
        return false;
    }

    // Import Data
    bool success = true;

    beginTransaction();

    for (const auto &tableName : tableNames(srcSchema)) {
        const auto dstColumns = columnNames(tableName, dstSchema);
        if (dstColumns.isEmpty())
            continue;  // new schema doesn't contain old table

        const auto srcColumns = columnNames(tableName, srcSchema);
        if (srcColumns.isEmpty())
            continue;  // empty old table

        // Intersect column names
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        auto columnsSet = QSet<QString>(srcColumns.constBegin(), srcColumns.constEnd());
        const auto dstColumnsSet = QSet<QString>(dstColumns.constBegin(), dstColumns.constEnd());
#else
        auto columnsSet = QSet<QString>::fromList(srcColumns);
        const auto dstColumnsSet = QSet<QString>::fromList(dstColumns);
#endif
        columnsSet.intersect(dstColumnsSet);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        const QStringList columns(columnsSet.constBegin(), columnsSet.constEnd());
#else
        const QStringList columns(columnsSet.toList());
#endif
        const QString columnNames = columns.join(", ");

        // Insert
        const auto sql = QString("INSERT INTO %1 (%3) SELECT %3 FROM %2;")
                .arg(entityName(dstSchema, tableName),
                     entityName(srcSchema, tableName),
                     columnNames);

        if (!execute(sql.toLatin1())) {
            success = false;
            break;
        }
    }

    endTransaction(success);

    detach(srcSchema);

    return success;
}
