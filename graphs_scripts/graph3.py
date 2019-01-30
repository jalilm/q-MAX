#!/usr/bin/python
import pandas as pd
import matplotlib.pyplot as plt
from cycler import cycler
from math import log, ceil
import numpy as np

#multichrome = cycler(color=list('bgrcmyk')) + cycler(marker=['o','^','v','d','|','*','8']) + cycler(linestyle=['-', '--', '-.', ':', '-' ,'--', '-.'])
multichrome = cycler(color=list('rcmyk')) + cycler(marker=['v','d','8','*','X']) + cycler(linestyle=['-.', ':', '-' ,'--', '-.'])

fig, ax1 = plt.subplots(1,1)
ax1.set_prop_cycle(multichrome)
ax1.grid()

ax1.set_ylabel('Throughput [MPPS]', fontsize=20)
ax1.set_xlabel(r'Performance parameter ($\gamma$)', fontsize=20)

df_org = pd.read_csv('../results/timing_random.raw_res', skipinitialspace=True)
df_org['throughput'] = df_org['insertions'] / 1000000 / df_org['duration']
del df_org['insertions']
del df_org['duration']
del df_org['dataset']

s=1000000

heap_df_org = df_org[(df_org['type'] == 'Heap')]
heap_df_org = heap_df_org[(heap_df_org['size'] == s)]
del heap_df_org['size']
gh = heap_df_org.mean()
ax1.axhline(y=gh['throughput'], label=r'Heap, $q=10^{' + str(int(ceil(log(s,10)))) + r'}$', markersize=10, color='b', marker='o', linestyle='-')

skiplist_df_org = df_org[(df_org['type'] == 'SkipList')]
skiplist_df_org = skiplist_df_org[(skiplist_df_org['size'] == s)]
del skiplist_df_org['size']
gs = skiplist_df_org.mean()
ax1.axhline(y=gs['throughput'], label=r'SkipList, $q=10^{' + str(int(ceil(log(s,10)))) + r'}$', markersize=10, color='g', marker='^', linestyle='--')

df_org = df_org[df_org['gamma'] <= 2]
df_org = df_org[df_org['gamma'] >= 0.0001]

for s in [10000, 100000, 1000000, 10000000]:
    size_df = df_org[(df_org['size'] == s)]
    del size_df['size']
    qmax_size_df = size_df[(size_df['type'] == 'AmortizedQMax')]
    g = qmax_size_df.groupby(['gamma'])
    ax1.plot(g.mean(), label=r'q-MAX, $q=10^{' + str(int(ceil(log(s,10)))) + r'}$', markersize=10)

handles, labels = ax1.get_legend_handles_labels()
ax1.legend(prop={'size': 16}, loc='upper left', bbox_to_anchor=(0,1), ncol=1)
fig.tight_layout()

ax1.set_xticks(np.arange(9))
plt.tick_params(axis='both', which='major', labelsize=16)
plt.ylim(ymin=0)
ax1.set_xscale('log', basex=2)
plt.show()

