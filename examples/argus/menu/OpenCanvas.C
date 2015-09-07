void OpenCanvas()
{
   int event = gPad->GetEvent();
   if (event != 11) return;

   TPad *pnew = static_cast<TPad*>(gPad->Clone());
   TCanvas *c = new TCanvas("newpad");
   pnew->Draw();
   pnew->SetPad(0, 0, 1, 1);
   c->Update();
   c->SaveAs("t3window.pdf");

   return;
}
