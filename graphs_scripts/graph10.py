#!/usr/bin/python
import pandas as pd
import matplotlib.pyplot as plt
from cycler import cycler
from math import log, ceil
import numpy as np
import sys
import itertools

#multichrome = cycler(color=list('bgrbgr')) + cycler(marker=['o','^','v','d','8','*']) + cycler(linestyle=['-', '--', '-.', '-', '--' ,'-.'])
hcs = cycler(color=list('bb')) + cycler(marker=['o','^']) + cycler(linestyle=['-', '-'])
scs = cycler(color=list('gg')) + cycler(marker=['v','d']) + cycler(linestyle=['--', '--'])
multichrome = cycler(color=list('rr')) + cycler(marker=['8','*']) + cycler(linestyle=[':', ':'])

fig, ax1 = plt.subplots(1,1)
ax1.set_prop_cycle(multichrome)
ax1.grid()

ax1.set_ylabel('CPU Throughput [MPPS]', fontsize=20)
ax1.set_xlabel(r'Performance parameter ($\gamma$)', fontsize=20)

df_org = pd.read_csv('../results/ed-0.75.raw_res', skipinitialspace=True)
df_org['throughput'] = df_org['insertions'] / 1000000 / df_org['duration']
del df_org['insertions']
del df_org['duration']
del df_org['dataset']
del df_org['c']
heap_df_org = df_org[(df_org['type'] == 'Heap')]
skiplist_df_org = df_org[(df_org['type'] == 'SkipList')]
qmax_df_org = df_org[(df_org['type'] == 'AmortizedQMax')]

sizes=map(lambda s : 10**int(s), sys.argv[1:])
for s, hc ,sc in itertools.izip(sizes, hcs, scs):
	p = str(int(ceil(log(s,10))))

	heap_size_df = heap_df_org[(heap_df_org['size'] == s)]
	del heap_size_df['size']
	gh = heap_size_df.mean()
#	ax1.axhline(y=gh['throughput'], label=r'Heap, $q=10^{' + p + r'}$', markersize=10, color=hc['color'], marker=hc['marker'], linestyle=hc['linestyle'])
        ax1.axhline(y=gh['throughput'], label='Heap', markersize=10, color=hc['color'], marker=hc['marker'], linestyle=hc['linestyle'])

	skiplist_size_df = skiplist_df_org[(skiplist_df_org['size'] == s)]
	del skiplist_size_df['size']
	gs = skiplist_size_df.mean()
#	ax1.axhline(y=gs['throughput'], label=r'SkipList, $q=10^{' + p + r'}$', markersize=10, color=sc['color'], marker=sc['marker'], linestyle=sc['linestyle'])
        ax1.axhline(y=gs['throughput'], label='SkipList', markersize=10, color=sc['color'], marker=sc['marker'], linestyle=sc['linestyle'])

	qmax_df_org = qmax_df_org[qmax_df_org['gamma'] <= 4]
	qmax_df_org = qmax_df_org[qmax_df_org['gamma'] >= 0.05]

	qmax_size_df = qmax_df_org[(qmax_df_org['size'] == s)]
	del qmax_size_df['size']
	gq = qmax_size_df.groupby(['gamma'])
	print gq.mean()
#	ax1.plot(gq.mean(), label=r'q-MAX, $q=10^{' + p + r'}$', markersize=10)
        ax1.plot(gq.mean(), label='q-MAX', markersize=10)

handles, labels = ax1.get_legend_handles_labels()
ax1.legend(prop={'size': 24}, loc='upper left', bbox_to_anchor=(0,1), ncol=1)
fig.tight_layout()

ax1.set_xticks(np.arange(9))
plt.tick_params(axis='both', which='major', labelsize=16)
plt.xlim(xmin=0.05, xmax=4)
plt.ylim(ymin=0, ymax=11)
ax1.set_xscale('log', basex=2)
#plt.show()
for item in ([ax1.xaxis.label, ax1.yaxis.label] +
             ax1.get_xticklabels() + ax1.get_yticklabels()):
    item.set_fontsize(24)
plt.savefig('graph10-q='+str(sizes)+'.png', bbox_inches='tight')
