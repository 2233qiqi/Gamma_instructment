import ROOT
import numpy as np
import matplotlib.pyplot as plt

f = ROOT.TFile("EventData.root")
h = f.Get("E_dep")

nbins = h.GetNbinsX()

x = []
y = []

for i in range(1, nbins + 1):
    x.append(h.GetBinCenter(i))
    y.append(h.GetBinContent(i))

x = np.array(x)
y = np.array(y)


plt.figure(figsize=(8,6))

plt.step(x, y, where='mid', linewidth=1.5)

plt.xlabel("Energy (keV)", fontsize=14)
plt.ylabel("Counts", fontsize=14)
plt.title("Energy Deposition Spectrum", fontsize=16)

plt.grid(alpha=0.3)
plt.tight_layout()

plt.show()