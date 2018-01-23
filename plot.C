/* Jan 2018
 * Nguyen Ton
 * Purpose: plot sieve pattern of sim and data with current optic
 * Input:
 * - Root file from sim and data
 * Output:
 * - png plot of sieve slit
 * Attention:
 * - Low energy, I used dp cut instead of w cut
 */
#include <iomanip.h>

void plot()
{
  gStyle->SetOptStat(0);
  int nbin=200;
  double dp_min,w_max;
  int dpp;
  int runnum=2074;
  int Ebeam=2200;
  TString name="up";

  double x_min,x_max;
  if(runnum==1866||runnum==1805||runnum==2071||runnum==1565) {
    dp_min=-0.004;
    dpp=0;
    w_max=0.007;
    if(runnum==2071)
      w_max=0.01;
    x_min=-0.4;
    x_max=0.2;
  }
  if(runnum==1816||runnum==1566) {
    dp_min=0.017;
    dpp=-2;
    x_min=-0.1;
    x_max=0.4;
  }
  if(runnum==1809||runnum==1578) {
    dp_min=-0.025;
    dpp=2;
    x_min=-0.6;
    x_max=-0.1;
  }
  if(runnum==1869||runnum==2074) {
    dp_min=0.027;
    dpp=-3;
    w_max = 0.02;
    x_min=-0.1;
    x_max=0.6;
  }
  double y_min=-0.06,  y_max=0.06;
  double ph_min =-0.06,ph_max=0.06;
  double th_min=-0.06, th_max=0.06;
 
  //TFile* f1 = new TFile(Form("/home/ton/Desktop/rootfile/output_170412_16:10:32/gdh_%d.root",runnum));
  //TFile* f1 = new TFile(Form("/home/ton/Desktop/rootfile/output_160831_00:45:52/gdh_%d.root",runnum));
  //  TTree *tC=(TTree*)f1->Get("T");
  TFile* f1 = new TFile(Form("/home/ton/Desktop/asymmetry/2200MeV_rootfiles/output_171122_15:57:02/%d_sieve.root",runnum));
  TTree *tC=(TTree*)f1->Get("t1");

  Float_t deltatg,xs,phfoc,yfoc,thetatg,xfoc,thfoc,wmm;
  TH2F * h2x = new TH2F("h2x","",nbin,y_min,y_max,nbin,x_min, x_max);
  TH2F * h2phi = new TH2F("h2phi","",nbin,y_min,y_max,nbin,ph_min,ph_max);
  TH2F * h2theta = new TH2F("h2theta","",nbin,y_min,y_max,nbin,th_min,th_max);
  TFile * f = new TFile(Form("%d_143data_51pars_%s.root",runnum,name.Data()));

  TTree *ntup = (TTree*)gROOT->FindObject("h1");
  Int_t n_entries = (Int_t)ntup->GetEntries();
  ntup->SetBranchAddress("deltat",&(deltatg));
  ntup->SetBranchAddress("phfoc",&(phfoc));
  ntup->SetBranchAddress("yfoc",&(yfoc));
  ntup->SetBranchAddress("thetat",&(thetatg));
  ntup->SetBranchAddress("xfoc",&(xfoc));
  ntup->SetBranchAddress("thfoc",&(thfoc));
  ntup->SetBranchAddress("xs",&(xs));
  ntup->SetBranchAddress("wmm",&(wmm));
  for(int kk=0;kk<n_entries-1;kk++){
    ntup->GetEntry(kk);
   
    if(wmm<0.005&&thetatg>19){
    h2x->Fill(yfoc,xfoc);
    h2phi->Fill(yfoc,phfoc);
    h2theta->Fill(yfoc,thfoc);
    }
  }
  char cutdp[200],wcut[200],hLabelx[200],hLabelph[200],hLabelth[200];
  sprintf(hLabelx,"%s foil, %d dp;y_{fp}(m);x_{fp}(m)",name.Data(),dpp);
  sprintf(hLabelph,"%s foil, %d dp;y_{fp}(m);#phi_{fp}(rad)",name.Data(),dpp);
  sprintf(hLabelth,"%s foil, %d dp;y_{fp}(m);#theta_{fp}(rad)",name.Data(),dpp);

  //sprintf(cutdp,"R.gold.dp>=%f",dp_min);
 TH2F * C2dx = new TH2F("C2dx",hLabelx,nbin,y_min,y_max,nbin,x_min,x_max);
 TH2F * C2dph = new TH2F("C2dph",hLabelph,nbin,y_min,y_max,nbin,ph_min,ph_max);
 TH2F * C2dth = new TH2F("C2dth",hLabelth,nbin,y_min,y_max,nbin,th_min,th_max);

 sprintf(wcut,"rec_w2<%f",w_max);
 /*
 tC->Draw("R.tr.r_x[0]:R.tr.r_y[0]>>C2dx",cutdp,"cont0");
 tC->Draw("R.tr.r_ph[0]:R.tr.r_y[0]>>C2dph",cutdp,"cont0");
 tC->Draw("R.tr.r_th[0]:R.tr.r_y[0]>>C2dth",cutdp,"cont0");
 */
 tC->Draw("xfocal:yfocal>>C2dx",wcut,"cont0");
 tC->Draw("phfocal:yfocal>>C2dph",wcut,"cont0");
 tC->Draw("thfocal:yfocal>>C2dth",wcut,"cont0");

 TCanvas * cc1 = new TCanvas("cc1","",1500,400);
 cc1->Divide(3,1);
 cc1->cd(1);
 cc1->SetFrameFillColor(0);
 cc1->SetFrameFillStyle(0);
 cc1->SetFillStyle(0);
 C2dx->Draw("cont0");
 h2x->Draw("same");

 cc1->cd(2);
 C2dph->Draw("cont0");
 h2phi->Draw("same");

 cc1->cd(3);
 C2dth->Draw("cont0");
 h2theta->Draw("same");
 cc1->SaveAs(Form("fp_143data_51par_%dMeV_%s_%d_dp.png",Ebeam,name.Data(),dpp));
}
