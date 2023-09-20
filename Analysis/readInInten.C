#include <TH1D.h>
#include <TFile.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath> // Include the cmath library for mathematical operations

int readInInten() {
    const int maxFileNumber = 17;  // Change this to the number of files you have

    // Number of bins
    const double xMin = 175.56; // Minimum x value
    const double xMax = 1100.53; // Maximum x value
    double binSize = 0.57;
    double NBinsCalc = (xMax - xMin) / binSize;
    const int nBins = NBinsCalc; // Number of bins

    // Loop over file numbers
    std::cout << "Looping over files" << std::endl;
    for (int fileNumber = 0; fileNumber < maxFileNumber; ++fileNumber) {
        // Generate the file name
        std::string fileName = "Inten_360nmDG_3160umS_dial_";
	int  dialNumbs = 50+50*fileNumber;
	std::string fileNumbStr = std::to_string(fileNumber) ;
	fileName=fileName+std::to_string(dialNumbs)+"_"+fileNumbStr  +".txt";
        std::cout << "Opening file " << fileNumber << std::endl;
        // Open the file
        std::ifstream file(fileName);
        if (!file.is_open()) {
            std::cerr << "Could not open file: " << fileName << std::endl;
            continue;
        }

        // Skip the first line of the header
        std::string discard;
        std::getline(file, discard);

        // Variables to store header information
        double integrationTime;
        int averagingNumber;
        int smoothingNumber;
        std::string spectrometerName;

        // Parse the header information from lines 1 to 4
        for (int j = 1; j <= 4; ++j) {
            std::string headerLine;
            std::getline(file, headerLine);

            // Parse header line based on its index
            switch (j) {
            case 1: // Integration time
                integrationTime = std::stod(headerLine.substr(24));
                break;
            case 2: // Averaging number
                averagingNumber = std::stoi(headerLine.substr(23));
                break;
            case 3: // Smoothing number
                smoothingNumber = std::stoi(headerLine.substr(23));
                break;
            case 4: // Spectrometer name
                spectrometerName = headerLine.substr(39);
                break;
            default:
                break;
            }
        }

        // Skip the rest of the header
        for (int j = 0; j < 4; ++j) {
            std::string header;
            std::getline(file, header);
        }

        // Create a histogram
        TH1D* histogram1 = new TH1D(("abs_irrad_" + std::to_string(fileNumber)).c_str(),
                                    ("Absolute Irradiance " + std::to_string(fileNumber) + " - ").c_str(),
                                    nBins, xMin, xMax);
	
	TH1D* histogram2 = new TH1D(("PhotonCount_" + std::to_string(fileNumber)).c_str(),
                                    ("PhotonCount " + std::to_string(fileNumber) + " - ").c_str(),
                                    nBins, xMin, xMax);
	TH1D* histogram3 = new TH1D(("scopeCount_" + std::to_string(fileNumber)).c_str(),("ScoupeCount " + std::to_string(fileNumber) + " - ").c_str(),nBins, xMin, xMax);


	
        // Skip the units line
        std::string unitsLine;
        std::getline(file, unitsLine);

        // Read and store data
        double wavelength, sampleCount, darkCount, referenceCount, absoluteIrradiance, photonCount;
        char delimiter;
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            if (iss >> wavelength >> delimiter >> sampleCount >> delimiter >> darkCount >> delimiter >>
                referenceCount >> delimiter >> absoluteIrradiance >> delimiter >> photonCount) {
                
                double absoluteIrradianceError = 0.1; // Modify this value using error propogation
                // Fill the histogram and set the bin error
                int binX1 = histogram1->GetXaxis()->FindBin(wavelength);
                histogram1->Fill(wavelength, absoluteIrradiance);
                histogram1->SetBinError(binX1, absoluteIrradianceError);
		histogram1->GetXaxis()->SetTitle("Wavelength (nm)");
		histogram1->GetYaxis()->SetTitle("Absolute Irradiance (#mu Watt/cm^2/nm)");
		histogram1->GetYaxis()->SetRangeUser(-120,120);
		histogram1->SetStats(kFALSE);
		
		double photonCountError = 0.1; // Modify this value using error propogation
                // Fill the histogram and set the bin error
                int binX2 = histogram2->GetXaxis()->FindBin(wavelength);
                histogram2->Fill(wavelength, photonCount);
                histogram2->SetBinError(binX2, photonCountError);
		histogram2->GetXaxis()->SetTitle("Wavelength (nm)");
		histogram2->GetYaxis()->SetTitle("Photon Counts (#mu mol/s/m^2/nm)");
		histogram2->GetYaxis()->SetRangeUser(-10,10);
		histogram2->SetStats(kFALSE);
		
		double scopeCountError = 0.1; // Modify this value using error propogation
                // Fill the histogram and set the bin error
                int binX3 = histogram3->GetXaxis()->FindBin(wavelength);
                histogram3->Fill(wavelength, sampleCount-darkCount);
                histogram3->SetBinError(binX3, scopeCountError);
		histogram3->GetXaxis()->SetTitle("Wavelength (nm)");
		histogram3->GetYaxis()->SetTitle("Sample-Dark (counts)");
		histogram3->GetYaxis()->SetRangeUser(-200,600);
		histogram3->SetStats(kFALSE);
		
            }
        }

        // Close the file
        file.close();

        // Save the histogram and header values to a ROOT file
        std::cout << "File " << fileName << " processed" << std::endl;
        TFile output("output.root", "UPDATE");
        if (!output.IsZombie()) {
            histogram1->Write();
	    histogram2->Write();
	    histogram3->Write();
            output.Close();
            delete histogram1;
	    delete histogram2;
	    delete histogram3;
        } else {
            std::cerr << "Could not open output file." << std::endl;
        }
    }
    std::cout<<"Read in Complete"<<std::endl;
    return 0;
}
