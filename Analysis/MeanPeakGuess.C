// Author Sean and Naz

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <TFile.h>
#include "TH1D.h"
#include "TF1.h"
#include "TCanvas.h"




void CreateHistogramsFromCSV(const char* inputFileName) {
    // Open the input CSV file
    std::ifstream inputFile(inputFileName);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input CSV file." << std::endl;
        return;
    }

// Create a TH1D histogram for col4, col5, and col6 using col3 as the x-axis
    TFile* outputFile = new TFile("output_guess.root", "RECREATE");
    TH1D* histG1 = new TH1D("GuessPhoton", "Wavelength mean Guess for Photons", 900, 0, 900);
    TH1D* histG2 = new TH1D("GuessAbsoluteIrr", "Wavelength mean Guess for Absolute Irr", 900, 0, 900);
    TH1D* histG3 = new TH1D("GuessScopeCount", "Wavelength mean Guess for Scope Count", 900, 0, 900);


    //** FOR NAZ ADD AXIS TITLES WITH UNITS**//
    //** FOR NAZ ADD SET POINTS LARGER**//
    //** FOR NAZ ADD CHANGE STATS BLOCK TO REMOVE SQURE IN TOP RIGHT CORNER**//

    
    // Read data from the CSV file line by line
    std::string line;
    int HistNum,HistType;
    double Wavelength,Amplitude,Sigma,Chi2Num,NDFNum,integral,integral2,integral3,integral4,integral5,PeakPositionGuess;
    char comma;
    int interval = 50;
    while(std::getline(inputFile, line)) {
        std::istringstream ss(line);

        // Read line
        ss >> HistNum >> comma >> HistType >> comma >> Wavelength>> comma >> Amplitude >> comma
	   >> Sigma >> comma >> Chi2Num >> comma >> NDFNum>> comma >>integral2 >> comma >> integral3
	   >> comma >> integral4 >> comma >> integral5 >> comma >> integral>> comma >> PeakPositionGuess;
	Double_t perresidual;
	perresidual = TMath::Abs((PeakPositionGuess-Wavelength)/(Wavelength));
	
	
	if(HistType == 1){
	  if(Wavelength!=0 && perresidual<0.05){
	    histG1->Fill(double(HistNum)*interval, Wavelength);
	    //histG1->SetBinError(double(HistNum)*50,1);
	    //std::cout<<"HisNum "<< HistNum<< " Wavelength "<< Wavelength<<std::endl;
	    
	  }
	} else if(HistType == 2){
	  if(Wavelength!=0 && perresidual<0.05){
	    histG2->Fill(double(HistNum)*interval, Wavelength);
	    // histG2->SetBinError(double(HistNum)*50,1);
	    //std::cout<<"HisNum "<< HistNum<< " Wavelength "<< Wavelength<<std::endl;
	  }
	} else if(HistType == 3) {
	  if(Wavelength!=0 && perresidual<0.04){
	    histG3->Fill(double(HistNum)*interval, Wavelength);
	    //histG3->SetBinError(double(HistNum)*50,1);
	    //std::cout<<"HisNum "<< HistNum<< " Wavelength "<< Wavelength<<std::endl;
	  }	  
	}
	
    }

    // Create a TF1 object for the linear function
    TF1 *linearFitFunction1 = new TF1("linearFitFunction1", "[0] + [1]*x", 0, 900);
    //Make parameter guesses
    linearFitFunction1->SetParameter(0,100.0);
    linearFitFunction1->SetParameter(1,0.58895);
    // Fit the line to the histogram
    histG1->Fit("linearFitFunction1","Q");
    // Access the fit parameters
    double intercept1 = linearFitFunction1->GetParameter(0);
    double slope1 = linearFitFunction1->GetParameter(1);
    // Print the fit parameters
    //std::cout << "Fit Parameters:" << std::endl;
    //std::cout << "Intercept1: " << intercept1 << std::endl;
    //std::cout << "Slope1: " << slope1 << std::endl;
    std::cout<<interval <<" "<<intercept1<< " "<<slope1<<std::endl;  
    // Optionally, you can create a canvas and draw the histogram
    TCanvas *canvas1 = new TCanvas("canvas1", "Canvas1", 800, 600);
    histG1->Draw();
    canvas1->SaveAs("fit_result1.png");
    canvas1->Close();
    
    // Create a TF1 object for the linear function
    TF1 *linearFitFunction2 = new TF1("linearFitFunction2", "[0] + [1]*x", 0, 900);
    //Make parameter guesses
    linearFitFunction2->SetParameter(0,100.0);
    linearFitFunction2->SetParameter(1,0.58895);
    // Fit the line to the histogram
    histG2->Fit("linearFitFunction2","Q");
    // Access the fit parameters
    double intercept2 = linearFitFunction2->GetParameter(0);
    double slope2 = linearFitFunction2->GetParameter(1);
    // Print the fit parameters
    //std::cout << "Fit Parameters:" << std::endl;
    //std::cout << "Intercept2: " << intercept2 << std::endl;
    //std::cout << "Slope2: " << slope2 << std::endl;
    // Optionally, you can create a canvas and draw the histogram
    TCanvas *canvas2 = new TCanvas("canvas2", "Canvas2", 800, 600);
    histG2->Draw();
    canvas2->SaveAs("fit_result2.png");
    canvas2->Close();

    // Create a TF1 object for the linear function
    TF1 *linearFitFunction3 = new TF1("linearFitFunction3", "[0] + [1]*x", 0, 900);
    //Make parameter guesses
    linearFitFunction3->SetParameter(0,100.0);
    linearFitFunction3->SetParameter(1,0.58895);
    // Fit the line to the histogram
    histG3->Fit("linearFitFunction3","Q");
    // Access the fit parameters
    double intercept3 = linearFitFunction3->GetParameter(0);
    double slope3 = linearFitFunction3->GetParameter(1);
    // Print the fit parameters
    //std::cout << "Fit Parameters:" << std::endl;
    //std::cout << "Intercept3: " << intercept3 << std::endl;
    //std::cout << "Slope3: " << slope3 << std::endl;
    // Optionally, you can create a canvas and draw the histogram
    TCanvas *canvas3 = new TCanvas("canvas3", "Canvas3", 800, 600);
    histG3->Draw();
    canvas3->SaveAs("fit_result3.png");
    canvas3->Close();

    
    inputFile.close();  
    outputFile->Write();
    outputFile->Close();
 
}
 

int MeanPeakGuess() {
    const char* inputFileName = "FitResults.txt";
    CreateHistogramsFromCSV(inputFileName);
    return 0;
}
