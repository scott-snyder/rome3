/********************************************************************
  TNetFolderServer.h, M. Schneebeli PSI

  $Id: TNetFolderServer.h 2069 2007-08-19 18:23:50Z sawada $

********************************************************************/
#ifndef TNetFolderServer_H
#define TNetFolderServer_H

#include <RConfig.h>
#if defined( R__VISUAL_CPLUSPLUS )
#   pragma warning( push )
#   pragma warning( disable : 4800 )
#endif // R__VISUAL_CPLUSPLUS
#include <TNamed.h>
#include <TString.h>
#include <TFolder.h>
#if defined( R__VISUAL_CPLUSPLUS )
#   pragma warning( pop )
#endif // R__VISUAL_CPLUSPLUS
#include "ROME.h"

class TApplication;
class TSocket;
class TThread;
class TList;

extern Bool_t gNetFolderServerRunning;
const  Int_t  kNetFolderServerTimeOut = 20; // micro sec

class TNetFolderServer : public TNamed
{
protected:
   static TApplication *fApplication;
   static Int_t         fPort;
   static TString       fServerName;
   static TThread      *fServerLoopThread;
   static TList        *fServerThreadList;

private:
   TNetFolderServer(const TNetFolderServer &server); // not implemented
   TNetFolderServer &operator=(const TNetFolderServer &rhs); // not implemented

public:
   TNetFolderServer();
   virtual ~TNetFolderServer();

   virtual void       StartServer(TApplication *app,Int_t port,const char* serverName);
   virtual void       StopServer();
   static Int_t       ResponseFunction(TSocket *socket);

protected:
   static Int_t       CheckCommand(TSocket *socket,char *str);
   static TFolder    *ReadFolderPointer(TSocket *socket);
   static THREADTYPE  Server(void *arg);
   static THREADTYPE  ServerLoop(void *arg);

   ClassDef(TNetFolderServer, 0) // Server for socket connection
};

#endif   // TNetFolderServer_H
