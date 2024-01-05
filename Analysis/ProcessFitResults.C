// Author Sean and Naz

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <TFile.h>
#include <TH1D.h>



void CreateHistogramsFromCSV(const char* inputFileName) {
    // Open the input CSV file
    std::ifstream inputFile(inputFileName);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input CSV file." << std::endl;
        return;
    }

// Create a TH1D histogram for col4, col5, and col6 using col3 as the x-axis
    TFile* outputFile = new TFile("output_histograms.root", "RECREATE");

    TH2D* histE1 = new TH2D("EnvelopPhoton", "Envelope of Gassian Fit", 700, 200, 900, 600, 0, 300);
    TH2D* histS1 = new TH2D("SigmaPhoton", "Sigma of Gaussian Fits", 700,200,900,100,0,100);
    TH2D* histE2 = new TH2D("EnvelopAbsoluteIrr", "Envelope of Gassian Fit", 700, 200, 900, 600, 0, 300);
    TH2D* histS2 = new TH2D("SigmaAbsoluteIrr", "Sigma of Gaussian Fits", 700,200,900,100,0,100);
    TH2D* histE3 = new TH2D("EnvelopScopeCount", "Envelope of Gassian Fit", 700, 200, 900, 600, 0, 300);
    TH2D* histS3 = new TH2D("SigmaScope", "Sigma of Gaussian Fits", 700,200,900,100,0,100);

    //** FOR NAZ ADD AXIS TITLES WITH UNITS**//
    //** FOR NAZ ADD SET POINTS LARGER**//
    //** FOR NAZ ADD CHANGE STATS BLOCK TO REMOVE SQURE IN TOP RIGHT CORNER**//

    
    // Read data from the CSV file line by line
    std::string line;
    int HistNum,HistType;
    double Wavelength,Amplitude,Sigma,Chi2Num,NDFNum;
    char comma;

    while (std::getline(inputFile, line)) {
        std::istringstream ss(line);

        // Read line
        ss >> HistNum >> comma >> HistType >> comma >> Wavelength>> comma >> Amplitude >> comma >> Sigma >> comma >> Chi2Num >> comma >> NDFNum;


	if(HistType == 1){
        // Fill the scatter plot
	  histE1->Fill(Wavelength, Amplitude);
	  histS1->Fill(Wavelength, Sigma);
	} else if(HistType == 2){
	  histE2->Fill(Wavelength, Amplitude);
	  histS2->Fill(Wavelength, Sigma);
	} else if(HistType == 3) {
	  histE3->Fill(Wavelength, Amplitude);
	  histS3->Fill(Wavelength, Sigma);
	}
    }

    inputFile.close();

   // Write the scatter plot and histograms to the output ROOT file

    //** FOR NAZ DRAW AND PRINT HISTOGRAMS AS PNG HERE**// //or root output.root and run .ls and [histname]->Draw() and save there
    // WHEN SAVING SAVE WITH FILENAME THAT DECRIBES FULL TEST
  
    outputFile->Write();
    outputFile->Close();
}
 

int ProcessFitResults() {
    const char* inputFileName = "FitResults.txt";
    CreateHistogramsFromCSV(inputFileName);
    return 0;
}
