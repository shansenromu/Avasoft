
void relDiffDiv() {
    // Read data from files and populate the vectors
    std::ifstream dataFile1("abs_irr_1.txt");
    std::ifstream dataFile2("abs_irr_2.txt");

    std::vector<double> wavelengths1, wavelengths2;
    std::vector<double> absoluteIrradiance1, absoluteIrradiance2;

    std::string line;
    // Skip the header lines
    for (int i = 0; i < 7; ++i) {
        std::string line;
        std::getline(dataFile1, line);
	std::getline(dataFile2, line);
    }

    
    while (std::getline(dataFile1, line)) {

      std::istringstream iss(line);
      double wavelength, sampleCount, darkCount, referenceCount, irradiance, photonCounts;
      char delimiter;
      if (iss >> wavelength >> delimiter >> sampleCount >> delimiter >> darkCount >> delimiter
	  >> referenceCount >> delimiter >> irradiance >> delimiter >> photonCounts) {
	wavelengths1.push_back(wavelength);
	absoluteIrradiance1.push_back(irradiance);
	std::cout << "wavelength: " << wavelength << ", irradiance: " << irradiance << std::endl;
	
      
        }
    }

    while (std::getline(dataFile2, line)) {

       std::istringstream iss(line);
      double wavelength, sampleCount, darkCount, referenceCount, irradiance, photonCounts;
      char delimiter;
      if (iss >> wavelength >> delimiter >> sampleCount >> delimiter >> darkCount >> delimiter
	  >> referenceCount >> delimiter >> irradiance >> delimiter >> photonCounts) {
	wavelengths2.push_back(wavelength);
	absoluteIrradiance2.push_back(irradiance);
	std::cout << "wavelength: " << wavelength << ", irradiance: " << irradiance << std::endl;
	
      
        
        }
    }

    // Create histograms
    double startWavelength = 175.56;
    double endWavelength = 1100.53;
    double binSize = 0.57;

    int numBins = (endWavelength - startWavelength) / binSize + 1;

    TH1D *hist1 = new TH1D("hist1", "Absolute Irradiance 1", numBins, startWavelength, endWavelength);
    TH1D *hist2 = new TH1D("hist2", "Absolute Irradiance 2", numBins, startWavelength, endWavelength);
    TH1D *dividedHist = new TH1D("dividedHist", "Relative Difference", numBins, startWavelength, endWavelength);

    // Fill histograms and perform division with error propagation
    for (size_t i = 0; i < wavelengths1.size(); ++i) {
        int bin = hist1->FindBin(wavelengths1[i]);
        hist1->SetBinContent(bin, absoluteIrradiance1[i]);
    }

    for (size_t i = 0; i < wavelengths2.size(); ++i) {
        int bin = hist2->FindBin(wavelengths2[i]);
        hist2->SetBinContent(bin, absoluteIrradiance2[i]);
    }

    for (int bin = 1; bin <= dividedHist->GetNbinsX(); ++bin) {
        double content1 = hist1->GetBinContent(bin);
        double error1 = std::sqrt(content1); // Assuming error is the square root of the content

        double content2 = hist2->GetBinContent(bin);
        double error2 = std::sqrt(content2); // Assuming error is the square root of the content

        if (content2 == 0) {
	  dividedHist->SetBinContent(bin, 0);
	  dividedHist->SetBinError(bin, 0);
        }else{

        double division = content1 / content2;
        double divisionError = division * std::sqrt((error1 / content1) * (error1 / content1) + (error2 / content2) * (error2 / content2));
        dividedHist->SetBinContent(bin, division);
        dividedHist->SetBinError(bin, 0);
	}
    }

    // Draw histograms and save to PNG file
    TCanvas *canvas = new TCanvas("canvas", "Histograms", 800, 600);
  
 

    
   
    //hist1->Draw();
    hist1->SetLineColor(kRed);
    hist1->SetLineWidth(2);

   
    //hist2->Draw("SAME");
    hist2->SetLineColor(kBlue);
    hist2->SetLineWidth(2);

    
    
    TLegend *legend1 = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend1->AddEntry(hist1, "Hist 1", "l");
    legend1->AddEntry(hist2, "Hist 2", "l");



    legend1->Draw();
    
    
    dividedHist->Draw();
    dividedHist->SetLineColor(kGreen);
    dividedHist->SetLineWidth(2);

   
    legend1->AddEntry(dividedHist, "Hist 1 / Hist 2", "l");
    legend1->Draw();
    canvas->Print("histograms2.png");
    //canvas->Print("histograms2.png");

   
}

