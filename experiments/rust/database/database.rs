use rusqlite::{Connection, Result};
use std::fmt;
use std::time::{SystemTime, UNIX_EPOCH};

#[derive(Debug)]
struct User {
    username: String,
    passhash: String,
}

#[derive(Debug)]
struct Qump {
    id: u64,
    contents: String, // use BLOB eventually?
}

impl Qump {
    fn new(contents: &str) -> Qump {
        return Qump {
            id: 0,
            contents: contents.to_string(),
        };
    }
}

#[derive(Debug)]
enum QumpAction {
    Add,
    Edit,
    Remove,
}

impl fmt::Display for QumpAction {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        use QumpAction::*;
        let s = match self {
            Add => "add",
            Edit => "edit",
            Remove => "rm",
        };
        write!(f, "{}", s)?;
        Ok(())
    }
}

#[derive(Debug)]
struct Log {
    qump_id: u64,
    user: String,
    timestamp: u64, // use BLOB eventually?
    action: String,
}

#[derive(Debug)]
struct AccessKey {
    key: String,
    user: String,
    expiration: u64,
}

struct DatabaseManager {
    conn: Connection,
}

impl DatabaseManager {
    fn init(path: &str) -> Result<DatabaseManager, rusqlite::Error> {
        let conn = Connection::open(path)?;
        conn.execute(
            "create table if not exists qumps (
                id integer primary key,
                contents text not null
            )",
            [],
        )?;
        conn.execute(
            "create table if not exists users (
                id integer primary key,
                username text not null,
                password text not null
            )",
            [],
        )?;
        conn.execute(
            "create table if not exists log (
                id integer primary key,
                qump_id integer references qumps(id),
                timestamp integer not null,
                actions text not null
            )",
            // TODO: Index 'timestamp' and add 'user' var
            [],
        )?;
        Ok(DatabaseManager { conn: conn })
    }
    fn insert_qumps(&self, qumps: &[Qump]) -> Result<usize, rusqlite::Error> {
        for q in qumps {
            let ts = {
                let now = SystemTime::now();
                now.duration_since(UNIX_EPOCH)
                    .expect("Error getting timestamp")
                    .as_millis()
            };
            self.conn.execute(
                "INSERT INTO qumps (contents) values (?1)",
                &[&q.contents.to_string()],
            )?;
            let last_id: String = self.conn.last_insert_rowid().to_string();
            self.conn.execute(
                "INSERT INTO log (qump_id, timestamp, actions) values (?1, ?2, ?3)",
                &[&last_id, &ts.to_string(), &QumpAction::Add.to_string()],
            )?;
        }
        Ok(qumps.len())
    }
    fn remove_qumps(&self, qump_ids: &[u64]) {}
}

fn main() -> Result<()> {
    let dbm = DatabaseManager::init("qumps.db").unwrap();
    let qumps: [Qump; 2] = [Qump::new("New qump"), Qump::new("New qump again!")];
    dbm.insert_qumps(&qumps)?;
    Ok(())
}
