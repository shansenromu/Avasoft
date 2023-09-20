#include <TFile.h>
#include <TKey.h>
#include <TCanvas.h>
#include <TH1D.h>

void SaveTH1DAsPNG(const char *key, TH1D *histogram) {
    // Create a canvas
    TCanvas *canvas = new TCanvas("canvas", "Canvas", 800, 600);

    // Draw the histogram
    histogram->Draw();

    // Save the canvas as a PNG image using the provided key
    TString outputFileName = TString(key) + ".png";
    canvas->SaveAs(outputFileName);

    // Clean up
    delete canvas;
}

int drawAll() {
    // Open the ROOT file
    TFile *file = TFile::Open("output.root", "READ");

    if (!file || file->IsZombie()) {
        std::cerr << "Error: Could not open the ROOT file." << std::endl;
        return 1;
    }

    // Loop through all keys in the file
    TIter next(file->GetListOfKeys());
    TKey *key;

    while ((key = dynamic_cast<TKey*>(next()))) {
        TObject *obj = key->ReadObj();

        // Check if the object is a TH1D
        if (obj && obj->InheritsFrom("TH1D")) {
            TH1D *histogram = dynamic_cast<TH1D*>(obj);

            // Save the TH1D as a PNG using its key as the file name
            SaveTH1DAsPNG(key->GetName(), histogram);
        }
    }

    // Close the ROOT file
    file->Close();

    return 0;
}
