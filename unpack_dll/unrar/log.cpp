#include "rar.hpp"

static void WriteToLog(const char *ArcName,const char *Message);

static char LogName[NM];

void InitLogOptions(char *LogName)
{
  strcpy(::LogName,LogName);
}


void Log(const char *ArcName,const char *Format,...)
{
  char Msg[4096];
  va_list ArgPtr;
  va_start(ArgPtr,Format);
  vsprintf(Msg,Format,ArgPtr);
  va_end(ArgPtr);
  eprintf("%s",Msg);
}


