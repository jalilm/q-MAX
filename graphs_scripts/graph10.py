#!/usr/bin/python
import pandas as pd
import matplotlib.pyplot as plt
from cycler import cycler
from math import log, ceil
import numpy as np
import sys
import itertools
import math
from matplotlib import container

#multichrome = cycler(color=list('bgrbgr')) + cycler(marker=['o','^','v','d','8','*']) + cycler(linestyle=['-', '--', '-.', '-', '--' ,'-.'])
hcs = cycler(color=list('bb')) + cycler(marker=['o','^']) + cycler(linestyle=['-', '-'])
scs = cycler(color=list('gg')) + cycler(marker=['v','d']) + cycler(linestyle=['--', '--'])
multichrome = cycler(color=list('rr')) + cycler(marker=['8','*']) + cycler(linestyle=[':', ':'])

fig, ax1 = plt.subplots(1,1)
ax1.set_prop_cycle(multichrome)
#ax1.grid()
x_max = 4
x_min = 0.05
x_mid = 2**((math.log(x_max,2) + math.log(x_min,2))/2)

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
        #ax1.axhline(y=gh['throughput'], label='Heap', markersize=10, color=hc['color'], marker=hc['marker'], linestyle=hc['linestyle'])
	ax1.errorbar(x_mid,heap_size_df.mean()['throughput'], yerr=0.1, label='Heap', fmt='o', linewidth=2, capsize=1000, elinewidth=3, capthick=1.5 ,markersize=10, color=hc['color'], marker=hc['marker'])

	skiplist_size_df = skiplist_df_org[(skiplist_df_org['size'] == s)]
	del skiplist_size_df['size']
	gs = skiplist_size_df.mean()
#	ax1.axhline(y=gs['throughput'], label=r'SkipList, $q=10^{' + p + r'}$', markersize=10, color=sc['color'], marker=sc['marker'], linestyle=sc['linestyle'])
        #ax1.axhline(y=gs['throughput'], label='SkipList', markersize=10, color=sc['color'], marker=sc['marker'], linestyle=sc['linestyle'])
	ax1.errorbar(x_mid, skiplist_size_df.mean()['throughput'],  yerr=0.15, label='SkipList', fmt='o', linewidth=2, capsize=1000, elinewidth=3, capthick=1.5 ,markersize=10, color=sc['color'], marker=sc['marker'])

	qmax_df_org = qmax_df_org[qmax_df_org['gamma'] <= 4]
	qmax_df_org = qmax_df_org[qmax_df_org['gamma'] >= 0.05]

	qmax_size_df = qmax_df_org[(qmax_df_org['size'] == s)]
	del qmax_size_df['size']
	gq = qmax_size_df.groupby(['gamma']).agg(['mean','std']).reset_index()
	gq.fillna(0, inplace=True)
	print gq['throughput']['mean']
	print gq['gamma']
	print gq['throughput']['std']
#	ax1.plot(gq.mean(), label=r'q-MAX, $q=10^{' + p + r'}$', markersize=10)
#        ax1.plot(gq.mean(), label='q-MAX', markersize=10)
	ax1.errorbar(gq['gamma'], gq['throughput']['mean'],  yerr=gq['throughput']['std'], label='q-MAX', fmt='o', linewidth=2, capsize=10, elinewidth=3, capthick=1.5 ,markersize=10)


#Plot configurations
handles, labels = ax1.get_legend_handles_labels()
new_handles = []
for h in handles:
	#only need to edit the errorbar legend entries
	if isinstance(h, container.ErrorbarContainer):
		new_handles.append(h[0])
	else:
		new_handles.append(h)
ax1.legend(new_handles, labels, prop={'size': 16}, loc='upper left', bbox_to_anchor=(0,1), ncol=1)
fig.tight_layout()

ax1.set_xticks(np.arange(9))
plt.tick_params(axis='both', which='major', labelsize=16)
plt.xlim(xmin=x_min, xmax=x_max)
plt.ylim(ymin=0, ymax=11)
ax1.set_xscale('log', basex=2)
#plt.show()
for item in ([ax1.xaxis.label, ax1.yaxis.label] +
             ax1.get_xticklabels() + ax1.get_yticklabels()):
    item.set_fontsize(24)
#plt.show()
plt.savefig('../figures/graph10-q='+str(sizes)+'.pdf', bbox_inches='tight')

