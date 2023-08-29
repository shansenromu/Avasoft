#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TSpectrum.h>

int FitPeaks(){

  TFile *file = TFile::Open("output.root");
  TH1D *histogram = dynamic_cast<TH1D*>(file->Get("hist_1"));

  double sigma = 5.;
  double threshold = 0.5;
  
  TSpectrum *spectrum = new TSpectrum();
  Int_t nPeaks = spectrum->Search(histogram, sigma, "goff", threshold);
  std::cout<< nPeaks << std::endl;
  Double_t *peakPositions = spectrum->GetPositionX();

  TCanvas *canvas = new TCanvas("canvas", "Gaussian Fit", 800, 600);
  TF1 *gaussianFit = new TF1("gaussianFit", "gaus", 200, 1100);
  for (Int_t i = 0; i < nPeaks; ++i) {
    Double_t peakPosition = peakPositions[i];
    std::cout<<peakPosition<<std::endl;
    // Set initial parameters for the Gaussian fit
    Double_t initialAmplitude = histogram->GetBinContent(histogram->GetXaxis()->FindBin(peakPosition));
    Double_t initialMean = peakPosition;
    Double_t initialSigma = 1.0; // Adjust as needed
    
    gaussianFit->SetParameters(initialAmplitude, initialMean, initialSigma);
    histogram->Fit(gaussianFit, "RQ+", "", peakPosition - 5, peakPosition + 5);

    // Print Gaussian fit parameters
    Double_t amplitude = gaussianFit->GetParameter(0);
    Double_t mean = gaussianFit->GetParameter(1);
    Double_t sigma = gaussianFit->GetParameter(2);

    // Print the parameters
    std::cout << "Peak " << i+1 << " parameters:" << std::endl;
    std::cout << "Amplitude: " << amplitude << std::endl;
    std::cout << "Mean: " << mean << std::endl;
    std::cout << "Sigma: " << sigma << std::endl;
    
    // Draw histogram and Gaussian fit
    histogram->Draw();
    gaussianFit->SetLineColor(kRed);
    gaussianFit->Draw("same");
    canvas->SaveAs(Form("output_peak_%d.png", i));
  }
  
  
  
  return 0;
  
}
