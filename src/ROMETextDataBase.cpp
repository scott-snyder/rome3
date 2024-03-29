// $Id: ROMETextDataBase.cpp 2506 2009-04-25 20:17:21Z sawada $
// Author: Ryu Sawada

//////////////////////////////////////////////////////////////////////////
//
//  Text format non-relational database.
//
//  Arguments of Init
//     name       : name of database
//     path       : directory of files
//     connection : not used
//
//  Rules of dataBasePath
//     Data with following dataBasePath
//        filename/valuename+
//        filename/valuename
//        filename/valuename-
//     will be stored in text file like
//     <valuename>
//     1.0, 1.0
//     2.0, 1.0
//     3.4,                  // <-- there is a comma
//     2.2                   // <-- no comma
//     2.1, 4.5
//     ...
//     5.0, 0.0
//     </valuename>
//     Letters between <valuename> and </valuename> are recognized as data.
//     In the above example, 3.4 and 2.2 are recognized in the same row.
//
//     If there is + at the end, new data will be added after existing data.
//     If there is - at the end, new data will be added before existing data.
//     When reading, data which was found first will be used.
//
//   In text files, C like comment is available.
//
//////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <RConfig.h>
#if defined( R__VISUAL_CPLUSPLUS )
#   pragma warning( push )
#   pragma warning( disable : 4244 4800)
#endif // R__VISUAL_CPLUSPLUS
#include <TSystem.h>
#include <TArrayI.h>
#include <TTimeStamp.h>
#if defined( R__VISUAL_CPLUSPLUS )
#   pragma warning( pop )
#endif // R__VISUAL_CPLUSPLUS
#include "ROMEiostream.h"
#include "ROMEStrArray.h"
#include "ROMETextDataBase.h"

using namespace std;

const char* EndOfHeader = "/////////////////////////////////////----///////////////////////////////////////";
const Int_t numbering = 10;

ClassImp(ROMETextDataBase)

//______________________________________________________________________________
ROMETextDataBase::ROMETextDataBase()
:ROMEDataBase()
,fDirectoryPath("")
,fTime(new TTimeStamp())
{
}

//______________________________________________________________________________
ROMETextDataBase::~ROMETextDataBase() {
   SafeDelete(fTime);
}

//______________________________________________________________________________
Bool_t ROMETextDataBase::Init(const char* name,const char* path,const char* /*connection*/) {
   // set directory
   // "connection" has no mean for this class.
   if (!path) {
      return kFALSE;
   }
   if(strlen(path)){
      fDirectoryPath = path;
   } else {
      fDirectoryPath = ".";
   }
   fDirectoryPath += "/";

   fName = name;

   return true;
}

//______________________________________________________________________________
Bool_t ROMETextDataBase::Read(ROMEStr2DArray *values,const char *dataBasePath,
                              Long64_t /*runNumber*/,Long64_t /*eventNumber*/)
{
   Ssiz_t     ps,pe;
   Int_t      iRow;
   Int_t      iCol = 0;
   ROMEString lineBuffer = "";
   ROMEString fileName;
   ROMEString valueName;
   ROMEString tmp;
   // initialize
   RemoveComment(lineBuffer,true);

   // extract value name and file name
   fileName = dataBasePath;
   // remove + and - at the end of file name
   if(fileName(fileName.Length()-1) == '+'){
      fileName.Resize(fileName.Length()-1);
   }
   if(fileName(fileName.Length()-1) == '-'){
      fileName.Resize(fileName.Length()-1);
   }
   ps = pe = 0;
   while((ps=fileName.Index("/",1,pe,TString::kExact))!=-1){
      pe = ps +1;
   }
   if(!pe){
      valueName = "Value";
   } else {
      valueName = fileName(pe,fileName.Length()-pe);
      fileName.Remove(pe-1);
   }
   fileName.Prepend(fDirectoryPath);

   // open file
   gSystem->ExpandPathName(fileName);
   fstream    fileStream(fileName.Data(), ios::in);
   if(!fileStream)
      return true;

   // read data
   Bool_t start = false;
   Bool_t end = false;
   Bool_t lineEndsWithComma = false;

   iRow = 0;
   while(lineBuffer.ReadLine(fileStream)){
      if(end)
         break;

      RemoveComment(lineBuffer);
      if(!ContainsData(lineBuffer.Data()))
         continue;

      if(lineBuffer.Contains(StartMark(valueName.Data())))
         start = true;

      if(start){
         lineBuffer.StripSpaces();
         if (lineEndsWithComma) {
            lineEndsWithComma = false;
         } else {
            iCol=0;
         }
         ps = pe = 0;
         // skip start mark
         lineBuffer.ReplaceAll(StartMark(valueName.Data()),"");
         if((ps=lineBuffer.Index(EndMark(valueName.Data()),strlen(EndMark(valueName.Data())),0,TString::kExact))!=-1){
            end = true;
            lineBuffer.ReplaceAll(EndMark(valueName.Data()),"");
         }

         if(!ContainsData(lineBuffer.Data()))
            continue;

        while((ps=lineBuffer.Index(",",1,pe,TString::kExact))!=-1){
           tmp = lineBuffer(pe,ps-pe);
           tmp.StripSpaces();
           values->SetAt(tmp,iRow,iCol);
           pe = ps+1;
           iCol++;
           if (ps == lineBuffer.Length()-1) {
              lineEndsWithComma = true;
              break;
           }
        }
        if (!lineEndsWithComma) {
           tmp = lineBuffer(pe,lineBuffer.Length()-pe);
           tmp.StripSpaces();
           values->SetAt(tmp,iRow,iCol);
           iRow++;
        }
      }
   }

   return true;
}

//______________________________________________________________________________
Bool_t ROMETextDataBase::Write(ROMEStr2DArray* values,const char *dataBasePath,
                               Long64_t /*runNumber*/,Long64_t /*eventNumber*/)
{
   fstream*   fileStream;
   ROMEString fileBuffer;
   ROMEString buffer;
   ROMEString fileName;
   ROMEString valueName;
   Int_t      iRow,iCol;
   Bool_t     append  = false;
   Bool_t     prepend = false;
   Ssiz_t     ps,pe;
   TArrayI    fieldLen;

   fTime->Set();

   // extract value name and file name
   fileName = dataBasePath;
   if(fileName(fileName.Length()-1) == '+'){
      append = true;
      fileName.Resize(fileName.Length()-1);
   }
   if(fileName(fileName.Length()-1) == '-'){
      prepend = true;
      fileName.Resize(fileName.Length()-1);
   }
   ps = pe = 0;
   while((ps=fileName.Index("/",1,pe,TString::kExact))!=-1){
      pe = ps +1;
   }
   if(!pe){
      valueName = "Value";
   } else {
      valueName = fileName(pe,fileName.Length()-pe);
      fileName.Remove(pe-1);
   }
   fileName.Prepend(fDirectoryPath);

   // read existing file
   if(append || prepend){
      if(!(fileStream = new fstream(fileName.Data(),ios::in))){
         append = prepend = false;
      } else {
         fileBuffer.ReadFile(*fileStream);
         delete fileStream;
      }
   }

   const Int_t kColPerLine = 5;

   // count field length
   for(iRow=0;iRow<values->GetEntries();iRow++){
      if(values->GetEntriesAt(iRow)>fieldLen.GetSize())
         fieldLen.Set(values->GetEntriesAt(iRow));
      for(iCol=0;iCol<values->GetEntriesAt(iRow);iCol++){
         if(fieldLen.At(iCol%kColPerLine) < static_cast<Ssiz_t>(strlen(values->At(iRow,iCol))))
            fieldLen.AddAt(strlen(values->At(iRow,iCol)),iCol%kColPerLine);
      }
   }

   // format data
   buffer.Resize(0);
   if(append || prepend)
      buffer.AppendFormatted("// %s\n",fTime->AsString("l"));
   buffer.AppendFormatted("%s",StartMark(valueName.Data()));
   if(values->GetEntries()>1)
      buffer += "\n";
   for(iRow=0;iRow<values->GetEntries();iRow++){
      for(iCol=0;iCol<fieldLen.GetSize();iCol++){
         if(iCol<values->GetEntriesAt(iRow)) {
            buffer.AppendFormatted(" %*s",fieldLen.At(iCol%kColPerLine),values->At(iRow,iCol).Data());
         } else {
            buffer.AppendFormatted(" %*s",fieldLen.At(iCol%kColPerLine),"");
         }
         if(iCol == fieldLen.GetSize()-1){
            if(values->GetEntries()>numbering)
               buffer.AppendFormatted("        //%5d",iRow);
            buffer += "\n";
         } else if ((iCol+1) % kColPerLine == 0) {
            buffer += ",\n";
         } else {
            buffer += ",";
         }
      }
   }
   if (values->GetEntries() == 1)
      buffer.Resize(buffer.Length()-1); // remove the last '\n'
   buffer.AppendFormatted("%s\n",EndMark(valueName.Data()));
   buffer.AppendFormatted("\n");

   RemoveHeader(fileBuffer);

   if(append) {
      buffer.Prepend(fileBuffer);
   } else if (prepend) {
      buffer.Append(fileBuffer);
   }

   AddHeader(buffer,fileName.Data());

   // write file
   if(!(fileStream = new fstream(fileName.Data(),ios::out | ios::trunc))){
      ROMEPrint::Error("\n\nError : Failed to open '%s' !!!\n", fileName.Data());
      return false;
   }
   *fileStream<<buffer;

   delete fileStream;
   return true;
}

//______________________________________________________________________________
void ROMETextDataBase::RemoveComment(ROMEString &buffer,Bool_t initialize) const
{
   static Bool_t inComment;
   Ssiz_t ps,pe;

   if(initialize) {
      inComment = false;
   }

   // remove after "//"
   if((ps = buffer.Index("//",2,0,TString::kExact)) != -1){
      buffer.Remove(ps);
   }

   // remove between "/*" and "*/"
   if(inComment){
      if((pe = buffer.Index("*/",2,0,TString::kExact)) != -1){
         buffer.Remove(0,pe+strlen("*/"));
         inComment = false;
      } else {
         buffer.Resize(0);
      }
   }
   ps = pe = 0;
   while((ps = buffer.Index("/*",2,pe,TString::kExact)) != -1){
      inComment = true;
      if((pe = buffer.Index("*/",2,ps,TString::kExact)) != -1){
         buffer.Remove(ps,pe-ps+strlen("*/"));
         inComment = false;
         pe = ps;
      } else {
         pe = ps + strlen("/*");
      }
   }
   if(inComment) {
      buffer.Remove(max(0, static_cast<int>(pe - strlen("/*"))));
   }

   return;
}

//______________________________________________________________________________
Bool_t ROMETextDataBase::ContainsData(const char* buffer) const
{
   // return true if "buffer" contains non-space characters
   if(!buffer)
      return false;

   ROMEString tmp = buffer;
   tmp.ReplaceAll(" ","");
   tmp.ReplaceAll("\t","");

   if(tmp.Length())
      return true;

   return false;
}

//______________________________________________________________________________
const char* ROMETextDataBase::StartMark(const char* valueName) const
{
   static ROMEString mark;
   mark.SetFormatted("<%s>",valueName);
   return mark.Data();
}

//______________________________________________________________________________
const char* ROMETextDataBase::EndMark(const char* valueName) const
{
   static ROMEString mark;
   mark.SetFormatted("</%s>",valueName);
   return mark.Data();
}

//______________________________________________________________________________
void ROMETextDataBase::AddHeader(ROMEString &buffer,const char* fileName) const
{
   ROMEString filename = fileName;
   ROMEString tmp;
   ROMEStrArray valueNames;

   Ssiz_t ps,pe;

   ps = pe = 0;
   while((ps=filename.Index("/",1,pe,TString::kExact))!=-1){
      pe = ps+1;
   }
   if(pe)
      filename.Remove(0,pe);

   ROMEString header;
   header.Resize(0);
   header.AppendFormatted("////////////////////////////////////////////////////////////////////////////////\n");
   header.AppendFormatted("//\n");
   header.AppendFormatted("// %s\n",filename.Data());
   header.AppendFormatted("// %s\n",fTime->AsString("l"));
   header.AppendFormatted("//\n");
   header.AppendFormatted("// This file contains following data.\n");
   ps = pe = 0;
   Int_t i;
   while((ps=buffer.Index("<",1,pe,TString::kExact))!=-1){
      pe=buffer.Index(">",1,ps,TString::kExact);
      if((ps=buffer.Index("</",2,pe,TString::kExact))==-1)
         break;
      pe=buffer.Index(">",1,ps,TString::kExact);
      tmp = buffer(ps+2,pe-ps-2);
      for(i=0;i<valueNames.GetEntries();i++){
         if(tmp == valueNames.At(i))
            break;
      }
      if(i==valueNames.GetEntries()){
         valueNames.AddAt(tmp,i);
         header.AppendFormatted("//    %s\n",tmp.Data());
      }
      pe = ps + 1;
   }
   header.AppendFormatted("//\n");
   header.AppendFormatted("%s\n",EndOfHeader);

   buffer.Prepend(header);

   return;
}

//______________________________________________________________________________
void ROMETextDataBase::RemoveHeader(ROMEString &buffer) const
{
   Ssiz_t pBuffer = buffer.Index(EndOfHeader,strlen(EndOfHeader),0,TString::kExact);
   if(pBuffer>=0)
      buffer.Remove(0,pBuffer+strlen(EndOfHeader)+1);
   return;
}
