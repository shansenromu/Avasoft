#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TSpectrum.h>



// Function to fit a Gaussian to a specified range of a histogram
void FitGaussianInRange(TH1D* histogram, double xMin, double xMax, int j, int Htype, double peakPosition) {
  // Create a Gaussian fit function
  TF1* fitFunction = new TF1("gaussian_fit", "gaus", xMin, xMax);
  //fitFunction->SetParameters(100.0, (xMin+xMax)/2.0, 5.0);
  // Fit the function to the histogram within the specified range
  histogram->Fit(fitFunction, "RQ"); // "R" stands for "range" "Q" stands for Quiet
  double amplitude = fitFunction->GetParameter(0); // 
  double mean = fitFunction->GetParameter(1); // Mean of the Gaussian
  double sigma = fitFunction->GetParameter(2); // Standard deviation of the Gaussian
  double Chi2num = fitFunction->GetChisquare();
  if (Chi2num == NULL){
    Chi2num = -1;
  }
  double ndf_num= fitFunction->GetNDF();

  xStart2 = mean-sigma*2.;
  xEnd2 = mean+sigma*2.;
  xStart3 = mean-sigma*3.;
  xEnd3 = mean+sigma*3.;
  xStart4 = mean-sigma*4.;
  xEnd4 = mean+sigma*4.;
  xStart5 = mean-sigma*5.;
  xEnd5 = mean+sigma*5.;
  // Find the corresponding bin numbers for the start and end x values
  Int_t binStart2 = histogram->GetXaxis()->FindBin(xStart2);
  Int_t binEnd2 = histogram->GetXaxis()->FindBin(xEnd2);
  Int_t binStart3 = histogram->GetXaxis()->FindBin(xStart3);
  Int_t binEnd3 = histogram->GetXaxis()->FindBin(xEnd3);
  Int_t binStart4 = histogram->GetXaxis()->FindBin(xStart4);
  Int_t binEnd4 = histogram->GetXaxis()->FindBin(xEnd4);
  Int_t binStart5 = histogram->GetXaxis()->FindBin(xStart5);
  Int_t binEnd5 = histogram->GetXaxis()->FindBin(xEnd5);
  // Calculate the integral over the specified bin range
  Double_t integral2 = histogram->Integral(binStart2, binEnd2);
  Double_t integral3 = histogram->Integral(binStart3, binEnd3);
  Double_t integral4 = histogram->Integral(binStart4, binEnd4);
  Double_t integral5 = histogram->Integral(binStart5, binEnd5);
  
  // You can print or use the fit parameters as needed
  std::cout<< j<<", "<<Htype <<", "<< mean <<", "<<amplitude <<", " << sigma <<","<< Chi2num <<", "<< ndf_num <<
    integral2<< integral3<<integral4<<integral5<<peakPosition<<std::endl;
  delete fitFunction;
}


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

int FitIntensitySpec() {
    TFile *file = TFile::Open("output.root");

    int th1dCount = 0;

    TList *keys = file->GetListOfKeys();
    TIter keyIter(keys);

    TKey *key;
    while ((key = dynamic_cast<TKey*>(keyIter.Next()))) {
        TObject *obj = key->ReadObj();

        if (obj && obj->InheritsFrom("TH1D")) {
            th1dCount++;
        }
    }

    Int_t numberOfHist = th1dCount/3;
    for (int j = 0; j < numberOfHist;j++){
      std::string histNum = std::to_string(j);
      std::string histName1 = "PhotonCount_" + histNum;
      std::string histName2 = "abs_irrad_" + histNum;
      std::string histName3 = "scopeCount_" + histNum;
      TH1D *histogram1 = dynamic_cast<TH1D*>(file->Get(histName1.c_str()));
      TH1D *histogram2 = dynamic_cast<TH1D*>(file->Get(histName2.c_str()));
      TH1D *histogram3 = dynamic_cast<TH1D*>(file->Get(histName3.c_str()));
      
      if (!histogram1) {
	std::cerr << "Histogram " << histName1 << " not found!" << std::endl;
	continue;
      }
      if (!histogram2) {
	std::cerr << "Histogram " << histName2 << " not found!" << std::endl;
	continue;
      }
      if (!histogram3) {
	std::cerr << "Histogram " << histName3 << " not found!" << std::endl;
	continue;
      }
      
      histogram1->SetStats(kTRUE);
      histogram2->SetStats(kTRUE);
      histogram3->SetStats(kTRUE);

      
      Double_t PeakPosition,MinWavelength,MaxWavelength;

      Double_t  interval=10;

      
      //Double_t mSlope = 1.008571429; // For 300 nm Diffraction Grating
      Double_t mSlope = 0.58895 ;// For 275 nm Diffraction Grating
      Double_t mSlope_new = 0.527 ;// For 275 nm Diffraction Grating
      
      Double_t startWavelength = 195.11;

   
      PeakPosition = startWavelength+(j*interval)*mSlope_new;
      
      MinWavelength = PeakPosition - 30;
      MaxWavelength = PeakPosition +30;


      
      TCanvas *canvas1 = new TCanvas(Form("canvas1_%d", j), "Gaussian Fit", 800, 600);
      FitGaussianInRange(histogram1, MinWavelength,MaxWavelength,j,1,PeakPosition);
      //histogram1->GetYaxis()->SetRangeUser(-10, 300);
      histogram1->Draw();
      canvas1->SaveAs(Form("./PhotonCounts_hist%02d.png",j));
      delete canvas1;

      TCanvas *canvas2 = new TCanvas(Form("canvas2_%d", j), "Gaussian Fit", 800, 600);
      FitGaussianInRange(histogram2, MinWavelength,MaxWavelength,j,2,PeakPosition);
      //histogram2->GetYaxis()->SetRangeUser(-10, 1000);
      histogram2->Draw();
      canvas2->SaveAs(Form("./AbsoluteIrradiance_hist%02d.png",j));
      delete canvas2;
      
      TCanvas *canvas3 = new TCanvas(Form("canvas3_%d", j), "Gaussian Fit", 800, 600);
      FitGaussianInRange(histogram3, MinWavelength,MaxWavelength,j,3,PeakPosition);
      //histogram3->GetYaxis()->SetRangeUser(-10, 2000);
      histogram3->Draw();
      canvas3->SaveAs(Form("./ScopeCount_hist%02d.png",j));
      delete canvas3;
      
    }
    
    file->Close();
    
    return 0;
    
}
