/*Codice che permette di visualizzare gli istogrammi relativi all'energia depositata, calcolando anche il valore d'aspettazione del numero di hit di rumore
per evento, in ambo le modalità discusse.
In questo codice si studiano anche le frequenze dei vari casi di hit multipli e hit di rumore che si riscontrano
nel processo di selezione delle tracce.

Esecuzione:
.L hits.cpp+
.L statistica.cpp+
DoAll()*/

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
#include <TLegend.h> 
#include <TStyle.h> 
#include "hits_header.h"


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


TH1F *E_depx_tot = nullptr;
TH1F *E_depy_tot = nullptr;



int prova=1000000;


using namespace std;

vector<double> z;
vector<double> x;
vector<double> e_dep;

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
}










void energy(){
    int num = t->GetEntries();

    E_depx1 =new TH1F("E_depx1", "Energia depositata layer 1, coordinata X; Energia (eV); log(Entries)", sqrt(num), 0, 400000);
    E_depx2 =new TH1F("E_depx2", "Energia depositata layer 2, coordinata X; Energia (eV); log(Entries)", sqrt(num), 0, 400000);
    E_depx3 =new TH1F("E_depx3", "Energia depositata layer 3, coordinata X; Energia (eV); log(Entries)", sqrt(num), 0, 400000);
    E_depx4 =new TH1F("E_depx4", "Energia depositata layer 4, coordinata X; Energia (eV); log(Entries)", sqrt(num), 0, 400000);
    E_depx5 =new TH1F("E_depx5", "Energia depositata layer 5, coordinata X; Energia (eV); log(Entries)", sqrt(num), 0, 400000);

    E_depy1 =new TH1F("E_depy1", "Energia depositata layer 1, coordinata Y; Energia (eV); log(Entries)", sqrt(num), 0, 400000);
    E_depy2 =new TH1F("E_depy2", "Energia depositata layer 2, coordinata Y; Energia (eV); log(Entries)", sqrt(num), 0, 400000);
    E_depy3 =new TH1F("E_depy3", "Energia depositata layer 3, coordinata Y; Energia (eV); log(Entries)", sqrt(num), 0, 400000);
    E_depy4 =new TH1F("E_depy4", "Energia depositata layer 4, coordinata Y; Energia (eV); log(Entries)", sqrt(num), 0, 400000);
    E_depy5 =new TH1F("E_depy5", "Energia depositata layer 5, coordinata Y; Energia (eV); log(Entries)", sqrt(num), 0, 400000);

    E_depx_tot =new TH1F("E_depx_tot", "Energia depositata coordinata X; Energia (eV); log(Entries)", sqrt(num), 0, 400000);
    E_depy_tot =new TH1F("E_depy_tot", "Energia depositata coordinata Y; Energia (eV); log(Entries)", sqrt(num), 0, 400000);

    for(Int_t i=0; i<t->GetEntries(); i++){
        t->GetEntry(i);
        //if(i%1000==0) cout<<"ciclo "<<i<<endl;
        for(int k=0; k<10; k++){
            if(X[2][k] == -999) continue;

            E_depx_tot->Fill(Edep[0][k]);
            E_depy_tot->Fill(Edep[1][k]);
            
            if(X[2][k]==0.){
                E_depx1->Fill(Edep[0][k]);
                E_depy1->Fill(Edep[1][k]);
            } 
            else if(X[2][k]==250.){
                E_depx2->Fill(Edep[0][k]);
                E_depy2->Fill(Edep[1][k]);
            }
            else if(X[2][k]==500.){
                E_depx3->Fill(Edep[0][k]);
                E_depy3->Fill(Edep[1][k]);
            }
            else if(X[2][k]==750.){
                E_depx4->Fill(Edep[0][k]);
                E_depy4->Fill(Edep[1][k]);
            }
            else if(X[2][k]==1000.){
                E_depx5->Fill(Edep[0][k]);
                E_depy5->Fill(Edep[1][k]);
            }
            else continue;
            
        }    
    }

    //TCanvas *c_x[5];
    //gStyle->SetOptStat("ne"); 
    //gStyle->SetOptFit(0);

    TH1F *h_listx[5] = {E_depx1, E_depx2, E_depx3, E_depx4, E_depx5};
    TH1F *h_listy[5] = {E_depy1, E_depy2, E_depy3, E_depy4, E_depy5};

    
    cout<<"\n---------------Coordinata X---------------"<<endl;
    for(int i=0; i<5; i++){
        //c_x[i] = new TCanvas(Form("c_x%d", i+1), Form("c_x%d", i+1), 800, 600);

        if(i<3){
            TF1 *landau_tot = new TF1("landau_tot", "landau(0) + landau(3)", 0, 400000);
            landau_tot->SetParameter(0, h_listx[i]->GetMaximum()*0.5);
            landau_tot->SetParameter(1, 7500);
            landau_tot->SetParameter(2, 7500);

            landau_tot->SetParameter(3, h_listx[i]->GetMaximum()*0.5);
            landau_tot->SetParameter(4, 86000);
            landau_tot->SetParameter(5, 10000);

            h_listx[i]->Fit(landau_tot, "L RQN");
            mu_sglx.push_back(landau_tot->GetParameter(4));
            sigma_sglx.push_back(landau_tot->GetParameter(5));
            mu_bkgx.push_back(landau_tot->GetParameter(1));
            sigma_bkgx.push_back(landau_tot->GetParameter(2));

            //h_listx[i]->SetStats(0);
           /*h_listx[i]->SetFillColor(kViolet-9);
            h_listx[i]->SetLineColor(kViolet-9);
            h_listx[i]->Draw("hist");
            c_x[i]->SetLogy(1);
            landau_tot->Draw("same");*/
        }
        

        else{
            TF1 *f_sgl = new TF1("f_sgl", "landau", 40000, 400000);
            f_sgl->SetParameters(400000, 90000, 10000);

            h_listx[i]->Fit(f_sgl, "RQN");
            mu_sglx.push_back(f_sgl->GetParameter(1));
            sigma_sglx.push_back(f_sgl->GetParameter(2));

            //c_energiax->cd(j+1);
            //c_x[i]->SetLogy(1);
            //h_listx[j]->SetStats(0);
            /*h_listx[i]->SetFillColor(kViolet-9);
            h_listx[i]->SetLineColor(kViolet-9);
            h_listx[i]->Draw("hist");
            f_sgl->Draw("same");*/
        }
        //c_x[i]->Modified();
        //c_x[i]->Update();
        
    }

    //TCanvas *c_energiax = new TCanvas("c_energiax", "c_energiax", 800, 600);
    TF1 *landau_totx = new TF1("landau_totx", "landau(0) + landau(3)", 0, 400000);
    landau_totx->SetParameter(0, E_depx_tot->GetMaximum()*0.5);
    landau_totx->SetParameter(1, 7500);
    landau_totx->SetParameter(2, 7500);

    landau_totx->SetParameter(3, E_depx_tot->GetMaximum()*0.5);
    landau_totx->SetParameter(4, 86000);
    landau_totx->SetParameter(5, 10000);

    E_depx_tot->Fit(landau_totx, "L R N");

    //c_energiax->SetLogy(1);
    E_depx_tot->SetFillColor(kBlue-9);
    E_depx_tot->SetLineColor(kBlue-9);
    E_depx_tot->Draw("hist");
    //landau_totx->Draw("same");


    TF1 *f_noisex = new TF1("f_noisex", "landau", 0, 400000);
    f_noisex->SetParameters(landau_totx->GetParameter(0), landau_totx->GetParameter(1), landau_totx->GetParameter(2));
    f_noisex->Draw("same");

    TF1 *f_signalx = new TF1("f_signalx", "landau", 0, 400000);
    f_signalx->SetParameters(landau_totx->GetParameter(3), landau_totx->GetParameter(4), landau_totx->GetParameter(5));

    //c_energiax->Modified();
    //c_energiax->Update();

    // Calcola l'integrale esatto della curva nell'intervallo dell'istogramma
    double total_noise_eventsx = f_noisex->Integral(0, 400000) / E_depx_tot->GetBinWidth(1);
    double total_signal_eventsx = f_signalx->Integral(0, 400000) / E_depx_tot->GetBinWidth(1);
    std::cout << "Eventi di rumore stimati: " << total_noise_eventsx << std::endl;
    std::cout << "Eventi di segnale stimati: " << total_signal_eventsx << std::endl;






    cout<<"\n---------------Coordinata Y---------------"<<endl;
    for(int i=0; i<3; i++){
        TF1 *landau_tot = new TF1("landau_tot", "landau(0) + landau(3)", 0, 400000);
        landau_tot->SetParameter(0, h_listy[i]->GetMaximum()*0.5);
        landau_tot->SetParameter(1, 7500);
        landau_tot->SetParameter(2, 7500);

        landau_tot->SetParameter(3, h_listy[i]->GetMaximum()*0.5);
        landau_tot->SetParameter(4, 86000);
        landau_tot->SetParameter(5, 10000);

        h_listy[i]->Fit(landau_tot, "L RQ N");
        mu_sgly.push_back(landau_tot->GetParameter(4));
        sigma_sgly.push_back(landau_tot->GetParameter(5));
        mu_bkgy.push_back(landau_tot->GetParameter(1));
        sigma_bkgy.push_back(landau_tot->GetParameter(2));
    }

    
    for(int j=3; j<5; j++){

        TF1 *f_sgl = new TF1("f_sgl", "landau", 40000, 400000);
        f_sgl->SetParameters(500000, 120000, 15000);

        h_listy[j]->Fit(f_sgl, "RQ N");
        mu_sgly.push_back(f_sgl->GetParameter(1));
        sigma_sgly.push_back(f_sgl->GetParameter(2));

        //gPad->SetLogy(1);
        //h_listy[j]->SetStats(0);
        //h_listy[j]->SetFillColor(kGreen-9);
        //h_listy[j]->SetLineColor(kGreen-9);
        //h_listy[j]->Draw("hist");
        //f_sgl->Draw("same");        
        

    }
    //TCanvas *c_energiay = new TCanvas("c_energiay", "c_energiay", 800, 600);

    TF1 *landau_toty = new TF1("landau_toty", "landau(0) + landau(3)", 0, 400000);
    landau_toty->SetParameter(0, E_depy_tot->GetMaximum()*0.5);
    landau_toty->SetParameter(1, 7500);
    landau_toty->SetParameter(2, 7500);

    landau_toty->SetParameter(3, E_depy_tot->GetMaximum()*0.5);
    landau_toty->SetParameter(4, 86000);
    landau_toty->SetParameter(5, 10000);

    E_depy_tot->Fit(landau_toty, "L R N");

    //c_energiay->SetLogy(1);
    E_depy_tot->SetFillColor(kBlue-9);
    E_depy_tot->SetLineColor(kBlue-9);
    //E_depy_tot->Draw("hist");
    //landau_toty->Draw("same");

    //c_energiay->Modified();
    //c_energiay->Update();

    TF1 *f_noisey = new TF1("f_noisey", "landau", 0, 400000);
    f_noisey->SetParameters(landau_toty->GetParameter(0), landau_toty->GetParameter(1), landau_toty->GetParameter(2));

    TF1 *f_signaly = new TF1("f_signaly", "landau", 0, 400000);
    f_signaly->SetParameters(landau_toty->GetParameter(3), landau_toty->GetParameter(4), landau_toty->GetParameter(5));

    // Calcola l'integrale esatto della curva nell'intervallo dell'istogramma
    double total_noise_eventsy = f_noisey->Integral(0, 400000) / E_depy_tot->GetBinWidth(1);
    double total_signal_eventsy = f_signaly->Integral(0, 400000) / E_depy_tot->GetBinWidth(1);
    std::cout << "Eventi di rumore stimati: " << total_noise_eventsy << std::endl;
    std::cout << "Eventi di segnale stimati: " << total_signal_eventsy << std::endl;


    
    

    /*cout<<"\n\nMedie Landau per rumore (background) X: "<<endl;
    for(int i=0; i<3; i++){
        cout<<mu_bkgx[i]<<", ";
    }
    cout<<"\nSigma Landau per rumore (background) X: "<<endl;
    for(int i=0; i<3; i++){
        cout<<sigma_bkgx[i]<<", ";
    }*/

    /*cout<<"\n\nMedie Landau per rumore (background) Y: "<<endl;
    for(int i=0; i<3; i++){
        cout<<mu_bkgy[i]<<", ";
    }
    cout<<"\nSigma Landau per rumore (background) Y: "<<endl;
    for(int i=0; i<3; i++){
        cout<<sigma_bkgy[i]<<", ";
    }*/

    
    /*cout<<"\n\nMedie Landau per segnale (signal) X: "<<endl;
    for(int i=0; i<5; i++){
        cout<<mu_sglx[i]<<", ";
    }
    cout<<"\nSigma Landau per segnale (signal) X: "<<endl;
    for(int i=0; i<5; i++){
        cout<<sigma_sglx[i]<<", ";
    }*/

    /*cout<<"\n\nMedie Landau per segnale (signal) Y: "<<endl;
    for(int i=0; i<5; i++){
        cout<<mu_sgly[i]<<", ";
    }
    cout<<"\nSigma Landau per segnale (signal) Y: "<<endl;
    for(int i=0; i<5; i++){
        cout<<sigma_sgly[i]<<", ";
    }*/

    
}




void fit_x(){
    cout<<"\n\nMedie Landau per rumore (background) X: "<<endl;
    for(int i=0; i<3; i++){
        cout<<mu_bkgx[i]<<", ";
    }
    cout<<"\n\nMedie Landau per segnale (signal) X: "<<endl;
    for(int i=0; i<5; i++){
        cout<<mu_sglx[i]<<", ";
    }
    vector<double> z;
    vector<double> x;
    vector<double> e_dep;

    vector<int> bkg_hit;
      
    if(!t) cout<<"Nessun tree trovato"<<endl;

    TH1I *h_casesx = new TH1I("h_casesx", "Frequenza delle Casistiche (coordinata X); ;Conteggi", 10, 0.5, 10.5);

    h_casesx->GetXaxis()->SetBinLabel(1, "5 hit puliti");
    h_casesx->GetXaxis()->SetBinLabel(2, "1 single hit rimosso");
    h_casesx->GetXaxis()->SetBinLabel(3, "2+ single hit rimossi");
    h_casesx->GetXaxis()->SetBinLabel(4, "1 double hit");
    h_casesx->GetXaxis()->SetBinLabel(5, "2+ double hit");
    h_casesx->GetXaxis()->SetBinLabel(6, "1 triple hit");
    h_casesx->GetXaxis()->SetBinLabel(7, "2+ triple hit");
    h_casesx->GetXaxis()->SetBinLabel(8, "double + triple hit");
    h_casesx->GetXaxis()->SetBinLabel(9, "fit non possibile");
    h_casesx->GetXaxis()->SetBinLabel(10, "rumore su layer 4/5");
    


    for(Int_t i=0; i<t->GetEntries(); i++){
        int bkg_hit_v=0;
        bool traccia_pulita = true;
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

            //controllo per ripulire la traccia dagli hit di rumore singoli 
            vector<int> indx_rm;
            for(int k=0; k<5; k++){
                if(z_counts[k] == 1){
                    for(int kk=0; kk<z.size(); kk++){
                        if(z[kk]==z_coords[k]){
                            double prob_sgl = TMath::Landau(e_dep[kk], mu_sglx[k], sigma_sglx[k]);
                            double prob_bkg = TMath::Landau(e_dep[kk], mu_bkgx[k], sigma_bkgx[k]);
                            double threshold = 10.0;
                            if(prob_bkg > prob_sgl && (prob_bkg/prob_sgl) > threshold){ //se è più probabile che faccia parte del background rimuoviamo l'hit
                                //cout<<"Hit singolo di rumore rimosso. Evento "<<i<<", energia depositata: "<<e_dep[kk]<<endl;
                                //cout<<"\nProb sgl: "<<prob_sgl<<", prob bkg: "<<prob_bkg<<endl;
                                indx_rm.push_back(kk); 
                                if(k==3 || k==4){
                                    h_casesx->Fill(10);
                                    cout<<"Rumore su layer 4 o 5 evento: "<<i<<endl;
                                }                               
                            }
                            break;
                        }
                    }
                    
                }
            }

            if(indx_rm.size()>0){
                std::sort(indx_rm.rbegin(), indx_rm.rend()); //ordino in ordine decrescente il vettore degli indici così non si sfasano i vector quando tolgo i punti
                for(int p=0; p < indx_rm.size(); p++){
                    x.erase(x.begin()+indx_rm[p]);
                    z.erase(z.begin()+indx_rm[p]);
                    e_dep.erase(e_dep.begin()+indx_rm[p]);
                    bkg_hit_v++; 
                }
            }
            if(indx_rm.size()==1){
                h_casesx->Fill(2);   
                traccia_pulita = false;                    
            }
            else if(indx_rm.size() > 2){
                h_casesx->Fill(3);   
                traccia_pulita = false;                    
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

            vector<int> indx_rm_doubles; //vector che raccoglie tutti gli indici da rimuovere

            if(double_hit_layers == 1 && triple_hit_layers == 0){// c'è un doppio hit su un solo layer
                h_casesx->Fill(4);
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
                            if(z_coords[pos[kk]] == 3 || z_coords[pos[kk]] == 4) h_casesx->Fill(10);
                            indx_rm_doubles.push_back(pos[kk]);
                        }   
                        break;                  
                   }
                }

            }
            


            
            else if(double_hit_layers > 1 || triple_hit_layers > 0){ //numero di doppi/tripli hit indefinito
                vector<int> pos;
                pos = gen_double_hit(z_counts_clean, z_coords, z, x, e_dep, mu_sglx, sigma_sglx);

                for(int k=0; k<pos.size(); k++){
                    if(z_coords[pos[k]] == 3 || z_coords[pos[k]] == 4) h_casesx->Fill(10);
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
                bkg_hit_v++;
            }

            if(double_hit_layers>1 && triple_hit_layers==0){
                h_casesx->Fill(5);
                traccia_pulita = false;
            }
            if(double_hit_layers>0 && triple_hit_layers>0){
                h_casesx->Fill(8);
                traccia_pulita = false;
            }
            if(triple_hit_layers==1 && double_hit_layers==0){
                h_casesx->Fill(6);
                traccia_pulita = false;
            }
            if(triple_hit_layers>1 && double_hit_layers==0){
                h_casesx->Fill(7);
                traccia_pulita = false;
            }

            
            
            int new_z12 = 0, new_z22 = 0, new_z32 = 0, new_z42 = 0, new_z52 = 0;
            for(int p = 0; p < z.size(); p++){
                if(z[p] == 0.) new_z12++;
                else if(z[p] == 250.) new_z22++;
                else if(z[p] == 500.) new_z32++;
                else if(z[p] == 750.) new_z42++;
                else if(z[p] == 1000.) new_z52++;
            }
            int z_counts_clean2[5] = {new_z12, new_z22, new_z32, new_z42, new_z52};

            
            
            if (punto1 < 3){ //traccia incompleta non mi permette di effettuare un buon fit leave-one-out
                //cout<<"Punti insufficienti per fit. Evento "<<i<<endl;
                z.clear();
                x.clear();
                h_casesx->Fill(9);
                traccia_pulita = false;
            }


            if(traccia_pulita == true){
                h_casesx->Fill(1);
            }

            bkg_hit.push_back(bkg_hit_v); //aggiungo al vector il conteggio di hit di rumore trovati per l'evento i
            
        }
                  
    }

    /*TCanvas *c_traccex = new TCanvas("c_traccex", "c_traccex", 800, 600);
    h_casesx->SetStats(0);
    h_casesx->SetFillColor(kAzure-3);
    h_casesx->Draw("hist");*/

    double perc[10] = {
        h_casesx->GetBinContent(1)*100/prova, 
        h_casesx->GetBinContent(2)*100/prova, 
        h_casesx->GetBinContent(3)*100/prova, 
        h_casesx->GetBinContent(4)*100/prova,
        h_casesx->GetBinContent(5)*100/prova,
        h_casesx->GetBinContent(6)*100/prova,
        h_casesx->GetBinContent(7)*100/prova,
        h_casesx->GetBinContent(8)*100/prova,
        h_casesx->GetBinContent(9)*100/prova,
        h_casesx->GetBinContent(10)*100/prova,
    };

    cout<<Form("\n\n%d eventi totali recap (coordinata X)", prova)<<endl;;
    cout<<"5 hit puliti: "<<h_casesx->GetBinContent(1)<<", "<< perc[0]<<"%"<<endl;
    cout<<"1 single hit rimosso: "<<h_casesx->GetBinContent(2)<<", "<<perc[1]<<"%"<<endl;
    cout<<"2+ single hit rimossi: "<<h_casesx->GetBinContent(3)<<", "<<perc[2]<<"%"<<endl;
    cout<<"1 double hit: "<<h_casesx->GetBinContent(4)<<", "<<perc[3]<<"%"<<endl;
    cout<<"2+ double hit: "<<h_casesx->GetBinContent(5)<<", "<<perc[4]<<"%"<<endl;
    cout<<"1 triple hit: "<<h_casesx->GetBinContent(6)<<", "<<perc[5]<<"%"<<endl;
    cout<<"2+ triple hit: "<<h_casesx->GetBinContent(7)<<", "<<perc[6]<<"%"<<endl;
    cout<<"double + triple hit: "<<h_casesx->GetBinContent(8)<<", "<<perc[7]<<"%"<<endl;
    cout<<"fit non possibile per troppi pochi punti: "<<h_casesx->GetBinContent(9)<<", "<<perc[8]<<"%"<<endl;
    cout<<"hit di rumore sui layer 4 o 5: "<<h_casesx->GetBinContent(10)<<", "<<perc[9]<<"%"<<endl;


    double mean_bkg=0;
    double sigma_bkg=0;
    TH1I *hit_bkgx = new TH1I("hit_bkgx", "Distribuzione degli hit di rumore su 1.000.000 di eventi (X); N hit; Entries", 6, -0.5, 5.5);
    for(int j=0; j<bkg_hit.size(); j++){
        //mean_bkg = mean_bkg + bkg_hit[j];
        hit_bkgx->Fill(bkg_hit[j]);
    }
    cout<<"\nNumero di eventi con:"<<endl;
    cout<<"0 hit di rumore: "<<hit_bkgx->GetBinContent(1)<<endl;
    cout<<"1 hit di rumore: "<<hit_bkgx->GetBinContent(2)<<endl;
    cout<<"2 hit di rumore: "<<hit_bkgx->GetBinContent(3)<<endl;
    cout<<"3 hit di rumore: "<<hit_bkgx->GetBinContent(4)<<endl;
    cout<<"4 hit di rumore: "<<hit_bkgx->GetBinContent(5)<<endl;
    cout<<"5 hit di rumore: "<<hit_bkgx->GetBinContent(6)<<endl;

    TF1 *f_pois = new TF1("f_pois", "[0] * TMath::Poisson(x, [1])", -0.5, 5.5);

    // Impostiamo dei valori di stima iniziale sensati
    f_pois->SetParameter(0, hit_bkgx->GetEntries()); // Stima iniziale normalizzazione
    f_pois->SetParameter(1, hit_bkgx->GetMean());      // Stima iniziale della media (presa dall'istogramma)

    // Eseguiamo il fit sull'istogramma dei conteggi di rumore
    hit_bkgx->Fit(f_pois, "R N");
    double ymax= f_pois->GetMaximum()*1.2;
    hit_bkgx->SetMaximum(ymax);

    // Estraiamo la media stimata dal fit
    double mean_noise_hits = f_pois->GetParameter(1);
    cout << "Numero medio di hit di rumore per evento (Poisson lambda): " << mean_noise_hits <<endl;


    TCanvas *c_hit_bkgx = new TCanvas("c_hit_bkgx", "c_hit_bkgx", 800, 600);
    hit_bkgx->SetFillColor(kGreen-3);
    hit_bkgx->SetLineColor(kGreen-3);
    hit_bkgx->Draw("hist");
    f_pois->Draw("same");

        
}
    




void fit_y(){
    vector<double> z;
    vector<double> y;
    vector<double> e_dep;

    vector<int> bkg_hity;
      
    if(!t) cout<<"Nessun tree trovato"<<endl;

    TH1I *h_casesy = new TH1I("h_casesy", "Frequenza delle Casistiche (coordinata Y); ;Conteggi", 10, 0.5, 10.5);

    h_casesy->GetXaxis()->SetBinLabel(1, "5 hit puliti");
    h_casesy->GetXaxis()->SetBinLabel(2, "1 single hit rimosso");
    h_casesy->GetXaxis()->SetBinLabel(3, "2+ single hit rimossi");
    h_casesy->GetXaxis()->SetBinLabel(4, "1 double hit");
    h_casesy->GetXaxis()->SetBinLabel(5, "2+ double hit");
    h_casesy->GetXaxis()->SetBinLabel(6, "1 triple hit");
    h_casesy->GetXaxis()->SetBinLabel(7, "2+ triple hit");
    h_casesy->GetXaxis()->SetBinLabel(8, "double + triple hit");
    h_casesy->GetXaxis()->SetBinLabel(9, "fit non possibile");
    h_casesy->GetXaxis()->SetBinLabel(10, "rumore su layer 4 o 5");
    
    
    double cov, var0, var1;
    bool cov_mat = false;
    for(Int_t i=0; i<t->GetEntries(); i++){
        int bkg_hity_v=0;
        t->GetEntry(i);
        bool traccia_pulita = true;
        //if(i%1000==0) cout<<"ciclo "<<i<<endl;
        if(i < prova){
            z.clear();
            y.clear();
            e_dep.clear();
                        
            int punto1=0;
            int null=0;
            int z1=0, z2=0, z3=0, z4=0, z5=0;
            for(int p=0; p<10; p++){
                if(X[2][p]==-999 || X[1][p]==-999){ 
                    null++;
                    continue;
                }
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
            if(null==10)    cout<<"Evento senza hit!!!!"<<endl;

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
                            double threshold = 10.0;
                            if(prob_bkg > prob_sgl && (prob_bkg/prob_sgl) > threshold){ //se è più probabile che faccia parte del background rimuoviamo l'hit
                                //cout<<"Hit singolo di rumore rimosso. Evento "<<i<<", energia depositata: "<<e_dep[kk]<<endl;
                                //cout<<"Prob sgl: "<<prob_sgl<<"\nProb bkg: "<<prob_bkg<<endl;
                                indx_rm.push_back(kk); 
                                if(k==3 || k==4){
                                    h_casesy->Fill(10);
                                }                                 
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
                    bkg_hity_v++; 
                }
            }
            if(indx_rm.size()==1){
                h_casesy->Fill(2);   
                traccia_pulita = false;                    
            }
            else if(indx_rm.size() > 2){
                h_casesy->Fill(3);   
                traccia_pulita = false;                    
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
                h_casesy->Fill(4);
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
                            if(z_coords[pos[kk]] == 3 || z_coords[pos[kk]] == 4) h_casesy->Fill(10);
                            indx_rm_doubles.push_back(pos[kk]);
                        }                    
                   }
                }

            }
            


            
            if(double_hit_layers > 1 || triple_hit_layers > 0){ //numero di doppi/tripli hit indefinito
                vector<int> pos;
                pos = gen_double_hit(z_counts_clean, z_coords, z, y, e_dep, mu_sgly, sigma_sgly);

                for(int k=0; k<pos.size(); k++){
                    if(z_coords[pos[k]] == 3 || z_coords[pos[k]] == 4) h_casesy->Fill(10);
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
                bkg_hity_v++; 
            }



            if(double_hit_layers>1 && triple_hit_layers==0){
                h_casesy->Fill(5);
                traccia_pulita = false;
            }
            if(double_hit_layers>0 && triple_hit_layers>0){
                h_casesy->Fill(8);
                traccia_pulita = false;
            }
            if(triple_hit_layers==1 && double_hit_layers==0){
                h_casesy->Fill(6);
                traccia_pulita = false;
            }
            if(triple_hit_layers>1 && double_hit_layers==0){
                h_casesy->Fill(7);
                traccia_pulita = false;
            }
            
            
            if (punto1 < 3){ //traccia incompleta non mi permette di effettuare un buon fit leave-one-out
                //cout<<"Punti insufficienti per fit. Evento "<<i<<endl;
                z.clear();
                x.clear();
                h_casesy->Fill(9);
                traccia_pulita = false;
                continue; 
            }


            if(traccia_pulita == true){
                h_casesy->Fill(1);
            }

            bkg_hity.push_back(bkg_hity_v); //aggiungo al vector il conteggio di hit di rumore trovati per l'evento i
        }
    }

    /*TCanvas *c_traccey = new TCanvas("c_traccey", "c_traccey", 800, 600);
    h_casesy->SetStats(0);
    h_casesy->SetFillColor(kAzure-3);
    h_casesy->Draw("hist");*/

    double perc[10] = {
        h_casesy->GetBinContent(1)*100/prova, 
        h_casesy->GetBinContent(2)*100/prova, 
        h_casesy->GetBinContent(3)*100/prova, 
        h_casesy->GetBinContent(4)*100/prova,
        h_casesy->GetBinContent(5)*100/prova,
        h_casesy->GetBinContent(6)*100/prova,
        h_casesy->GetBinContent(7)*100/prova,
        h_casesy->GetBinContent(8)*100/prova,
        h_casesy->GetBinContent(9)*100/prova,
        h_casesy->GetBinContent(10)*100/prova,
    };

    cout<<Form("\n\n%d eventi totali recap (coordinata Y)", prova)<<endl;;
    cout<<"5 hit puliti: "<<h_casesy->GetBinContent(1)<<", "<< perc[0]<<"%"<<endl;
    cout<<"1 single hit rimosso: "<<h_casesy->GetBinContent(2)<<", "<<perc[1]<<"%"<<endl;
    cout<<"2+ single hit rimossi: "<<h_casesy->GetBinContent(3)<<", "<<perc[2]<<"%"<<endl;
    cout<<"1 double hit: "<<h_casesy->GetBinContent(4)<<", "<<perc[3]<<"%"<<endl;
    cout<<"2+ double hit: "<<h_casesy->GetBinContent(5)<<", "<<perc[4]<<"%"<<endl;
    cout<<"1 triple hit: "<<h_casesy->GetBinContent(6)<<", "<<perc[5]<<"%"<<endl;
    cout<<"2+ triple hit: "<<h_casesy->GetBinContent(7)<<", "<<perc[6]<<"%"<<endl;
    cout<<"double + triple hit: "<<h_casesy->GetBinContent(8)<<", "<<perc[7]<<"%"<<endl;
    cout<<"fit non possibile per troppi pochi punti: "<<h_casesy->GetBinContent(9)<<", "<<perc[8]<<"%"<<endl;
    cout<<"hit di rumore sui layer 4 o 5: "<<h_casesy->GetBinContent(10)<<", "<<perc[9]<<"%"<<endl;


    
    cout<<"Vettore bkg_hity dimensione: "<<bkg_hity.size()<<endl;
    TH1I *hit_bkgy = new TH1I("hit_bkgy", "Distribuzione degli hit di rumore su 1.000.000 di eventi (Y); N hit; Entries", 6, -0.5, 5.5);
    for(int j=0; j<bkg_hity.size(); j++){
        //mean_bkg = mean_bkg + bkg_hity[j];
        hit_bkgy->Fill(bkg_hity[j]);
    }
    
    TF1 *f_pois2 = new TF1("f_pois2", "[0] * TMath::Poisson(x, [1])", -0.5, 5.5);

    // Impostiamo dei valori di stima iniziale sensati
    f_pois2->SetParameter(0, hit_bkgy->GetEntries()); // Stima iniziale normalizzazione
    f_pois2->SetParameter(1, hit_bkgy->GetMean());      // Stima iniziale della media (presa dall'istogramma)

    // Eseguiamo il fit sull'istogramma dei conteggi di rumore
    hit_bkgy->Fit(f_pois2, "R N");
    double ymax= f_pois2->GetMaximum()*1.2;
    hit_bkgy->SetMaximum(ymax);

    // Estraiamo la media stimata dal fit
    double mean_noise_hits = f_pois2->GetParameter(1);
    cout << "Numero medio di hit di rumore per evento (Poisson lambda): " << mean_noise_hits <<endl;


    TCanvas *c_hit_bkgy = new TCanvas("c_hit_bkgy", "c_hit_bkgy", 800, 600);
    hit_bkgy->SetFillColor(kGreen-3);
    hit_bkgy->SetLineColor(kGreen-3);
    hit_bkgy->Draw("hist");
    f_pois2->Draw("same");
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
