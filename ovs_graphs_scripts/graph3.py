#!/usr/bin/python
import pandas as pd
import matplotlib.pyplot as plt
from cycler import cycler
from math import log, ceil
import numpy as np
import sys
import itertools

#multichrome = cycler(color=list('bgrbgr')) + cycler(marker=['o','^','v','d','8','*']) + cycler(linestyle=['-', '--', '-.', '-', '--' ,'-.'])
ocs = cycler(color=list('yy')) + cycler(marker=['d','^']) + cycler(linestyle=['-.', '-.'])
hcs = cycler(color=list('bb')) + cycler(marker=['o','^']) + cycler(linestyle=['-', '-'])
scs = cycler(color=list('gg')) + cycler(marker=['v','d']) + cycler(linestyle=['--', '--'])
multichrome = cycler(color=list('r')) + cycler(marker=['*']) + cycler(linestyle=[':'])

fig, ax1 = plt.subplots(1,1)
ax1.set_prop_cycle(multichrome)
ax1.grid()

ax1.set_ylabel('OVS Throughput [Gbps]', fontsize=20)
ax1.set_xlabel(r'Performance parameter ($\gamma$)', fontsize=20)

df_org = pd.read_csv('../qmax_ovs_res/graph3_40G.txt', skipinitialspace=True)
df_org['throughput'] = df_org['throughput'] / 1000000000
heap_df_org = df_org[(df_org['type'] == 'Heap')]
skiplist_df_org = df_org[(df_org['type'] == 'Skiplist')]
qmax_df_org = df_org[(df_org['type'] == 'QMax')]
ovs_df_org = df_org[(df_org['type'] == 'OVS')]


sizes=map(lambda s : 10**int(s), sys.argv[1:])
for s, hc ,sc , oc in itertools.izip(sizes, hcs, scs, ocs):
	p = str(int(ceil(log(s,10))))
	del ovs_df_org['size']
	oh = ovs_df_org.mean()
        ax1.axhline(y=oh['throughput'], label='OVS', markersize=10, color=oc['color'], marker=oc['marker'], linestyle=oc['linestyle'])

	heap_size_df = heap_df_org[(heap_df_org['size'] == s)]
	del heap_size_df['size']
	gh = heap_size_df.mean()
        ax1.axhline(y=gh['throughput'], label='Heap', markersize=10, color=hc['color'], marker=hc['marker'], linestyle=hc['linestyle'])

	skiplist_size_df = skiplist_df_org[(skiplist_df_org['size'] == s)]
	del skiplist_size_df['size']
	gs = skiplist_size_df.mean()
        ax1.axhline(y=gs['throughput'], label='SkipList', markersize=10, color=sc['color'], marker=sc['marker'], linestyle=sc['linestyle'])

#	qmax_df_org = qmax_df_org[qmax_df_org['gamma'] <= 0.1]
#	qmax_df_org = qmax_df_org[qmax_df_org['gamma'] >= 0.0001]

	qmax_size_df = qmax_df_org[(qmax_df_org['size'] == s)]
	del qmax_size_df['size']
	gq = qmax_size_df.groupby(['gamma'])
        ax1.plot(gq.mean(), label='q-MAX', markersize=10)

handles, labels = ax1.get_legend_handles_labels()
ax1.legend(prop={'size': 16}, loc='lower right', bbox_to_anchor=(1,0), ncol=1)
fig.tight_layout()

ax1.set_xticks(np.arange(9))
plt.tick_params(axis='both', which='major', labelsize=16)
plt.xlim(xmin=0.01, xmax=1)
plt.ylim(ymin=0, ymax=41)
ax1.set_xscale('log', basex=2)
#plt.show()
for item in ([ax1.xaxis.label, ax1.yaxis.label] +
             ax1.get_xticklabels() + ax1.get_yticklabels()):
    item.set_fontsize(24)
plt.savefig('graph3-40G-q='+str(sizes)+'.png', bbox_inches='tight')
