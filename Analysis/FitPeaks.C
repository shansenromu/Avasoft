#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TSpectrum.h>

// Function to perform Gaussian with background fit
void FitGaussianWithBackground(TH1D *histogram, Double_t peakPosition) {
    // Set initial parameters for the fit
    Double_t initialAmplitude = histogram->GetBinContent(histogram->GetXaxis()->FindBin(peakPosition));
    Double_t initialMean = peakPosition;
    Double_t initialSigma = 1.0; // Adjust as needed
    Double_t initialBackground = 0.0;
    
    // Create a fitting function that is a guassian with a sloped background
    TF1 *fitFunction = new TF1("fitFunction", "[0]*exp(-0.5*((x-[1])/[2])**2) + [3]*x + [4]", 
                               peakPosition - 10, peakPosition + 10);
    fitFunction->SetParameters(initialAmplitude, initialMean, initialSigma, initialBackground);

    //gStyle->SetOptStat(1111);
    gStyle->SetOptFit(1111);
    histogram->Fit(fitFunction, "RQ+", "", peakPosition - 50, peakPosition + 50);

    
    
    // Print fit parameters
    Double_t amplitude = fitFunction->GetParameter(0);
    Double_t amplitude_err = fitFunction->GetParError(0);
    Double_t mean = fitFunction->GetParameter(1);
    Double_t mean_err = fitFunction->GetParError(1);
    Double_t sigma = fitFunction->GetParameter(2);
    Double_t sigma_err = fitFunction->GetParError(2);
    Double_t background_slope = fitFunction->GetParameter(3);
    Double_t background_slope_err = fitFunction->GetParError(3);
    Double_t background_offset = fitFunction->GetParameter(4);
    Double_t background_offset_err = fitFunction->GetParError(4);
    Double_t Chi2num = fitFunction->GetChisquare();
    Double_t NumberDegreesOfFreedom = fitFunction->GetNDF();
    Double_t pi = TMath::Pi();
    Double_t integral = amplitude * sigma * sqrt(2 * pi); // Using the properties of the Gaussian

    //approximately 95.45% within two standard deviations, and approximately 99.73% within three standard deviations
    Double_t xStart95, xEnd95, xStart99, xEnd99;
    xStart95 = mean-sigma*2.;
    xEnd95 = mean+sigma*2.;
    xStart99 = mean-sigma*3.;
    xEnd99 = mean+sigma*3.;
    // Find the corresponding bin numbers for the start and end x values
    Int_t binStart95 = histogram->GetXaxis()->FindBin(xStart95);
    Int_t binEnd95 = histogram->GetXaxis()->FindBin(xEnd95);
    Int_t binStart99 = histogram->GetXaxis()->FindBin(xStart99);
    Int_t binEnd99 = histogram->GetXaxis()->FindBin(xEnd99);
    // Calculate the integral over the specified bin range
    Double_t integral95 = histogram->Integral(binStart95, binEnd95);
    Double_t integral99 = histogram->Integral(binStart99, binEnd99);
    
    std::cout << peakPosition<<", ";
    std::cout <<  fitFunction->GetParameter(0) << ", " << fitFunction->GetParError(0);
    std::cout << ", " << fitFunction->GetParameter(1) << ", " << fitFunction->GetParError(1);
    std::cout << ", " << fitFunction->GetParameter(2) << ", " << fitFunction->GetParError(2);
    std::cout << ", " << fitFunction->GetParameter(3) << ", " << fitFunction->GetParError(3);
    std::cout << ", " << fitFunction->GetParameter(4) << ", " << fitFunction->GetParError(4);
    std::cout << ", " << fitFunction->GetChisquare() << ", " << fitFunction->GetNDF();
    std::cout << ", "<< integral<< ", " << integral95 << ", "<< integral99 << std::endl;;
    
    // Clean up
    delete fitFunction;
}

int FitPeaks() {
    TFile *file = TFile::Open("output.root");
    
    for (int j = 0; j < 70 ; j++) {
        std::string histNum = std::to_string(j);
        std::string histName = "hist_" + histNum;
        TH1D *histogram = dynamic_cast<TH1D*>(file->Get(histName.c_str()));
	std::cout<<" "<<std::endl;
        
        if (!histogram) {
            std::cerr << "Histogram " << histName << " not found!" << std::endl;
            continue;
        }
        
        double sigma = 5.;
        double threshold = 0.9;
        
        TSpectrum *spectrum = new TSpectrum();
        Int_t nPeaks = spectrum->Search(histogram, sigma, "goff", threshold);
        
        TCanvas *canvas = new TCanvas(Form("canvas_%d", j), "Gaussian Fit", 800, 600);
	std::ofstream outputFIle("FitPeakResultsNewGrating.cvs");
	
	for (Int_t i = 0; i < nPeaks; ++i) {
	  Double_t peakPosition = spectrum->GetPositionX()[i];
	  
	  // Perform Gaussian with background fit
	  FitGaussianWithBackground(histogram, peakPosition);
	  
	  // Draw histogram and fitted function
	  histogram->Draw();
	  canvas->SaveAs(Form("./pics/output_peak%d_file%d.png", i, j));
	
	
	
	  
	}
    	
	delete canvas;
    }
    file->Close();
    
    return 0;
    
}
