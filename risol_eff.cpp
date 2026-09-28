#include "TF1.h" 
#include "TH1F.h" 
#include "TH1I.h" 
#include "TTree.h" 
#include <TFile.h>
#include <TCanvas.h>
#include <iostream>
#include <TGraph.h>
#include <TSystem.h>
#include <cmath>
#include <TFitResultPtr.h>
#include <TFitResult.h>
#include <TVector.h>
#include <TLinearFitter.h>
#include <TMatrixD.h>
#include <TMath.h>
#include <algorithm> 
#include "hits_header.h"

using namespace std;


TTree *t = nullptr;
Double_t X[3][10];
Double_t Edep[2][10];

TH1F *E_depx1 = nullptr;
TH1F *E_depx2 = nullptr;
TH1F *E_depx3 = nullptr;
TH1F *E_depx4 = nullptr;
TH1F *E_depx5 = nullptr;

TH1F *E_depy1 = nullptr;
TH1F *E_depy2 = nullptr;
TH1F *E_depy3 = nullptr;
TH1F *E_depy4 = nullptr;
TH1F *E_depy5 = nullptr;


TH1F *hx1 = nullptr;
TH1F *hx2 = nullptr;
TH1F *hx3 = nullptr;
TH1F *hx4 = nullptr;
TH1F *hx5 = nullptr;

TH1F *hy1 = nullptr;
TH1F *hy2 = nullptr;
TH1F *hy3 = nullptr;
TH1F *hy4 = nullptr;
TH1F *hy5 = nullptr;


TCanvas *c_scartix = nullptr;
TCanvas *c_scartiy = nullptr;
TCanvas *c_energiax = nullptr;
TCanvas *c_energiay = nullptr;


int prova=1000000;


vector<double> mu_bkgx;
vector<double> sigma_bkgx;
vector<double> mu_sglx;
vector<double> sigma_sglx;

vector<double> mu_bkgy;
vector<double> sigma_bkgy;
vector<double> mu_sgly;
vector<double> sigma_sgly;





void estrai(){

    TFile *file = TFile::Open("dati_Pieri_Sara.root", "READ"); //apro il file in modalità lettura
    if(!file || file->IsZombie()) cout<<"File non trovato!"<<endl;

    file->GetObject("tree", t);
    if(!t){
        cout<<"Tree non trovato"<<endl;
        file->Close();
    }

    t->SetBranchAddress("X", X);
    t->SetBranchAddress("Edep", Edep);

    /*for(Int_t i=0; i<t->GetEntries(); i++){
        t->GetEntry(i);
        if(i<5){ //stampo le prime 5 entries per visualizzazione
            cout<<"Entry "<<i<<endl;
            cout<<"Coordinata X: ";
            for(Int_t ii=0; ii<10; ii++){
                cout<<X[0][ii]<<", ";
            }
            cout<<"\nCoordinata Y: ";
            for(Int_t ii=0; ii<10; ii++){
                cout<<X[1][ii]<<", ";
            }
            cout<<"\nCoordinata Z: ";
            for(Int_t ii=0; ii<10; ii++){
                cout<<X[2][ii]<<", ";
            }
            cout<<"\n-------------- || ----------------\n"<<endl;
        }
    }*/
}










void energy(){
    int num = t->GetEntries();

    E_depx1 =new TH1F("E_depx1", "Energia depositata layer 1, coordinata X", sqrt(num), 0, 400000);
    E_depx2 =new TH1F("E_depx2", "Energia depositata layer 2, coordinata X", sqrt(num), 0, 400000);
    E_depx3 =new TH1F("E_depx3", "Energia depositata layer 3, coordinata X", sqrt(num), 0, 400000);
    E_depx4 =new TH1F("E_depx4", "Energia depositata layer 4, coordinata X", sqrt(num), 0, 400000);
    E_depx5 =new TH1F("E_depx5", "Energia depositata layer 5, coordinata X", sqrt(num), 0, 400000);

    E_depy1 =new TH1F("E_depy1", "Energia depositata layer 1, coordinata Y", sqrt(num), 0, 400000);
    E_depy2 =new TH1F("E_depy2", "Energia depositata layer 2, coordinata Y", sqrt(num), 0, 400000);
    E_depy3 =new TH1F("E_depy3", "Energia depositata layer 3, coordinata Y", sqrt(num), 0, 400000);
    E_depy4 =new TH1F("E_depy4", "Energia depositata layer 4, coordinata Y", sqrt(num), 0, 400000);
    E_depy5 =new TH1F("E_depy5", "Energia depositata layer 5, coordinata Y", sqrt(num), 0, 400000);

    for(Int_t i=0; i<t->GetEntries(); i++){
        t->GetEntry(i);
        //if(i%1000==0) cout<<"ciclo "<<i<<endl;
        for(int k=0; k<10; k++){
            if(X[2][k]==0.){
                E_depx1->Fill(Edep[0][k]);
                E_depy1->Fill(Edep[1][k]);
            } 
            if(X[2][k]==250.){
                E_depx2->Fill(Edep[0][k]);
                E_depy2->Fill(Edep[1][k]);
            }
            if(X[2][k]==500.){
                E_depx3->Fill(Edep[0][k]);
                E_depy3->Fill(Edep[1][k]);
            }
            if(X[2][k]==750.){
                E_depx4->Fill(Edep[0][k]);
                E_depy4->Fill(Edep[1][k]);
            }
            if(X[2][k]==1000.){
                E_depx5->Fill(Edep[0][k]);
                E_depy5->Fill(Edep[1][k]);
            }
        }    
    }

    c_energiax = new TCanvas("c_energiax", "c_energiax", 3000, 600);
    c_energiax->Divide(5,1);

    c_energiay = new TCanvas("c_energiay", "c_energiay", 3000, 600);
    c_energiay->Divide(5,1);

    TH1F *h_listx[5] = {E_depx1, E_depx2, E_depx3, E_depx4, E_depx5};
    TH1F *h_listy[5] = {E_depy1, E_depy2, E_depy3, E_depy4, E_depy5};

    for(int j=0; j<5; j++){
        TF1 *f_bkg = new TF1("f_bkg", "landau", 0, 60000);
        f_bkg->SetParameters(100000, 10000, 3000);
        f_bkg->SetLineColor(kBlue);

        TF1 *f_sgl = new TF1("f_sgl", "landau", 50000, 400000);
        f_sgl->SetParameters(500000, 120000, 15000);

    //COORDINATA X
        c_energiax->cd(j+1);
        gPad->SetLogy(1);
        h_listx[j]->SetFillColor(kViolet-4);
        h_listx[j]->SetLineColor(kViolet-4);
        h_listx[j]->Draw("hist");
        if(j<3){
            h_listx[j]->Fit(f_bkg, "RQ");
            mu_bkgx.push_back(f_bkg->GetParameter(1));
            sigma_bkgx.push_back(f_bkg->GetParameter(2));
            f_bkg->Draw("same");
        }
        else{
            mu_bkgx.push_back(-999);
        }
        h_listx[j]->Fit(f_sgl, "RQ +");
        mu_sglx.push_back(f_sgl->GetParameter(1));
        sigma_sglx.push_back(f_sgl->GetParameter(2));
        f_sgl->Draw("same");

    //COORDINATA Y
        c_energiay->cd(j+1);
        gPad->SetLogy(1);
        h_listy[j]->SetFillColor(kGreen-4);
        h_listy[j]->SetLineColor(kGreen-4);
        h_listy[j]->Draw("hist");
        if(j<3){
            h_listy[j]->Fit(f_bkg, "RQ");
            mu_bkgy.push_back(f_bkg->GetParameter(1));
            sigma_bkgy.push_back(f_bkg->GetParameter(2));
            f_bkg->Draw("same");
        }
        else{
            mu_bkgy.push_back(-999);
        }
        h_listy[j]->Fit(f_sgl, "RQ +");
        mu_sgly.push_back(f_sgl->GetParameter(1));
        sigma_sgly.push_back(f_sgl->GetParameter(2));
        f_sgl->Draw("same");

    }


    /*cout<<"Medie Landau per rumore (background): "<<endl;
    for(int i=0; i<5; i++){
        cout<<mu_bkgx[i]<<", ";
    }*/

    /*cout<<"\nMedie Landau per segnale (signal): "<<endl;
    for(int i=0; i<5; i++){
        cout<<mu_sglx[i]<<", ";
    }
    cout<<"\nSigma Landau per segnale (signal): "<<endl;
    for(int i=0; i<5; i++){
        cout<<sigma_sglx[i]<<", ";
    }*/
    
}




void fit_x(){
    vector<double> z;
    vector<double> x;
    vector<double> e_dep;
      
    if(!t) cout<<"Nessun tree trovato"<<endl;

    if(gDirectory->FindObject("hx1")) delete gDirectory->FindObject("hx1");
    if(gDirectory->FindObject("hx2")) delete gDirectory->FindObject("hx2");
    if(gDirectory->FindObject("hx3")) delete gDirectory->FindObject("hx3");
    if(gDirectory->FindObject("hx4")) delete gDirectory->FindObject("hx4");
    if(gDirectory->FindObject("hx5")) delete gDirectory->FindObject("hx5");
    if(gDirectory->FindObject("c_scartix")) delete gDirectory->FindObject("c_scartix");

    hx1 = new TH1F("hx1", "Layer 1: scarti", sqrt(prova), -2,2);
    hx2 = new TH1F("hx2", "Layer 2: scarti", sqrt(prova), -2,2);
    hx3 = new TH1F("hx3", "Layer 3: scarti", sqrt(prova), -2,2);
    hx4 = new TH1F("hx4", "Layer 4: scarti", sqrt(prova), -2,2);
    hx5 = new TH1F("hx5", "Layer 5: scarti", sqrt(prova), -2,2); 


    double cov, var0, var1;
    bool cov_mat = false;
    int good_events1=0, good_events2=0, good_events3=0, good_events4=0, good_events5=0, good_events_tot=0;
    for(Int_t i=0; i<t->GetEntries(); i++){
        t->GetEntry(i);
        //if(i%1000==0) cout<<"ciclo "<<i<<endl;
        if(i < prova){

            z.clear();
            x.clear();
            e_dep.clear();
                        
            int punto1=0;
            int z1=0, z2=0, z3=0, z4=0, z5=0;
            for(int p=0; p<10; p++){
                if(X[2][p]==-999 || X[0][p]==-999) continue;
                z.push_back(X[2][p]);
                x.push_back(X[0][p]);
                punto1++;
                e_dep.push_back(Edep[0][p]);

                if(X[2][p]==0.) z1++;
                if(X[2][p]==250.) z2++;
                if(X[2][p]==500.) z3++;
                if(X[2][p]==750.) z4++;
                if(X[2][p]==1000.) z5++;

                //cout<<Form("Punto %d: (", punto1+1)<<z<<", "<<x<<")"<<endl;
            }

            int z_counts[5] = {z1, z2, z3, z4, z5};
            double z_coords[5] = {0., 250., 500., 750., 1000.};

            /*TGraph *g = new TGraph();
            g->SetMarkerStyle(20);
            g->SetMarkerColor(kGreen+2);
            TGraph *g_tot = new TGraph();
            g_tot->SetTitle(Form("Event #%d track; z(mm); x(mm)", i));
            g_tot->SetMarkerStyle(20);
            g_tot->SetMarkerColor(kRed);
            for(int gg=0; gg<z.size(); gg++){
                g_tot->SetPoint(gg, z[gg], x[gg]);
                g->SetPoint(gg, z[gg], x[gg]);
            }*/

            //controllo per ripulire la traccia dagli hit di rumore singoli 
            vector<int> indx_rm;
            for(int k=0; k<5; k++){
                if(z_counts[k] == 1){
                    for(int kk=0; kk<z.size(); kk++){
                        if(z[kk]==z_coords[k]){
                            double prob_sgl = TMath::Landau(e_dep[kk], mu_sglx[k], sigma_sglx[k]);
                            double prob_bkg = TMath::Landau(e_dep[kk], mu_bkgx[k], sigma_bkgx[k]);
                            double threshold = 100.0;
                            if(prob_bkg > prob_sgl && (prob_bkg/prob_sgl) > threshold){ //se è più probabile che faccia parte del background rimuoviamo l'hit
                                //cout<<"Hit singolo di rumore rimosso. Evento "<<i<<", energia depositata: "<<e_dep[kk]<<" eV"<<endl;
                                //cout<<"Prob sgl: "<<prob_sgl<<"\nProb bkg: "<<prob_bkg<<endl;
                                indx_rm.push_back(kk);                                
                            }
                            break;
                        }
                    }
                    
                }
            }
            

            if(indx_rm.size()>0){
                std::sort(indx_rm.rbegin(), indx_rm.rend()); //ordino in ordine decrescente il vettore degli indici così non si sfasano i vector quando tolgo i punti
                for(int p=0; p < indx_rm.size(); p++){
                    //g->RemovePoint(indx_rm[p]);
                    x.erase(x.begin()+indx_rm[p]);
                    z.erase(z.begin()+indx_rm[p]);
                    e_dep.erase(e_dep.begin()+indx_rm[p]);
                }
            }
            indx_rm.clear();


            
            /*TCanvas *cg = new TCanvas("cg", "cg", 800, 600);
            g_tot->GetXaxis()->SetLimits(-100.0, 1100.0);
            //g_tot->GetYaxis()->SetRangeUser(-20.0, 20.0);
            g_tot->Draw("AP");
            g->Draw("P");*/


            int new_z1 = 0, new_z2 = 0, new_z3 = 0, new_z4 = 0, new_z5 = 0;
            for(int p = 0; p < z.size(); p++){
                if(z[p] == 0.) new_z1++;
                else if(z[p] == 250.) new_z2++;
                else if(z[p] == 500.) new_z3++;
                else if(z[p] == 750.) new_z4++;
                else if(z[p] == 1000.) new_z5++;
            }
            int z_counts_clean[5] = {new_z1, new_z2, new_z3, new_z4, new_z5};

            int double_hit_layers = 0;
            int triple_hit_layers = 0;
            for(int ii = 0; ii < 5; ii++){ 
                if(z_counts_clean[ii] == 2) double_hit_layers++;
                if(z_counts_clean[ii] == 3) triple_hit_layers++;
            }

            vector<int> indx_rm_doubles; //vector che raccoglie tutti gli indici da rimuovere

            if(double_hit_layers == 1 && triple_hit_layers == 0){// c'è un doppio hit su un solo layer
                for(int ii=0; ii<5; ii++){
                    if(z_counts_clean[ii]==2) {
                        vector<int> pos;
                        pos=single_double_hit(&z_coords[ii], z, x, e_dep, ii, mu_sglx, sigma_sglx); 
                        //ottengo la posizione (indice) dell'hit peggiore all'interno dei vector
                        //cout<<"Pos: "<<pos<<endl;
                        if(pos.size()==2){ //i metodi sono discordi quindi elimino entrmabi i punti
                            //cout << "=== ERRORE NELL'EVENTO " << i << " === METODI DISCORDI" << endl;
                            //i vettori vengono svuotati all'interno della funzione per praticità
                        }
                        for(int kk=0; kk<pos.size(); kk++){
                            indx_rm_doubles.push_back(pos[kk]);
                        }   
                        break;                  
                   }
                }

            }
            


            
            else if(double_hit_layers > 1 || triple_hit_layers > 0){ //numero di doppi/tripli hit indefinito
                vector<int> pos;
                pos = gen_double_hit(z_counts_clean, z_coords, z, x, e_dep, mu_sglx, sigma_sglx);
                //cout<<"Doppio/triplo hit. Evento "<<i<<endl;
                for(int k=0; k<pos.size(); k++){
                    indx_rm_doubles.push_back(pos[k]); 
                }
                //cout<<"\n\nPosizioni degli hit da scartare: "<<pos.size()<<" doppi hit"<<endl;
                //cout<<"Evento: "<<i<<endl;
            }

            std::sort(indx_rm_doubles.rbegin(), indx_rm_doubles.rend());
            for(int ii=0; ii<indx_rm_doubles.size(); ii++){
                //cout<<pos[ii]<<": ("<<z[pos[ii]]<<", "<<x[pos[ii]]<<")"<<endl;
                z.erase(z.begin()+indx_rm_doubles[ii]);
                x.erase(x.begin()+indx_rm_doubles[ii]);
                e_dep.erase(e_dep.begin()+indx_rm_doubles[ii]);
            }
            


            
            
            if (z.size() < 3){ //traccia incompleta non mi permette di effettuare un buon fit leave-one-out
                //cout<<"Punti insufficienti per fit. Evento "<<i<<endl;
                z.clear();
                x.clear();
                e_dep.clear();
                continue; 
            }

            //Fit
            //TCanvas *c2= new TCanvas("c2", "c2", 3000, 600);
            //c2->Divide(punto1, 1);
            int fit=0;            
            for(Int_t j=0; j<z.size(); j++){
                Double_t z_reale = 0, x_reale = 0;
                z_reale=z[j];
                x_reale=x[j];
                
                TLinearFitter lf(1, "pol1");
                for(int jj=0; jj<z.size(); jj++){
                    if(jj==j) continue;
                    lf.AddPoint(&z[jj],x[jj]);
                }
                lf.Eval(); //esegue il fit
                int status = lf.Eval(); 
                if(status != 0) {
                    continue; //se il fit fallisce
                }
                fit++;

                double p0 = lf.GetParameter(0);
                double p1 = lf.GetParameter(1);
                double x = p0 + p1*z_reale;

                Double_t scarto = x - x_reale;
                //cout<<"Scarto: "<<scarto<<endl;

                if(cov_mat==false){ //la matrice di covarianza è la stessa sempre, perché non dipende dalle x, quindi basta calcolarla una volta

                    double chi2 = lf.GetChisquare();
                    double ndf = lf.GetNpoints() - 2;
                    double scale;
                    if(!chi2 || ndf<0) scale = 1.;
                    else scale = chi2/ndf;

                    cov = lf.GetCovarianceMatrixElement(0, 1) * scale;
                    var1 = lf.GetCovarianceMatrixElement(1, 1) * scale;
                    var0 = lf.GetCovarianceMatrixElement(0, 0) * scale;
                    cov_mat = true;
                }
                
                if(z_reale == 0.){
                    hx1->Fill(scarto);
                    good_events1++;
                }
                if(z_reale == 250.){
                    hx2->Fill(scarto);
                    good_events2++;
                }
                if(z_reale == 500.){
                    hx3->Fill(scarto);
                    good_events3++;
                }
                if(z_reale == 750.){
                    hx4->Fill(scarto);
                    good_events4++;
                }
                if(z_reale == 1000.){
                    hx5->Fill(scarto);
                    good_events5++;
                }
                
            }
            if(fit > 2)   good_events_tot++; //se almeno 3 fit riescono conto l'evento come passaggio di particella
            fit=0;
                  
        }
        
    }
    

    /*c_scartix = new TCanvas("c_scartix", "scartix", 3000, 600);
    c_scartix->Divide(5,1);
    c_scartix->cd(1);
    hx1->Draw("hist");
    c_scartix->cd(2);
    hx2->Draw("hist");
    c_scartix->cd(3);
    hx3->Draw("hist");
    c_scartix->cd(4);
    hx4->Draw("hist");
    c_scartix->cd(5);
    hx5->Draw("hist");*/

    Double_t var_res[5]; //varianza del residuo
    Double_t sigma_res_err[5]; //errore sulla sigma del residuo
    TH1F *hists[5] = {hx1, hx2, hx3, hx4, hx5};
    for(int k=0; k<5; k++){
        hists[k]->Fit("gaus", "Q");
        TF1 *f_gaus = hists[k]->GetFunction("gaus");
        var_res[k] = (f_gaus->GetParameter(2))*(f_gaus->GetParameter(2));
        sigma_res_err[k] = f_gaus->GetParError(2); 

        cout << Form("--- Layer %d Diagnostics ---", k+1) << endl;
        cout << "Numero di entry nell'istogramma: " << hists[k]->GetEntries() << endl;
        cout << "Sigma estratta: " << f_gaus->GetParameter(2) << endl;
        cout << "Errore riportato da ROOT: " << f_gaus->GetParError(2) << endl;
        cout << "Chi2 / NDF: " << f_gaus->GetChisquare() / f_gaus->GetNDF() << endl;
    }
    /*Nota bene: err_sigma = sigma/sqrt(2*N) dalla propagazione degli errori
    se N è dell'ordine di 10^4 o più, e la sigma estratta dal fit gaussiano è dell'ordine di 0.03... (10^-2)
    10^-2/sqrt(2)*10^2 ci sta che dia errori dell'ordine di 10^-4 o 10^-5*/

    

    Double_t var_fit[5]; //varianza del fit -> propagazione degli errori
    var_fit[0]=var0;
    var_fit[1]=var0 + 250.*250.*var1 + 2*250.*cov;
    var_fit[2]=var0 + 500.*500.*var1 + 2*500.*cov;
    var_fit[3]=var0 + 750.*750.*var1 + 2*750.*cov;
    var_fit[4]=var0 + 1000.*1000.*var1 + 2*1000.*cov;

    /*cout<<"Verifichiamo che la varianza del fit sia << della varianza del residuo"<<endl;
    for(int i=0; i<5; i++){
        cout<<"Fit: "<<var_fit[i]<<"\nRes: "<<var_res[i]<<endl;
        cout<<"   Res è "<<(var_fit[i]/var_res[i])*100<<" volte maggiore di Fit"<<endl;
    }*/


    double ris_err[5];
    Double_t ris[5];
    for(int i =0; i<5; i++){
        ris[i]=sqrt(var_res[i] - var_fit[i]);
        ris_err[i] = (sqrt(var_res[i])/ris[i])*sigma_res_err[i];
    }

    cout<<"\n\nRisoluzione del tracciatore per layer (COORDINATA X)"<<endl;
    for(int i =0; i<5; i++){
        cout<<Form("Layer %d (X): ", i+1)<<ris[i]<<" +- "<<ris_err[i]<<endl;
        cout<<"   res: "<<sqrt(var_res[i])<<" +- "<<sigma_res_err[i]<<"\n   fit: "<<sqrt(var_fit[i])<<endl;
    }

    double ris_tot=0;
    double w_tot=0;
    for(int r=0; r<5; r++){
        ris_tot = ris_tot + ris[r]*1/(ris_err[r]*ris_err[r]); 
        w_tot = w_tot + 1/(ris_err[r]*ris_err[r]); 
    }
    ris_tot = ris_tot/w_tot;

    cout<<"\nRisoluzione pesata sui layer: "<<ris_tot<<" +- "<<sqrt(1/w_tot)<<endl;

    /*cout<<"\nEventi buoni (passaggio particella): "<<good_events_tot<<endl;
    cout<<"\nEventi buoni per layer (X)"<<endl;
    cout<<"Layer 1: "<<good_events1<<endl;
    cout<<"Layer 2: "<<good_events2<<endl;
    cout<<"Layer 3: "<<good_events3<<endl;
    cout<<"Layer 4: "<<good_events4<<endl;
    cout<<"Layer 5: "<<good_events5<<endl;
    
    double eff[5], sigma_eff[5];
    cout<<"\nEfficienza per layer (X)"<<endl;

    eff[0]=(double) good_events1/(double) good_events_tot;
    sigma_eff[0] = sqrt(eff[0]*(1.-eff[0])/(double) good_events_tot);
    cout<<"Layer 1: "<<eff[0]<<" +- "<<sigma_eff[0]<<", "<<eff[0]*100<<"%"<<endl;

    eff[1]=(double) good_events2/(double) good_events_tot;
    sigma_eff[1] = sqrt(eff[1]*(1.-eff[1])/(double) good_events_tot);
    cout<<"Layer 2: "<<eff[1]<<" +- "<<sigma_eff[1]<<", "<<eff[1]*100<<"%"<<endl;

    eff[2]=(double) good_events3/(double) good_events_tot;
    sigma_eff[2] = sqrt(eff[2]*(1.-eff[2])/(double) good_events_tot);
    cout<<"Layer 3: "<<eff[2]<<" +- "<<sigma_eff[2]<<", "<<eff[2]*100<<"%"<<endl;

    eff[3]=(double) good_events4/(double) good_events_tot;
    sigma_eff[3] = sqrt(eff[3]*(1.-eff[3])/(double) good_events_tot);
    cout<<"Layer 4: "<<eff[3]<<" +- "<<sigma_eff[3]<<", "<<eff[3]*100<<"%"<<endl;

    eff[4]=(double) good_events5/(double) good_events_tot;
    sigma_eff[4] = sqrt(eff[4]*(1.-eff[4])/(double) good_events_tot);
    cout<<"Layer 5: "<<eff[4]<<" +- "<<sigma_eff[4]<<", "<<eff[4]*100<<"%"<<endl;*/


}


void fit_y(){
    vector<double> z;
    vector<double> y;
    vector<double> e_dep;
      
    if(!t) cout<<"Nessun tree trovato"<<endl;

    if(gDirectory->FindObject("hy1")) delete gDirectory->FindObject("hy1");
    if(gDirectory->FindObject("hy2")) delete gDirectory->FindObject("hy2");
    if(gDirectory->FindObject("hy3")) delete gDirectory->FindObject("hy3");
    if(gDirectory->FindObject("hy4")) delete gDirectory->FindObject("hy4");
    if(gDirectory->FindObject("hy5")) delete gDirectory->FindObject("hy5");
    if(gDirectory->FindObject("c_scartiy")) delete gDirectory->FindObject("c_scartiy");

    hy1 = new TH1F("hy1", "Layer 1: scarti", sqrt(prova), -2,2);
    hy2 = new TH1F("hy2", "Layer 2: scarti", sqrt(prova), -2,2);
    hy3 = new TH1F("hy3", "Layer 3: scarti", sqrt(prova), -2,2);
    hy4 = new TH1F("hy4", "Layer 4: scarti", sqrt(prova), -2,2);
    hy5 = new TH1F("hy5", "Layer 5: scarti", sqrt(prova), -2,2); 

    
    double cov, var0, var1;
    bool cov_mat = false;
    int good_events1=0, good_events2=0, good_events3=0, good_events4=0, good_events5=0, good_events_tot=0;
    for(Int_t i=0; i<t->GetEntries(); i++){
        t->GetEntry(i);
        //if(i%1000==0) cout<<"ciclo "<<i<<endl;
        if(i < prova){
            z.clear();
            y.clear();
            e_dep.clear();
                        
            int punto1=0;
            int z1=0, z2=0, z3=0, z4=0, z5=0;
            for(int p=0; p<10; p++){
                if(X[2][p]==-999 || X[1][p]==-999) continue;
                z.push_back(X[2][p]);
                y.push_back(X[1][p]);
                punto1++;
                e_dep.push_back(Edep[1][p]);

                if(X[2][p]==0.) z1++;
                if(X[2][p]==250.) z2++;
                if(X[2][p]==500.) z3++;
                if(X[2][p]==750.) z4++;
                if(X[2][p]==1000.) z5++;

                //cout<<Form("Punto %d: (", punto1+1)<<z<<", "<<x<<")"<<endl;
            }

            int z_counts[5] = {z1, z2, z3, z4, z5};
            double z_coords[5] = {0., 250., 500., 750., 1000.};

            //controllo per ripulire la traccia dagli hit di rumore singoli 
            vector<int> indx_rm;
            for(int k=0; k<5; k++){
                if(z_counts[k] == 1){
                    for(int kk=0; kk<z.size(); kk++){
                        if(z[kk]==z_coords[k]){
                            double prob_sgl = TMath::Landau(e_dep[kk], mu_sgly[k], sigma_sgly[k]);
                            double prob_bkg = TMath::Landau(e_dep[kk], mu_bkgy[k], sigma_bkgy[k]);
                            double threshold = 100.0;
                            if(prob_bkg > prob_sgl && (prob_bkg/prob_sgl) > threshold){ //se è più probabile che faccia parte del background rimuoviamo l'hit
                                //cout<<"Hit singolo di rumore rimosso. Evento "<<i<<", energia depositata: "<<e_dep[kk]<<endl;
                                //cout<<"Prob sgl: "<<prob_sgl<<"\nProb bkg: "<<prob_bkg<<endl;
                                indx_rm.push_back(kk);                                
                            }
                            break;
                        }
                    }
                    
                }
            }

            if(indx_rm.size()>0){
                for(int p=0; p < indx_rm.size(); p++){
                    y.erase(y.begin()+indx_rm[p]);
                    z.erase(z.begin()+indx_rm[p]);
                    e_dep.erase(e_dep.begin()+indx_rm[p]);
                }
            }
            indx_rm.clear();

            

            int new_z1 = 0, new_z2 = 0, new_z3 = 0, new_z4 = 0, new_z5 = 0;
            for(int p = 0; p < z.size(); p++){
                if(z[p] == 0.) new_z1++;
                else if(z[p] == 250.) new_z2++;
                else if(z[p] == 500.) new_z3++;
                else if(z[p] == 750.) new_z4++;
                else if(z[p] == 1000.) new_z5++;
            }
            int z_counts_clean[5] = {new_z1, new_z2, new_z3, new_z4, new_z5};

            int double_hit_layers = 0;
            int triple_hit_layers = 0;
            for(int ii = 0; ii < 5; ii++){ 
                if(z_counts_clean[ii] == 2) double_hit_layers++;
                if(z_counts_clean[ii] == 3) triple_hit_layers++;
            }

            vector<int> indx_rm_doubles;

            if(double_hit_layers == 1 && triple_hit_layers == 0){// c'è un doppio hit su un solo layer
                for(int ii=0; ii<5; ii++){
                    if(z_counts_clean[ii]==2) {
                        vector<int> pos;
                        pos=single_double_hit(&z_coords[ii], z, y, e_dep, ii, mu_sgly, sigma_sgly); 
                        //ottengo la posizione (indice) dell'hit peggiore all'interno dei vector
                        //cout<<"Pos: "<<pos<<endl;
                        if(pos.size()==2){ //i metodi sono discordi quindi elimino entrmabi i punti
                            //cout << "=== ERRORE NELL'EVENTO " << i << " === METODI DISCORDI" << endl;
                            //i vettori vengono svuotati all'interno della funzione per praticità
                        }
                        for(int kk=0; kk<pos.size(); kk++){
                            indx_rm_doubles.push_back(pos[kk]);
                        }                     
                   }
                }

            }
            


            
            if(double_hit_layers > 1 || triple_hit_layers > 0){ //numero di doppi/tripli hit indefinito
                vector<int> pos;
                pos = gen_double_hit(z_counts_clean, z_coords, z, y, e_dep, mu_sgly, sigma_sgly);

                for(int k=0; k<pos.size(); k++){
                    indx_rm_doubles.push_back(pos[k]); 
                }
                //cout<<"\n\nPosizioni degli hit da scartare: "<<pos.size()<<" doppi hit"<<endl;
                //cout<<"Evento: "<<i<<endl;
            }

            std::sort(indx_rm_doubles.rbegin(), indx_rm_doubles.rend());
            for(int ii=0; ii<indx_rm_doubles.size(); ii++){
                //cout<<pos[ii]<<": ("<<z[pos[ii]]<<", "<<x[pos[ii]]<<")"<<endl;
                z.erase(z.begin()+indx_rm_doubles[ii]);
                y.erase(y.begin()+indx_rm_doubles[ii]);
                e_dep.erase(e_dep.begin()+indx_rm_doubles[ii]);
            }
            


            
            
            if (z.size() < 3){ //traccia incompleta non mi permette di effettuare un buon fit leave-one-out
                //cout<<"Punti insufficienti per fit. Evento "<<i<<endl;
                z.clear();
                y.clear();
                e_dep.clear();
                continue; 
            }

            //Fit
            //TCanvas *c2= new TCanvas("c2", "c2", 3000, 600);
            //c2->Divide(punto1, 1);
            int fit=0;            
            for(Int_t j=0; j<z.size(); j++){
                Double_t z_reale = 0, y_reale = 0;
                z_reale=z[j];
                y_reale=y[j];
                
                TLinearFitter lf(1, "pol1");
                for(int jj=0; jj<z.size(); jj++){
                    if(jj==j) continue;
                    lf.AddPoint(&z[jj],y[jj]);
                }
                lf.Eval(); // Esegue il fit
                int status = lf.Eval(); 
                if(status != 0) {
                    continue; //Se il fit fallisce
                }
                fit++;

                double p0 = lf.GetParameter(0);
                double p1 = lf.GetParameter(1);
                double y = p0 + p1*z_reale;

                Double_t scarto = y - y_reale;
                //cout<<"Scarto: "<<scarto<<endl;

                if(cov_mat==false){ //la matrice di covarianza è la stessa sempre, perché non dipende dalle x, quindi basta calcolarla una volta
                   
                    double chi2 = lf.GetChisquare();
                    double ndf = lf.GetNpoints() - 2;
                    double scale;
                    if(!chi2 || ndf<0) scale = 1.;
                    else scale = chi2/ndf;

                    cov = lf.GetCovarianceMatrixElement(0, 1) * scale;
                    var1 = lf.GetCovarianceMatrixElement(1, 1) * scale;
                    var0 = lf.GetCovarianceMatrixElement(0, 0) * scale;
                    cov_mat = true;
                }
                
                if(z_reale == 0.){
                    hy1->Fill(scarto);
                    good_events1++;
                }
                if(z_reale == 250.){
                    hy2->Fill(scarto);
                    good_events2++;
                }
                if(z_reale == 500.){
                    hy3->Fill(scarto);
                    good_events3++;
                }
                if(z_reale == 750.){
                    hy4->Fill(scarto);
                    good_events4++;
                }
                if(z_reale == 1000.){
                    hy5->Fill(scarto);
                    good_events5++;
                }
                
            }
            if(fit > 2)   good_events_tot++; //se almeno 3 fit riescono conto l'evento come passaggio di particella
            fit=0;
                  
        }
        
    }
    

    /*c_scartiy = new TCanvas("c_scartiy", "scartix", 3000, 600);
    c_scartiy->Divide(5,1);
    c_scartiy->cd(1);
    hy1->Draw("hist");
    c_scartiy->cd(2);
    hy2->Draw("hist");
    c_scartiy->cd(3);
    hy3->Draw("hist");
    c_scartiy->cd(4);
    hy4->Draw("hist");
    c_scartiy->cd(5);
    hy5->Draw("hist");*/

    Double_t var_res[5]; //varianza del residuo
    Double_t sigma_res_err[5]; //errore sulla sigma del residuo
    TH1F *hists[5] = {hy1, hy2, hy3, hy4, hy5};
    for(int k=0; k<5; k++){
        hists[k]->Fit("gaus", "Q");
        TF1 *f_gaus = hists[k]->GetFunction("gaus");
        var_res[k] = (f_gaus->GetParameter(2))*(f_gaus->GetParameter(2));
        sigma_res_err[k] = f_gaus->GetParError(2); 

        cout << Form("--- Layer %d Diagnostics ---", k+1) << endl;
        cout << "Numero di entry nell'istogramma: " << hists[k]->GetEntries() << endl;
        cout << "Sigma estratta: " << f_gaus->GetParameter(2) << endl;
        cout << "Errore riportato da ROOT: " << f_gaus->GetParError(2) << endl;
        cout << "Chi2 / NDF: " << f_gaus->GetChisquare() / f_gaus->GetNDF() << endl;
    }

    /*Nota bene: err_sigma = sigma/sqrt(2*N) dalla propagazione degli errori
    se N è dell'ordine di 10^4 o più, e la sigma estratta dal fit gaussiano è dell'ordine di 0.03... (10^-2)
    10^-2/sqrt(2)*10^2 ci sta che dia errori dell'ordine di 10^-4 o 10^-5*/

    Double_t var_fit[5]; //varianza del fit
    var_fit[0]=var0;
    var_fit[1]=var0 + 250.*250.*var1 + 2*250.*cov;
    var_fit[2]=var0 + 500.*500.*var1 + 2*500.*cov;
    var_fit[3]=var0 + 750.*750.*var1 + 2*750.*cov;
    var_fit[4]=var0 + 1000.*1000.*var1 + 2*1000.*cov;

    /*cout<<"Verifichiamo che la varianza del fit sia << della varianza del residuo"<<endl;
    for(int i=0; i<5; i++){
        cout<<"Fit: "<<var_fit[i]<<"\nRes: "<<var_res[i]<<endl;
        cout<<"   Res è "<<(var_fit[i]/var_res[i])*100<<" volte maggiore di Fit"<<endl;
    }*/

    double ris_err[5];
    Double_t ris[5];
    for(int i =0; i<5; i++){
        ris[i]=sqrt(var_res[i] - var_fit[i]);
        ris_err[i] = (sqrt(var_res[i])/ris[i])*sigma_res_err[i];
    }

    cout<<"\n\nRisoluzione del tracciatore per layer (COORDINATA Y)"<<endl;
    for(int i =0; i<5; i++){
        cout<<Form("Layer %d (Y): ", i+1)<<ris[i]<<" +- "<<ris_err[i]<<endl;
        cout<<"   res: "<<sqrt(var_res[i])<<" +- "<<sigma_res_err[i]<<"\n   fit: "<<sqrt(var_fit[i])<<endl;
    }

    double ris_tot=0;
    double w_tot=0;
    for(int r=0; r<5; r++){
        ris_tot = ris_tot + (ris[r]/(ris_err[r]*ris_err[r]));
        w_tot = w_tot + (1/(ris_err[r]*ris_err[r]));
    }
    ris_tot = ris_tot/w_tot;

    cout<<"\nRisoluzione pesata sui layer: "<<ris_tot<<" +- "<<sqrt(1/w_tot)<<endl;

    /*cout<<"\nEventi buoni (passaggio particella): "<<good_events_tot<<endl;
    cout<<"\nEventi buoni per layer (Y)"<<endl;
    cout<<"Layer 1: "<<good_events1<<endl;
    cout<<"Layer 2: "<<good_events2<<endl;
    cout<<"Layer 3: "<<good_events3<<endl;
    cout<<"Layer 4: "<<good_events4<<endl;
    cout<<"Layer 5: "<<good_events5<<endl;
    
    double eff[5], sigma_eff[5];
    cout<<"\nEfficienza per layer (Y)"<<endl;

    eff[0]=(double) good_events1/(double) good_events_tot;
    sigma_eff[0] = sqrt(eff[0]*(1.-eff[0])/(double) good_events_tot);
    cout<<"Layer 1: "<<eff[0]<<" +- "<<sigma_eff[0]<<", "<<eff[0]*100<<"%"<<endl;

    eff[1]=(double) good_events2/(double) good_events_tot;
    sigma_eff[1] = sqrt(eff[1]*(1.-eff[1])/(double) good_events_tot);
    cout<<"Layer 2: "<<eff[1]<<" +- "<<sigma_eff[1]<<", "<<eff[1]*100<<"%"<<endl;

    eff[2]=(double) good_events3/(double) good_events_tot;
    sigma_eff[2] = sqrt(eff[2]*(1.-eff[2])/(double) good_events_tot);
    cout<<"Layer 3: "<<eff[2]<<" +- "<<sigma_eff[2]<<", "<<eff[2]*100<<"%"<<endl;

    eff[3]=(double) good_events4/(double) good_events_tot;
    sigma_eff[3] = sqrt(eff[3]*(1.-eff[3])/(double) good_events_tot);
    cout<<"Layer 4: "<<eff[3]<<" +- "<<sigma_eff[3]<<", "<<eff[3]*100<<"%"<<endl;

    eff[4]=(double) good_events5/(double) good_events_tot;
    sigma_eff[4] = sqrt(eff[4]*(1.-eff[4])/(double) good_events_tot);
    cout<<"Layer 5: "<<eff[4]<<" +- "<<sigma_eff[4]<<", "<<eff[4]*100<<"%"<<endl;*/


}







void DoAll(){
    estrai();
    cout<<"Analisi dell'energia depositata iniziata..."<<endl;
    energy();
    cout<<"Analisi dell'energia depositata conclusa."<<endl;
    cout<<"\nInizio analisi delle tracce per coordinata X ..."<<endl;
    fit_x();
    cout<<"\nInizio analisi delle tracce per coordinata Y ..."<<endl;
    fit_y();

}


/*
10000 eventi
Layer 1: 0.439885
   res: 0.193626
   fit: 0.000126517
Layer 2: 0.564209
   res: 0.3184
   fit: 6.81244e-05
Layer 3: 0.611646
   res: 0.374159
   fit: 4.86603e-05
Layer 4: 0.559028
   res: 0.312581
   fit: 6.81244e-05
Layer 5: 0.444428
   res: 0.197643
   fit: 0.000126517




100000 eventi
Layer 1: 0.446602
   res: 0.19958
   fit: 0.000126517
Layer 2: 0.56159
   res: 0.315451
   fit: 6.81244e-05
Layer 3: 0.592656
   res: 0.35129
   fit: 4.86603e-05
Layer 4: 0.553832
   res: 0.306798
   fit: 6.81244e-05
Layer 5: 0.442821
   res: 0.196217
   fit: 0.000126517




Medie Landau per segnale (signal): 
86295.1, 85950.9, 86016.2, 86356.1, 86361.7


------------- per 10.000 eventi -------------
Eventi buoni (passaggio particella): 9891
Efficienza per layer (X)
Layer 1: 0.989789 +- 0.00101086, 98.9789%
Layer 2: 0.876858 +- 0.00330406, 87.6858%
Layer 3: 0.954605 +- 0.00209312, 95.4605%
Layer 4: 0.836316 +- 0.00372022, 83.6316%
Layer 5: 0.910323 +- 0.00287289, 91.0323%

Eventi buoni (passaggio particella): 9876
Efficienza per layer (Y)
Layer 1: 0.990482 +- 0.000977026, 99.0482%
Layer 2: 0.874949 +- 0.00332846, 87.4949%
Layer 3: 0.956359 +- 0.00205574, 95.6359%
Layer 4: 0.835865 +- 0.00372716, 83.5865%
Layer 5: 0.915148 +- 0.00280406, 91.5148%



------------ per 100.000 eventi -------------
Eventi buoni (passaggio particella): 98803
Efficienza per layer (X)
Layer 1: 0.98997 +- 0.000317013, 98.997%
Layer 2: 0.876522 +- 0.00104663, 87.6522%
Layer 3: 0.954769 +- 0.000661126, 95.4769%
Layer 4: 0.835835 +- 0.00117846, 83.5835%
Layer 5: 0.913363 +- 0.000894929, 91.3363%

Eventi buoni (passaggio particella): 98777
Efficienza per layer (Y)
Layer 1: 0.99021 +- 0.000313272, 99.021%
Layer 2: 0.873675 +- 0.00105704, 87.3675%
Layer 3: 0.955597 +- 0.000655414, 95.5597%
Layer 4: 0.836592 +- 0.00117643, 83.6592%
Layer 5: 0.914069 +- 0.000891736, 91.4069%


*/