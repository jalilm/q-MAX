#!/usr/bin/python
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from cycler import cycler
import sys

fig, ax = plt.subplots(1,1)
bar_cycle = cycler(color=['b','g','r','c','m','y','darkred','gold','sienna']) + cycler(hatch=['--', '++', 'xX', '\\\\', '**', '..', 'OO', '//', 'oo'])
styles = bar_cycle()
ax.set_ylabel('Cache Throughput [MRPS]', fontsize=20)
ax.yaxis.grid(True)

df_org = pd.read_csv('../results/lrfu_P1.raw_res', skipinitialspace=True)
df_org = df_org.replace(np.nan, 0)
df_org['throughput'] = df_org['insertions'] / 1000000 / df_org['duration']
del df_org['insertions']
del df_org['duration']
del df_org['c']
del df_org['dataset']

df_org['type'] = df_org.apply(lambda row: r'$q$-MAX,$\gamma=$' + str(row['gamma']) if row['type'] == 'AmortizedQMax'
	    else row['type'], axis=1)
j = df_org.groupby(['type', 'size'])['throughput'].mean().unstack(0)
jstd = df_org.groupby(['type', 'size'])['throughput'].sem().unstack(0)
sizes_labels = [r'$q=10^4$', r'$q=10^5$', r'$q=10^6$']
sizes = [10000, 100000, 1000000]

w=-0.8
for c in ["Heap", "SkipList", r'$q$-MAX,$\gamma=$0.05', r'$q$-MAX,$\gamma=$0.1', r'$q$-MAX,$\gamma=$0.25', r'$q$-MAX,$\gamma=$0.5', r'$q$-MAX,$\gamma=$1.0', r'$q$-MAX,$\gamma=$2.0']:
    print c
    print j[c]
    y = []
    yerr = []
    for s in sizes:
        y.append(j[c][s])
        yerr.append(jstd[c][s])
    ax.bar([x+w for x in [1,3,5]],y, yerr=yerr, error_kw=dict(ecolor='r', lw=3, capsize=7, capthick=1), width=0.2, alpha=1, edgecolor='k', linewidth=1, label=c, **next(styles))
    w+=0.2
ax.set_xticks([1,3,5])
ax.set_xticklabels(sizes_labels, fontsize=16)
plt.tick_params(axis='y', which='major', labelsize=16)
ax.legend(prop={'size': 16}, loc='lower center', framealpha=1.0, bbox_to_anchor=(0.5,1), ncol=2)
fig.tight_layout()
#plt.show()
plt.savefig('../figures/graph_lrfu.pdf', bbox_inches='tight')

