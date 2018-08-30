#####################################################
#             PURITY MONITOR ANALYSIS               #
#   Questions: l.cremonesi@ucl.ac.uk                #
#              l.manenti@ucl.ac.uk ???              #
#####################################################

This package has been written to perform the purity monitor analysis.

DATA TAKING:

Data taking is done using LeCroy oscilloscope and python script found in package (link to github)
   i) Data is saved in binaries and is rootified using python rootify.py
   ii) Using the script rootifyAll.sh one can rootify all channels saved in a specific directory
   iii) Rootified data is composed of TGraphs that can be read in and used in this package
   iv) Rootified data is centred in 0
We take data for different fields (usually giving channel 3 to the anode and channel 4 to the cathode)
In the "normal" configuration pre-amp A is for the Anode and pre-amp B is for the cathode
   i) values for pre-amps tau electronics can be found in spreadsheet (add link)

ANALYSIS:

(The bash script processAllFields.sh does all these steps for you)
1- To run macros in this package you will need ROOT and libFFTWwrapper
2- First step in the analysis is to run findAllAverages.C that calculates the following:
   i) a simple average
   ii) two filtered averages
   iii) a noise template and a "subtracted" average
   iv) a periodogram
   v) the output is a summary root file containing all averages ("*_averages.root")
3- The real analysis is done in getSubtraction.C (Note for Linda: change name...) where the following steps are applied:
   i) for your chosen average (default is the simple zeroed average) the cathode and anode signals are taken as input
   ii) SMOOTHING:
      - Using the ICARUS polynomial we find the expected drift times between cathode-cathode grid, cathode grid - anode grid, and anode grid-anode.
      - We use the above the above times to calculate the max number of time points to smooth our waveform (tdrift>10*deltat) where deltat is the sampling time
      - if fibreOut waveforms were taken, those are subtracted from both the cathode and anode
   iii) CATHODE:
      - Using the expected tdrift between cathode and grid cathode, we look for the minimum of the waveform, this corresponds to tGK (time that electrons reach the grid-cathode) and K (cathode amplitude)
      - tK (time that the electrons leave the cathode) is taken to be 0.
   iv) ANODE:
      - The anode signal is fitted with a PMT function (put functional form here)
      - The maximum of the anode corresponds to tA (time electrons reach the anode) and A (anode amplitude)
      - tGA (time electrons leave the grid anode) is found by stepping backwards in time from tA until the amplitude is 0.01*A
   v) CORRECTION FACTOR:
      - first A and K are renormalised considering the different gains of the two preamps;
      - iterative correction factor is calculated (see subsection bla)
   vi) LIFETIME CALCULATION:
      - Lifetime is calculated both using the approximation in eq~\ref{} and using the full equation and the ROOT::Math::BrentRootFinder
   v) OUTPUT:
      - Output graphs and canvases saved in *signals.root
      - Summary txt file with parameters used and final lifetimes saved in *lifetime.txt
4- Final plots:
   i) fitAttachmentCoefficient() makes plots


