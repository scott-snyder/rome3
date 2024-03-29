// Author: Matthias Schneebeli
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// ROMEStr2DArray                                                       //
//                                                                      //
//  Handles character string array of array.
//                                                                      //
//  $Id: ROMEStr2DArray.cpp 2574 2011-04-27 19:05:11Z sawada $
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "ROMEiostream.h"
#include <TObjString.h>
#include "ROMEString.h"
#include "ROMEStrArray.h"
#include "ROMEStr2DArray.h"

ClassImp(ROMEStr2DArray)

//______________________________________________________________________________
ROMEStr2DArray::ROMEStr2DArray(Int_t sizeX,Int_t sizeY)
:array(new TObjArray(sizeX))
,fSizeX(sizeX)
,fSizeY(sizeY)
{
}

//______________________________________________________________________________
ROMEStr2DArray::ROMEStr2DArray(const ROMEStr2DArray &str2darray)
:TObject(str2darray)
,array(str2darray.array)
,fSizeX(str2darray.fSizeX)
,fSizeY(str2darray.fSizeY)
{
}

//______________________________________________________________________________
ROMEStr2DArray& ROMEStr2DArray::operator=(const ROMEStr2DArray &rhs)
{
   if (this != &rhs) {
      array = rhs.array;
      fSizeX = rhs.fSizeX;
      fSizeY = rhs.fSizeY;
   }
   return *this;
}

//______________________________________________________________________________
ROMEStr2DArray::~ROMEStr2DArray()
{
   this->RemoveAll();
   SafeDelete(array);
}

//______________________________________________________________________________
Int_t ROMEStr2DArray::GetEntries() const
{
   return array->GetEntries();
}

//______________________________________________________________________________
Int_t ROMEStr2DArray::GetEntriesFast() const
{
   return array->GetEntriesFast();
}

//______________________________________________________________________________
Int_t ROMEStr2DArray::GetEntriesAt(Int_t idx) const
{
   if (idx<0||idx>=array->GetEntriesFast()) {
      return 0;
   }
   if (array->At(idx)==NULL) {
      return 0;
   }
   return static_cast<TObjArray*>(array->At(idx))->GetEntries();
}

//______________________________________________________________________________
Int_t ROMEStr2DArray::GetEntriesFastAt(Int_t idx) const
{
   if (idx<0||idx>=array->GetEntriesFast()) {
      return 0;
   }
   if (array->At(idx)==NULL) {
      return 0;
   }
   return static_cast<TObjArray*>(array->At(idx))->GetEntriesFast();
}

//______________________________________________________________________________
void ROMEStr2DArray::SetAt(TString &str, Int_t idx, Int_t idy)
{
   this->SetAt(str.Data(),idx,idy);
}

//______________________________________________________________________________
void ROMEStr2DArray::SetAt(const char* str, Int_t idx, Int_t idy)
{
   if (idx<0||idx>=array->GetEntriesFast()) {
      array->AddAtAndExpand(new TObjArray(idy+1),idx);
   }
   if (array->At(idx)==NULL) {
      array->AddAtAndExpand(new TObjArray(idy+1),idx);
   }
   TObjArray *subArray = static_cast<TObjArray*>(array->At(idx));
   if (idy>=0&&idy<subArray->GetEntriesFast()) {
      if (subArray->At(idy)!=NULL) {
         this->RemoveAt(idx,idy);
      }
   }
   subArray->AddAtAndExpand(new TObjString(str),idy);
}

//______________________________________________________________________________
void ROMEStr2DArray::SetAt(ROMEStrArray *str, Int_t idx, Int_t idy)
{
   if (idx<0||idx>=array->GetEntriesFast()) {
      array->AddAtAndExpand(new TObjArray(idy+1),idx);
   }
   if (array->At(idx)==NULL) {
      array->AddAtAndExpand(new TObjArray(idy+1),idx);
   }
   TObjArray *subArray = static_cast<TObjArray*>(array->At(idx));
   for (int i=0;i<str->GetEntriesFast();i++) {
      if (str->At(i).Length()>0) {
         if (i+idy>=0&&i+idy<subArray->GetEntriesFast()) {
            if (subArray->At(i+idy)!=NULL) {
               this->RemoveAt(idx,i+idy);
            }
         }
         subArray->AddAtAndExpand(new TObjString(str->At(i)),i+idy);
      }
   }
}

//______________________________________________________________________________
void ROMEStr2DArray::RemoveAt(Int_t idx, Int_t idy)
{
   if (idx<0||idx>=array->GetEntriesFast()) {
      return;
   }
   if (array->At(idx)==NULL) {
      return;
   }
   TObjArray *subArray = static_cast<TObjArray*>(array->At(idx));
   if (idy>=subArray->GetEntriesFast()||idy<0) {
      return;
   }
   if (subArray->At(idy)==NULL) {
      return;
   }
   delete subArray->At(idy);
   subArray->RemoveAt(idy);
}

//______________________________________________________________________________
void ROMEStr2DArray::RemoveAllAt(Int_t idx)
{
   if (idx<0||idx>=array->GetEntriesFast()) {
      return;
   }
   if (array->At(idx)==NULL) {
      return;
   }
   TObjArray *subArray = static_cast<TObjArray*>(array->At(idx));
   const Int_t nStr = subArray->GetEntriesFast();
   for (int i=0;i<nStr;i++) {
      delete subArray->At(i);
   }
   subArray->RemoveAll();
   delete array->At(idx);
   array->RemoveAt(idx);
}

//______________________________________________________________________________
void ROMEStr2DArray::RemoveAll()
{
   const Int_t nArray = array->GetEntriesFast();
   Int_t nStr;
   for (int i=0;i<nArray;i++) {
      TObjArray *subArray = static_cast<TObjArray*>(array->At(i));
      if (subArray==NULL) {
         continue;
      }
      nStr = subArray->GetEntriesFast();
      for (int j=0;j<nStr;j++) {
         delete subArray->At(j);
      }
      subArray->RemoveAll();
      delete subArray;
   }
   array->RemoveAll();
}

//______________________________________________________________________________
TString ROMEStr2DArray::At(Int_t idx, Int_t idy) const
{
   if (idx<0||idx>=array->GetEntriesFast()) {
      return "";
   }
   if (array->At(idx)==NULL) {
      return "";
   }
   TObjArray *subArray = static_cast<TObjArray*>(array->At(idx));
   if (idy<0||idy>=subArray->GetEntriesFast()) {
      return "";
   }
   if (subArray->At(idy)==NULL) {
      return "";
   }
   return static_cast<TObjString*>(subArray->At(idy))->GetString();
}

//______________________________________________________________________________
TString& ROMEStr2DArray::At2(Int_t idx, Int_t idy) const
{
#if 0
   if (idx<0||idx>=array->GetEntriesFast()) {
      return "";
   }
   if (array->At(idx)==NULL) {
      return "";
   }
   TObjArray *subArray = static_cast<TObjArray*>(array->At(idx));
   if (idy<0||idy>=subArray->GetEntriesFast()) {
      return "";
   }
   if (subArray->At(idy)==NULL) {
      return "";
   }
#endif
   TObjArray *subArray = static_cast<TObjArray*>(array->At(idx));
   return static_cast<TObjString*>(subArray->At(idy))->String();
}
