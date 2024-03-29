// Author: Ryu Sawada
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// ROMESQLite3                                                           //
//                                                                      //
//  Provides SQL data base access.
//                                                                      //
//  $Id: ROMESQLite3.cpp 2574 2011-04-27 19:05:11Z sawada $
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "ROMEiostream.h"
#include <TString.h>
#include <TSystem.h>
#include "ROMESQLite3.h"

ClassImp(ROMESQLite3)
//______________________________________________________________________________
ROMESQLite3::ROMESQLite3()
:ROMESQL()
,db(0)
,stmt(0)
,result()
,numOfFields(0)
,numOfRows(0)
,currentRow(0)
{
   FreeResult();
}

//______________________________________________________________________________
ROMESQLite3::~ROMESQLite3()
{
   DisConnect();
}

//______________________________________________________________________________
Bool_t ROMESQLite3::Connect(const char *server, const char * /*user */ , const char * /*passwd */ ,
                            const char *database, const char * /*port */ )
{
   TString filename = server;
   if (strlen(database) && strlen(server)) {
      filename += "/";
      filename += database;
   }
   gSystem->ExpandPathName(filename);
   sqlite3_open(filename.Data(), &db);
   if (GetErrorCode() != SQLITE_OK) {
      ROMEPrint::Error("ROMESQLite3: %s\n", GetErrorMessage());
      return false;
   }
   return true;
}

//______________________________________________________________________________
Bool_t ROMESQLite3::DisConnect()
{
   if (db) {
      sqlite3_close(db);
      db = 0;
   }
   return true;
}

//______________________________________________________________________________
Bool_t ROMESQLite3::MakeQuery(const char *query, Bool_t store)
{
   if (strlen(query) < 2048) {
      ROMEPrint::Debug("\nROMESQLite3::MakeQuery : %s\n", query);
   } else {
      ROMEPrint::Debug("\nROMESQLite3::MakeQuery : Printing sql query is suppressed,"
                       " because query is longer than 2048.\n");
   }
   if (store) {
      sqlite3_prepare(db, query, -1, &stmt, NULL);
      if (GetErrorCode() != SQLITE_OK) {
         ROMEPrint::Error("%s\n", query);
         ROMEPrint::Error("Query error : %s\n", GetErrorMessage());
         return false;
      }
      if (!StoreResult()) {
         ROMEPrint::Error("Query error : %s\n", GetErrorMessage());
         return false;
      }
   } else if (sqlite3_exec(db, query, NULL, NULL, NULL) != SQLITE_OK) {
      ROMEPrint::Error("%s\n", query);
      ROMEPrint::Error("Query error : %s\n", GetErrorMessage());
      return false;
   }
   return true;
}

//______________________________________________________________________________
Bool_t ROMESQLite3::StoreResult()
{
   int i = 0;
   int j = 0;
   int column_count;

   while (sqlite3_step(stmt) != SQLITE_DONE) {
      switch (GetErrorCode()) {
      case SQLITE_BUSY:
         gSystem->Sleep(1);
         continue;
         break;
      case SQLITE_ROW:
         column_count = sqlite3_column_count(stmt);
         for (i = 0; i < column_count; i++) {
            result.SetAt(reinterpret_cast < const char *>(sqlite3_column_text(stmt, i)), i, j);
         }
         j++;
         break;
      default:
         return false;
      }
   }
   numOfFields = i;
   numOfRows = j;
   currentRow = -1;
   return true;
}

//______________________________________________________________________________
Bool_t ROMESQLite3::NextRow()
{
   if (currentRow + 1 >= GetNumberOfRows()) {
      ROMEPrint::Error("NextRow error : You have tried nonexistent row.\n");
      return false;
   }
   currentRow++;
   return true;
}

//______________________________________________________________________________
const char *ROMESQLite3::GetField(Int_t fieldNumber)
{
   if (fieldNumber < 0 || fieldNumber >= GetNumberOfFields()) {
      ROMEPrint::Error("GetField error : field number out of bounds\n");
      return NULL;
   }
   return result.At2(fieldNumber, currentRow).Data();
}

//______________________________________________________________________________
void ROMESQLite3::FreeResult()
{
   if (stmt) {
      sqlite3_finalize(stmt);
   }
   stmt = 0;
   numOfFields = 0;
   numOfRows = 0;
   currentRow = 0;
   result.RemoveAll();
}

//______________________________________________________________________________
Bool_t ROMESQLite3::StartTransaction(const char *option)
{
   TString sqlQuery = "BEGIN ";
   sqlQuery += option;
   sqlQuery += ";";
   return MakeQuery(sqlQuery.Data(), false);
}

//______________________________________________________________________________
Bool_t ROMESQLite3::CommitTransaction(const char *option)
{
   TString sqlQuery = "COMMIT ";
   sqlQuery += option;
   sqlQuery += ";";
   return MakeQuery(sqlQuery.Data(), false);
}

//______________________________________________________________________________
Bool_t ROMESQLite3::RollbackTransaction(const char *option)
{
   TString sqlQuery = "ROLLBACK ";
   sqlQuery += option;
   sqlQuery += ";";
   return MakeQuery(sqlQuery.Data(), false);
}
