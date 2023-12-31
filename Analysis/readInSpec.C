#include <TH1D.h>
#include <TFile.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath> // Include the cmath library for mathematical operations

int readInSpec() {
    const int maxFileNumber = 70;  // Change this to the number of files you have

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
        std::string fileName = "OldGrating_dial_";
	int  dialNumbs = 100+10*fileNumber;
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
        TH1D* histogram = new TH1D(("hist_" + std::to_string(fileNumber)).c_str(),
                                    ("Histogram " + std::to_string(fileNumber) + " - ").c_str(),
                                    nBins, xMin, xMax);

        // Skip the units line
        std::string unitsLine;
        std::getline(file, unitsLine);

        // Read and store data
        double wavelength, sampleCount, darkCount, referenceCount, scopeCorrected;
        char delimiter;
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            if (iss >> wavelength >> delimiter >> sampleCount >> delimiter >> darkCount >> delimiter >>
                referenceCount >> delimiter >> scopeCorrected) {
                // Calculate the error for scopeCorrected (modify this based on your error propagation method)
                // For example, assuming a fixed error value:
                double scopeCorrectedError = 0.1; // Modify this value as needed

                // Fill the histogram and set the bin error
                int binX = histogram->GetXaxis()->FindBin(wavelength);
                histogram->Fill(wavelength, scopeCorrected);
                histogram->SetBinError(binX, scopeCorrectedError);
            }
        }

        // Close the file
        file.close();

        // Save the histogram and header values to a ROOT file
        std::cout << "File " << fileName << " processed" << std::endl;
        TFile output("output.root", "UPDATE");
        if (!output.IsZombie()) {
            histogram->Write();
            output.Close();
            delete histogram;
        } else {
            std::cerr << "Could not open output file." << std::endl;
        }
    }

    return 0;
}
