#include <TH1D.h>
#include <TFile.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

int readInSpec() {
  const int maxFileNumber = 5;  // Change this to the number of files you have

  
  // Number of bins
  const double xMin = 175.56;     // Minimum x value
  const double xMax = 1100.53;     // Maximum x value
  double binSize = 0.57;
  double NBinsCalc = (xMax-xMin)/binSize;
  const int nBins = NBinsCalc;   // Number of bins
  
  // Loop over file numbers
  std::cout<<"Looping over files"<<std::endl;
  for (int fileNumber = 1; fileNumber <= maxFileNumber; ++fileNumber) {
    // Generate the file name
    std::string fileName = std::to_string(fileNumber) + "Channel.txt";


    std::cout<<"Opening file " << fileNumber<<std::endl;
    // Open the file
    std::ifstream file(fileName);
    if (!file.is_open()) {
      std::cerr << "Could not open file: " << fileName << std::endl;
      continue;
    }

    // Skip the first line of the header
    std::string discard;
    std::getline(file, discard);
    //std::cout<<discard<<std::endl;
    double integrationTime;
    int averagingNumber;
    int smoothingNumber;
    std::string spectrometerName;
   
    // Parse the header information from lines 1 to 4
    for (int j = 1; j <= 4; ++j) {
        std::string headerLine;
        std::getline(file, headerLine);
	//std::cout<< headerLine<<std::endl;
        // Parse header line based on its index
        switch (j) {
            case 1: // Integration time
                integrationTime = std::stod(headerLine.substr(24)); // Adjust the substring position based on your header structure
                break;
            case 2: // Averaging number
                averagingNumber = std::stoi(headerLine.substr(23)); // Adjust the substring position based on your header structure
                break;
            case 3: // Smoothing number
                smoothingNumber = std::stoi(headerLine.substr(23)); // Adjust the substring position based on your header structure
                break;
            case 4: // Spectrometer name
                spectrometerName = headerLine.substr(39); // Adjust the substring position based on your header structure
                break;
            default:
                break;
        }
    }

    // Skip the rest of the header
    for (int j = 0; j < 4; ++j) {
        std::string header;
        std::getline(file, header);
	//std::cout<< header<<std::endl;
    }
    
    
    //std::cout<< integrationTime<< " " <<averagingNumber <<" "<< smoothingNumber << std::endl; 
    
      
      
    
    // Create a histogram
    TH1D* histogram = new TH1D(("hist_" + std::to_string(fileNumber)).c_str(), ("Histogram " + std::to_string(fileNumber) + " - ").c_str(), nBins, xMin, xMax);
    
    // Skip the units line
    std::string unitsLine;
    std::getline(file, unitsLine);
    
    // Read and store data
    double wavelength, sampleCount, darkCount, referenceCount, scopeCorrected;
    char delimiter;
    std::string line;
    while (std::getline(file, line)) {
      std::istringstream iss(line);
      if (iss >> wavelength >> delimiter >> sampleCount >> delimiter >> darkCount >> delimiter >> referenceCount >> delimiter >> scopeCorrected) {
	histogram->Fill(wavelength, scopeCorrected);
      }

      
    }
    
    // Close the file
    file.close();
    
    // Save the histogram and header values to a ROOT file
    std::cout<<"File "<< fileName <<" processed" <<std::endl;
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




