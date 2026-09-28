#include "TF1.h" 
#include "TH1F.h" 
#include <TFile.h>
#include <TCanvas.h>
#include <iostream>
#include <TGraph.h>
#include <TSystem.h>
#include <cmath>
#include <TFitResultPtr.h>
#include <TFitResult.h>
#include <vector>
#include <TLinearFitter.h>
#include <TMath.h>
#include <algorithm> 
#include "hits_header.h"

using namespace std;

//double mu_sgl[5] = {86295.1, 85950.9, 86016.2, 86356.1, 86361.7};
//double sigma_sgl[5] = {11371.6, 11532.5, 11507.1, 11318.5, 11352.4};

vector<int> single_double_hit(double *z_double, vector<double>& z, vector<double>& x, vector<double>& e_dep, int indx, vector<double>& mu_sgl, vector<double>& sigma_sgl){

    TGraph *g1 = new TGraph();
    g1->SetTitle("Grafico con Hit #1; z(mm); x(mm)");
    g1->SetMarkerStyle(20);
    g1->SetMarkerColor(kBlue+2);
    TGraph *g2 = new TGraph();
    g2->SetTitle("Grafico con Hit #2; z(mm); x(mm)");
    g2->SetMarkerStyle(20);
    g2->SetMarkerColor(kRed);

    /*cout<<"z doppio hit: "<<z_double[0]<<endl;
    cout<<"coordinate hit: "<<endl;
    for(int j=0; j<z.size();j++){
        cout<<"("<<z[j]<<", "<<x[j]<<")"<<endl;
    }*/
    //questa funzione permette di decidere quale hit tenere su un layer con un doppio hit
    //in base al chi2 e il p-value
    TLinearFitter lf(1, "pol1"); 
    int pt=0;
    int pos1=-1, pos2=-1;
    for (int i = 0; i < z.size(); ++i) {
        if(z[i] == z_double[0] && pos1==-1){ //z_double è una variabile che contiene la z del layer con due hit
            pos1=i;
        }
        else if(z[i] == z_double[0] && pos1!=-1){
            pos2=i;
            continue;
        }
        g1->SetPoint(pt, z[i], x[i]);
        lf.AddPoint(&z[i], x[i]); //sto aggiungendo il primo hit che trovo al fit, lasciando fuori il secondo
        pt++;
    }
    double prob_sgl[2];
    prob_sgl[0] = TMath::Landau(e_dep[pos1], mu_sgl[indx], sigma_sgl[indx]);
    prob_sgl[1] = TMath::Landau(e_dep[pos2], mu_sgl[indx], sigma_sgl[indx]);

    //primo fit: quello che tiene pos1
    lf.Eval(); // Perform the fast analytical fit
    double chi2_1 = lf.GetChisquare();
    double pvalue_1 = TMath::Prob(chi2_1, lf.GetNumberFreeParameters());
    double p0_1 = lf.GetParameter(0);
    double p1_1 = lf.GetParameter(1);

    lf.ClearPoints();
    pt=0;
    for (int i = 0; i < z.size(); ++i) {
        if(i==pos1) continue;
        lf.AddPoint(&z[i], x[i]);
        g2->SetPoint(pt, z[i], x[i]);
        pt++;

    }
    //secondo fit: quello che tiene pos2
    lf.Eval(); 
    double chi2_2 = lf.GetChisquare();
    double pvalue_2 = TMath::Prob(chi2_2, lf.GetNumberFreeParameters());
    double p0_2 = lf.GetParameter(0);
    double p1_2 = lf.GetParameter(1);


    /*TF1 *f1 = new TF1("f1", "pol1", 0., 1000.);
    f1->SetParameters(p0_1, p1_1);
    f1->SetLineColor(kGreen);
    TF1 *f2 = new TF1("f2", "pol1", 0., 1000.);
    f2->SetParameters(p0_2, p1_2);
    f2->SetLineColor(kGreen);

    TCanvas *c = new TCanvas("c", "c", 1000, 600);
    c->Divide(2,1);
    c->cd(1);
    g1->GetXaxis()->SetRangeUser(-100.0, 1100.0);
    g1->GetYaxis()->SetRangeUser(-20.0, 20.0);
    g1->Draw("AP");
    f1->Draw("same");
    c->cd(2);
    g2->GetXaxis()->SetRangeUser(-100.0, 1100.0);
    g2->GetYaxis()->SetRangeUser(-20.0, 20.0);
    g2->Draw("AP");
    f2->Draw("same");

    cout<<"p-value 1: "<<pvalue_1<<"\np-value 2: "<<pvalue_2<<endl;
    cout<<"Prob sgl 1: "<<prob_sgl[0]<<", energia: "<<e_dep[pos1]<<" eV"<<endl;
    cout<<"Prob sgl 2: "<<prob_sgl[1]<<", energia: "<<e_dep[pos2]<<" eV"<<endl;*/

    vector<int> pos;
    if(pvalue_1 > pvalue_2 && prob_sgl[0] > prob_sgl[1]){
        pos.push_back(pos2);
        return pos; //restituisco la posizione del punto da eliminare 
    }
    else if(pvalue_2 > pvalue_1 && prob_sgl[1] > prob_sgl[0]){
        pos.push_back(pos1);
        return pos;
    }
    else{ //il metodo del chi2 e la prob dell'energia NON SONO IN ACCOORDO
        // tolgo entrambi i punti e vedo se ne rimangono abbastanza per fare il fit
        pos.push_back(pos1);
        pos.push_back(pos2);        
        return pos;
    }

}




vector<int> gen_double_hit(int z_counts[], double z_coords[], vector<double>& z, vector<double>& x, vector<double>& e_dep, vector<double>& mu_sgl, vector<double>& sigma_sgl){
    TGraph *g1 = new TGraph();
    g1->SetTitle("Traccia con tutti gli HIT; z(mm); x(mm)");
    g1->SetMarkerStyle(20);
    g1->SetMarkerColor(kBlue+2);
    TGraph *g2 = new TGraph();
    g2->SetTitle("Traccia pulita: rimossi gli HIT di rumore; z(mm); x(mm)");
    g2->SetMarkerStyle(20);
    g2->SetMarkerColor(kRed);


    vector<int> pos;
    double z_hit;
    for(int i=0; i<5; i++){ //scorre sui 5 layer
        int hits = z_counts[i];
        if(hits == 2 || hits == 3){
            z_hit=z_coords[i];
        }
        else continue;

        double prob_sgl[hits];
        int indx[hits];
        int k=0;
        for(int j=0; j<(int) z.size(); j++){
            if(z[j]==z_hit){
                prob_sgl[k] = TMath::Landau(e_dep[j], mu_sgl[i], sigma_sgl[i]);
                indx[k]=j; //indice dei vettori z,x,e_dep del doppio/triplo hit relativo a z=z_hit
                //cout<<Form("x(%.1f): ", z_hit)<<x[indx[k]]<<", energia: "<<e_dep[indx[k]]<<" eV"<<endl;
                k++;
            }
        }

        /*cout<<"Densita' di probabilita': "<<endl;
        cout<<z[indx[0]]<<": "<<prob_sgl[0]*100<<"%"<<", "<<e_dep[indx[0]]<<endl;
        cout<<z[indx[1]]<<": "<<prob_sgl[1]*100<<"%"<<", "<<e_dep[indx[1]]<<endl;*/
        double max=0;
        int indx_max=0;
        for(int i=0; i< hits; i++){
            if(prob_sgl[i] > max){
                max=prob_sgl[i];
                indx_max=indx[i];
            }
        }
        for(int i=0; i<hits; i++){
            if(prob_sgl[i] == max) continue;
            else pos.push_back(indx[i]);
        }
        
    }
    //alla fine ottengo un vector di indici degli hit da scartare in base alla probabilità della landau segnale
    /*cout<<"\n\nVector con posizioni degli hit da scartare:"<<endl;
    for(int i=0; i<pos.size(); i++){
        cout<<pos[i]<<": ("<<z[pos[i]]<<", "<<x[pos[i]]<<")"<<endl;
    }*/

    int point=0;
    for(int k=0; k<(int) z.size(); k++){
        g1->SetPoint(k, z[k], x[k]); //grafico con tutti gli hit
        auto it = find(pos.begin(), pos.end(), k);
        if(it==pos.end()){// se l'indice k non è presente nel vector pos allora aggiungo il punto k al grafico g2
            g2->SetPoint(point, z[k], x[k]);
            point++;
        }
    }
    /*TF1 *f = new TF1("f", "pol1", 0., 1000.);
    f->SetLineColor(kGreen);
    f->SetParameter(0, 0);
    f->SetParameter(1, 0);
    TFitResultPtr fitResult = g2->Fit(f, "SR");
    //cout<<"Chi2: "<<fitResult->Chi2()<<endl;
    cout<<"Prob: "<<TMath::Prob(fitResult->Chi2(), (z.size()-2))<<endl;

    TCanvas *c = new TCanvas("c", "c", 1000, 600);
    c->Divide(2,1);
    c->cd(1);
    g1->GetXaxis()->SetRangeUser(-100.0, 1100.0);
    g1->GetYaxis()->SetRangeUser(-20.0, 20.0);
    g1->Draw("AP");
    c->cd(2);
    g2->GetXaxis()->SetRangeUser(-100.0, 1100.0);
    g2->GetYaxis()->SetRangeUser(-20.0, 20.0);
    g2->Draw("AP");
    f->Draw("same");*/

    return pos;
}