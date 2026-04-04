import ROOT
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import AutoMinorLocator

plt.rcParams.update({
    "font.family": "serif",
    "font.serif": ["STIXGeneral"],
    "font.size": 12,
    "xtick.direction": "in",
    "ytick.direction": "in",
    "xtick.top": True,
    "ytick.right": True,
    "mathtext.fontset": "stix"
})

f = ROOT.TFile("EventData.root", "READ")
h = f.Get("E_dep")

nbins = h.GetNbinsX()
x = np.array([h.GetBinCenter(i) for i in range(1, nbins + 1)])
y = np.array([h.GetBinContent(i) for i in range(1, nbins + 1)])

max_idx = np.argmax(y)
x_max = x[max_idx]
y_max = y[max_idx]

fig, ax = plt.subplots(figsize=(7, 5))
ax.step(x, y, where='mid', color='blue', linewidth=1.2, label='Energy Spectrum')


peak_label = f'Peak:\n$E$ = {x_max:.2f} keV\n$C$ = {int(y_max)}' 

if y_max > 10000:
    peak_label = f'Peak:\n$E$ = {x_max:.2f} keV\n$C$ = {y_max:.2e}'

ax.annotate(peak_label, 
            xy=(x_max, y_max), 
            xytext=(x_max * 1.15, y_max * 0.85),
            arrowprops=dict(arrowstyle='->', color='black', lw=1),
            fontsize=10,
            bbox=dict(boxstyle="round,pad=0.3", fc="white", ec="gray", lw=0.5, alpha=0.8)) # 加入半透明白底，防止背景干扰

ax.scatter(x_max, y_max, color='red', s=20, zorder=5)

ax.set_xlabel("Energy (keV)", fontsize=14)
ax.set_ylabel("Counts", fontsize=14)
ax.ticklabel_format(style='sci', axis='y', scilimits=(0,0), useMathText=True)
ax.set_ylim(bottom=0, top=y_max * 1.3)
ax.legend(frameon=False)
plt.tight_layout()
plt.show()