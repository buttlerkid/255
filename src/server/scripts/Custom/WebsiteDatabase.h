#ifndef __WEBSITEDATABASE_H__
#define __WEBSITEDATABASE_H__

class WebsiteDatabaseConnection : public MySQLConnection
{
    public:
        //- Constructors for sync and async connections
        WebsiteDatabaseConnection(MySQLConnectionInfo& connInfo) : MySQLConnection(connInfo) {}
        WebsiteDatabaseConnection(ACE_Activation_Queue* q, MySQLConnectionInfo& connInfo) : MySQLConnection(q, connInfo) {}

        //- Loads database type specific prepared statements
		void DoPrepareStatements();
};

typedef DatabaseWorkerPool<WebsiteDatabaseConnection> WebsiteDatabaseWorkerPool;

enum WebsiteDatabaseStatements
{
	WEB_GET_VOTE_POINTS,
	WEB_ADD_VOTE_POINTS,
	WEB_SUB_VOTE_POINTS,
	WEB_GET_DONOR_POINTS,
	WEB_ADD_DONOR_POINTS,
	WEB_SUB_DONOR_POINTS,
	WEB_GET_LAST_VOTE_TIME,
	WEB_GET_LAST_VOTE_TIME_DIFF,

	MAX_WEBSITEDATABASE_STATEMENTS
};

class WDB
{
private:
	static uint32 references;
	static WebsiteDatabaseWorkerPool *WebsiteDatabase;
	static bool is_open;

public:
	WDB();
	~WDB();
	
	bool IsOpen() { return is_open; }

	bool IsVIP(uint32 accountId) { uint32 diff = GetLastVoteDiff(accountId); return (diff > 0 && diff < 12*HOUR); }
	inline bool IsVIP(WorldSession *sess) { uint32 diff = GetLastVoteDiff(sess); return (diff > 0 && diff < 12*HOUR); }
	inline bool IsVIP(Player *plr) { uint32 diff = GetLastVoteDiff(plr); return (diff > 0 && diff < 12*HOUR); }
	uint32 GetLastVoteDiff(uint32 accountId);
	inline uint32 GetLastVoteDiff(WorldSession *sess) { return (sess != NULL) ? GetLastVoteDiff(sess->GetAccountId()) : 0; }
	inline uint32 GetLastVoteDiff(Player *plr) { return (plr != NULL && plr->GetSession() != NULL) ? GetLastVoteDiff(plr->GetSession()->GetAccountId()) : 0; }

	WebsiteDatabaseWorkerPool *operator->() { return WebsiteDatabase; }
};

#endif