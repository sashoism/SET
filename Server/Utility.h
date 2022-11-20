#include <QDateTime>
#include <QDebug>

#define LOG_TIME qPrintable(QDateTime::currentDateTime().toString("[LOG] [dd/MM/yyyy HH:mm:ss]"))

#define LISTEN_PORT 2187
#define MAX_PLAYERS 8
#define MAX_CHARS_PNAME 16
#define SCHEDULED_BROADCAST_INTERVAL_SEC 30
#define EMPTY_GAME_EXPIRIRATION_PERIOD_SEC 30
#define SHOW_SETS 1
