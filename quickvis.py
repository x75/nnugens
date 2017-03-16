import numpy as np
import matplotlib.pyplot as plt
from matplotlib import gridspec
from scipy.io import wavfile


r, d = wavfile.read("synth_ep3_1.wav")

numrows = 4
numcols = 4

numchunks = numrows * numcols

chunksize = d.shape[0] // numchunks

gs = gridspec.GridSpec(numrows, numcols)
fig = plt.figure()
fig.subplots_adjust(wspace=0.0, hspace = 0.0)
fig.show()
axs = []

plt.ion()

for i in range(numchunks):
    axs.append(fig.add_subplot(gs[i]))
    ax = axs[-1]
    ax.plot(d[i*chunksize:(i+1)*chunksize,0],
        d[i*chunksize:(i+1)*chunksize,1],
        "k,", alpha = 0.05)
    ax.set_xlim([-32768, 32768])
    ax.set_ylim((-32768, 32768))
    # ax.set_aspect(1)
    ax.set_xticks([])
    ax.set_yticks([])
    plt.draw()
    plt.pause(1e-9)

plt.ioff()
plt.show()
