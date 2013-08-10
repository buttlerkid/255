#include "ScriptPCH.h"
#include "Config.h"
#include "DatabaseWorkerPool.h"
#include "MySQLConnection.h"
#include "WebsiteDatabase.h"

uint32 WDB::references;
WebsiteDatabaseWorkerPool *WDB::WebsiteDatabase;
bool WDB::is_open;

void WebsiteDatabaseConnection::DoPrepareStatements()
{
	if (!m_reconnecting)
		m_stmts.resize(MAX_WEBSITEDATABASE_STATEMENTS);

	// Vote points
	PrepareStatement(WEB_GET_VOTE_POINTS, "SELECT `vp` FROM `account_data` WHERE `id` = ?", CONNECTION_SYNCH);
	PrepareStatement(WEB_ADD_VOTE_POINTS, "UPDATE `account_data` SET `vp` = `vp` + ? WHERE `id` = ?", CONNECTION_ASYNC);
	PrepareStatement(WEB_SUB_VOTE_POINTS, "UPDATE `account_data` SET `vp` = `vp` - ? WHERE `id` = ?", CONNECTION_ASYNC);

	// Donor points
	PrepareStatement(WEB_GET_DONOR_POINTS, "SELECT `dp` FROM `account_data` WHERE `id` = ?", CONNECTION_SYNCH);
	PrepareStatement(WEB_ADD_DONOR_POINTS, "UPDATE `account_data` SET `dp` = `dp` + ? WHERE `id` = ?", CONNECTION_ASYNC);
	PrepareStatement(WEB_SUB_DONOR_POINTS, "UPDATE `account_data` SET `dp` = `dp` - ? WHERE `id` = ?", CONNECTION_ASYNC);

	// Other
	PrepareStatement(WEB_GET_LAST_VOTE_TIME, "SELECT `time` FROM `vote_log` WHERE `user_id` = ? ORDER BY `id` DESC LIMIT 1", CONNECTION_SYNCH);
	PrepareStatement(WEB_GET_LAST_VOTE_TIME_DIFF, "SELECT (UNIX_TIMESTAMP() - `time`) AS `diff` FROM `vote_log` WHERE `user_id` = ? ORDER BY `id` DESC LIMIT 1", CONNECTION_SYNCH);
}

WDB::WDB()
{
	if( references++ == 0 )
	{
		WebsiteDatabase = new WebsiteDatabaseWorkerPool;
		
		std::string dbstring = ConfigMgr::GetStringDefault("WebsiteDatabaseInfo", "");
		if( dbstring.empty() )
		{
			sLog->outError(LOG_FILTER_GENERAL, "WebsiteDatabase: No Database Specified.");
			return;
		}

		is_open = WebsiteDatabase->Open(dbstring.c_str(), 1, 1);
		sLog->outInfo(LOG_FILTER_GENERAL, "WebsiteDatabase: Started website database connection pool.");
	}
}

WDB::~WDB()
{
	if( --references == 0 )
	{
		WebsiteDatabase->Close();
		is_open = false;
		delete WebsiteDatabase;
		WebsiteDatabase = NULL;
	}
}

uint32 WDB::GetLastVoteDiff(uint32 accountId)
{
	if( !is_open || !accountId )
		return 0;

	PreparedStatement *stmt = WebsiteDatabase->GetPreparedStatement(WEB_GET_LAST_VOTE_TIME_DIFF);
	stmt->setUInt32(0, accountId);
	PreparedQueryResult res = WebsiteDatabase->Query(stmt);

	if( res )
		return (*res)[0].GetUInt32();
	else
		return 0;
}