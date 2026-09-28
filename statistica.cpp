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



TCanvas *c_scarti = nullptr;
TCanvas *c_energiax = nullptr;
TCanvas *c_energiay = nullptr;



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

    //c_energiax = new TCanvas("c_energiax", "c_energiax", 3000, 600);
    //c_energiax->Divide(5,1);

    TCanvas *c_x[5];

    TH1F *h_listx[5] = {E_depx1, E_depx2, E_depx3, E_depx4, E_depx5};
    TH1F *h_listy[5] = {E_depy1, E_depy2, E_depy3, E_depy4, E_depy5};

    //COORDINATA X
    for(int j=0; j<5; j++){

        
        TF1 *f_bkg = new TF1("f_bkg", "landau", 0, 60000);
        f_bkg->SetParameters(100000, 10000, 3000);
        f_bkg->SetLineColor(kBlue);

        TF1 *f_sgl = new TF1("f_sgl", "landau", 50000, 400000);
        f_sgl->SetParameters(500000, 120000, 15000);

        c_x[j] = new TCanvas(Form("c_x%d", j+1), "energiax", 800, 600);
        //c_energiax->cd(j+1);
        gPad->SetLogy(1);
        h_listx[j]->SetStats(0);
        h_listx[j]->SetFillColor(kViolet-9);
        h_listx[j]->SetLineColor(kViolet-9);
        h_listx[j]->Draw("hist");
        h_listx[j]->Fit(f_sgl, "RQ +");
        mu_sglx.push_back(f_sgl->GetParameter(1));
        sigma_sglx.push_back(f_sgl->GetParameter(2));
        f_sgl->Draw("same");

        auto legend = new TLegend(0.6,0.7,0.9,0.9);
        legend->AddEntry(h_listx[j],"Energia depositata","f");
        legend->AddEntry("f_sgl","Fit: Landau segnale","l");
        legend->AddEntry("f_bkg","Fit: Landau rumore","l");
        legend->Draw();

        if(j<3){
            h_listx[j]->Fit(f_bkg, "RQ");
            mu_bkgx.push_back(f_bkg->GetParameter(1));
            sigma_bkgx.push_back(f_bkg->GetParameter(2));
            f_bkg->Draw("same");
        }
        else{
            continue;
        }

    }

    c_energiay = new TCanvas("c_energiay", "c_energiay", 3000, 600);
    c_energiay->Divide(5,1);

    //COORDINATA Y
    for(int j=0; j<5; j++){
        TF1 *f_bkg = new TF1("f_bkg", "landau", 0, 60000);
        f_bkg->SetParameters(100000, 10000, 3000);
        f_bkg->SetLineColor(kBlue);

        TF1 *f_sgl = new TF1("f_sgl", "landau", 50000, 400000);
        f_sgl->SetParameters(500000, 120000, 15000);

        c_energiay->cd(j+1);
        gPad->SetLogy(1);
        h_listy[j]->SetStats(0);
        h_listy[j]->SetFillColor(kGreen-9);
        h_listy[j]->SetLineColor(kGreen-9);
        h_listy[j]->Draw("hist");
        h_listy[j]->Fit(f_sgl, "RQ +");
        mu_sgly.push_back(f_sgl->GetParameter(1));
        sigma_sgly.push_back(f_sgl->GetParameter(2));
        f_sgl->Draw("same");

        if(j<3){
            h_listy[j]->Fit(f_bkg, "RQ");
            mu_bkgy.push_back(f_bkg->GetParameter(1));
            sigma_bkgy.push_back(f_bkg->GetParameter(2));
            f_bkg->Draw("same");
        }
        else{
            continue;
        }
        

    }


    cout<<"\n\nMedie Landau per rumore (background) X: "<<endl;
    for(int i=0; i<3; i++){
        cout<<mu_bkgx[i]<<", ";
    }
    cout<<"\nSigma Landau per rumore (background) X: "<<endl;
    for(int i=0; i<3; i++){
        cout<<sigma_bkgx[i]<<", ";
    }

    cout<<"\n\nMedie Landau per rumore (background) Y: "<<endl;
    for(int i=0; i<3; i++){
        cout<<mu_bkgy[i]<<", ";
    }
    cout<<"\nSigma Landau per rumore (background) Y: "<<endl;
    for(int i=0; i<3; i++){
        cout<<sigma_bkgy[i]<<", ";
    }

    
    cout<<"\n\nMedie Landau per segnale (signal) X: "<<endl;
    for(int i=0; i<5; i++){
        cout<<mu_sglx[i]<<", ";
    }
    cout<<"\nSigma Landau per segnale (signal) X: "<<endl;
    for(int i=0; i<5; i++){
        cout<<sigma_sglx[i]<<", ";
    }

    cout<<"\n\nMedie Landau per segnale (signal) Y: "<<endl;
    for(int i=0; i<5; i++){
        cout<<mu_sgly[i]<<", ";
    }
    cout<<"\nSigma Landau per segnale (signal) Y: "<<endl;
    for(int i=0; i<5; i++){
        cout<<sigma_sgly[i]<<", ";
    }

/*
    double mu_bkg_totx = 0;
    double mu_bkg_toty = 0;
    double w_totx = 0;
    double w_toty = 0;
    for(int i=0; i<3; i++){
        mu_bkg_totx = mu_bkg_totx + mu_bkgx[i]/(sigma_bkgx[i]*sigma_bkgx[i]);
        mu_bkg_toty = mu_bkg_toty + mu_bkgy[i]/(sigma_bkgy[i]*sigma_bkgy[i]);
        w_totx = w_totx + 1/(sigma_bkgx[i]*sigma_bkgx[i]);
        w_toty = w_toty + 1/(sigma_bkgy[i]*sigma_bkgy[i]);
    }

    mu_bkg_totx = mu_bkg_totx/w_totx;
    mu_bkg_toty = mu_bkg_toty/w_toty;

    cout<<"\n\nValore d'aspettazione di hit di rumore per evento:"<<endl;
    cout<<"X: "<<mu_bkg_totx<<" +- "<<sqrt(1/w_totx)<<endl;
    cout<<"Y: "<<mu_bkg_toty<<" +- "<<sqrt(1/w_toty)<<endl;*/
    
}




void fit_x(){
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
                            double threshold = 100.0;
                            if(prob_bkg > prob_sgl && (prob_bkg/prob_sgl) > threshold){ //se è più probabile che faccia parte del background rimuoviamo l'hit
                                //cout<<"Hit singolo di rumore rimosso. Evento "<<i<<", energia depositata: "<<e_dep[kk]<<endl;
                                //cout<<"Prob sgl: "<<prob_sgl<<"\nProb bkg: "<<prob_bkg<<endl;
                                indx_rm.push_back(kk); 
                                if(k==3 || k==4){
                                    h_casesx->Fill(10);
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

    TCanvas *c_traccex = new TCanvas("c_traccex", "c_traccex", 800, 600);
    h_casesx->SetStats(0);
    h_casesx->SetFillColor(kAzure-3);
    h_casesx->Draw("hist");

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
    for(int j=0; j<bkg_hit.size(); j++){
        mean_bkg = mean_bkg + bkg_hit[j];
    }
    mean_bkg = mean_bkg/((double) bkg_hit.size());
    for(int j=0; j<bkg_hit.size(); j++){
        sigma_bkg = sigma_bkg + (mean_bkg - bkg_hit[j])*(mean_bkg - bkg_hit[j]);
    }
    sigma_bkg = sqrt(sigma_bkg/((double) bkg_hit.size()));
    cout<<"\nValore d'aspettazione hit di rumore per evento (X): "<<mean_bkg<<" +- "<<sigma_bkg/sqrt((double) bkg_hit.size())<<"\nDispersione (evidenza Poisson): "<<sigma_bkg<<endl;


        
}
    




void fit_y(){
    vector<double> z;
    vector<double> y;
    vector<double> e_dep;

    vector<int> bkg_hit;
      
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
        int bkg_hit_v=0;
        t->GetEntry(i);
        bool traccia_pulita = true;
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
                    bkg_hit_v++; 
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
                bkg_hit_v++; 
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

            bkg_hit.push_back(bkg_hit_v); //aggiungo al vector il conteggio di hit di rumore trovati per l'evento i
        }
    }

    TCanvas *c_traccey = new TCanvas("c_traccey", "c_traccey", 800, 600);
    h_casesy->SetStats(0);
    h_casesy->SetFillColor(kAzure-3);
    h_casesy->Draw("hist");

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


    double mean_bkg=0;
    double sigma_bkg=0;
    for(int j=0; j<bkg_hit.size(); j++){
        mean_bkg = mean_bkg + bkg_hit[j];
    }
    mean_bkg = mean_bkg/((double) bkg_hit.size());
    for(int j=0; j<bkg_hit.size(); j++){
        sigma_bkg = sigma_bkg + (mean_bkg - bkg_hit[j])*(mean_bkg - bkg_hit[j]);
    }
    sigma_bkg = sqrt(sigma_bkg/((double) bkg_hit.size()));
    cout<<"\nValore d'aspettazione hit di rumore per evento (Y): "<<mean_bkg<<" +- "<<sigma_bkg/sqrt((double) bkg_hit.size())<<"\nDispersione (evidenza Poisson): "<<sigma_bkg<<endl;

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
1000000 eventi totali recap (coordinata X)
5 hit puliti: 701140, 70.114%
1 single hit rimosso: 67871, 6.7871%
2+ single hit rimossi: 0, 0%
1 double hit: 358595, 35.8595%
2+ double hit: 149355, 14.9355%
1 triple hit: 33787, 3.3787%
2+ triple hit: 1720, 0.172%
double + triple hit: 50527, 5.0527%
fit non possibile per troppi pochi punti: 4819, 0.4819%


1000000 eventi totali recap (coordinata Y)
5 hit puliti: 700555, 70.0555%
1 single hit rimosso: 67681, 6.7681%
2+ single hit rimossi: 1, 0.0001%
1 double hit: 357886, 35.7886%
2+ double hit: 149378, 14.9378%
1 triple hit: 33993, 3.3993%
2+ triple hit: 1709, 0.1709%
double + triple hit: 50792, 5.0792%
fit non possibile per troppi pochi punti: 4851, 0.4851%


*/