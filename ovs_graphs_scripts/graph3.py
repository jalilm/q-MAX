#!/usr/bin/python
import pandas as pd
import matplotlib.pyplot as plt
from cycler import cycler
from math import log, ceil
import numpy as np
import sys
import itertools

G = sys.argv[1]
sizes=map(lambda s : 10**int(s), sys.argv[2:])
ss = len(sizes)

ocs = cycler(color=['y']*ss) + cycler(marker=['d']*ss) + cycler(linestyle=['-.']*ss)
hcs = cycler(color=['b']*ss) + cycler(marker=['o']*ss) + cycler(linestyle=['-']*ss)
scs = cycler(color=['g']*ss) + cycler(marker=['v']*ss) + cycler(linestyle=['--']*ss)
qcs = cycler(color=['r']*ss) + cycler(marker=['*']*ss) + cycler(linestyle=[':']*ss)

df_org = pd.read_csv('../ovs_results/graph3_' + G + 'G.txt', skipinitialspace=True)
df_org['throughput'] = df_org['throughput'] / 1000000000
heap_df_org = df_org[(df_org['type'] == 'Heap')]
skiplist_df_org = df_org[(df_org['type'] == 'Skiplist')]
qmax_df_org = df_org[(df_org['type'] == 'QMax')]
ovs_df_org = df_org[(df_org['type'] == 'OVS')]
del ovs_df_org['size']

sizes=map(lambda s : 10**int(s), sys.argv[2:])
for s, oc, hc, sc, qc in itertools.izip(sizes, ocs, hcs, scs, qcs):
	fig, ax1 = plt.subplots(1,1)
	ax1.grid()
	ax1.set_ylabel('OVS Throughput [Gbps]', fontsize=20)
	ax1.set_xlabel(r'Performance parameter ($\gamma$)', fontsize=20)

	#q-MAX prepration
#       qmax_df_org = qmax_df_org[qmax_df_org['gamma'] <= 0.1]
#       qmax_df_org = qmax_df_org[qmax_df_org['gamma'] >= 0.0001]
        qmax_size_df = qmax_df_org[(qmax_df_org['size'] == s)]
        del qmax_size_df['size']
        gq = qmax_size_df.groupby(['gamma']).agg(['mean', 'std'])
        gq = gq.reset_index()
	sg=len(gq['gamma'])
	

	#OVS
        #ax1.axhline(y=ovs_df_org.mean()['throughput'], label='OVS', markersize=10, color=oc['color'], marker=oc['marker'], linestyle=oc['linestyle'])
	ax1.errorbar(gq['gamma'],[ovs_df_org.mean()['throughput']]*sg, yerr=[ovs_df_org.std()['throughput']]*sg, label='OVS', markersize=10, color=oc['color'], marker=oc['marker'], linestyle=oc['linestyle'])

	# Heap
	heap_size_df = heap_df_org[(heap_df_org['size'] == s)]
	del heap_size_df['size']
	gh = heap_size_df.mean()
        #ax1.axhline(y=gh['throughput'], label='Heap', markersize=10, color=hc['color'], marker=hc['marker'], linestyle=hc['linestyle'])
        ax1.errorbar(gq['gamma'],[heap_size_df.mean()['throughput']]*sg, yerr=[heap_size_df.std()['throughput']]*sg, label='Heap', markersize=10, color=hc['color'], marker=hc['marker'], linestyle=hc['linestyle'])


	# Skiplist
	skiplist_size_df = skiplist_df_org[(skiplist_df_org['size'] == s)]
	del skiplist_size_df['size']
	gs = skiplist_size_df.mean()
        #ax1.axhline(y=gs['throughput'], label='SkipList', markersize=10, color=sc['color'], marker=sc['marker'], linestyle=sc['linestyle'])
        ax1.errorbar(gq['gamma'],[skiplist_size_df.mean()['throughput']]*sg, yerr=[skiplist_size_df.std()['throughput']]*sg, label='SkipList', markersize=10, color=sc['color'], marker=sc['marker'], linestyle=sc['linestyle'])

	#q-MAX
        #ax1.plot(gq.mean(), label='q-MAX', markersize=10)
	ax1.errorbar(gq['gamma'],gq['throughput']['mean'], yerr=gq['throughput']['std'], label='q-MAX', markersize=10, color=qc['color'], marker=qc['marker'], linestyle=qc['linestyle'])


	#Plot configurations
	handles, labels = ax1.get_legend_handles_labels()
	ax1.legend(prop={'size': 16}, loc='lower right', bbox_to_anchor=(1,0), ncol=1)
	fig.tight_layout()

	ax1.set_xticks(np.arange(sg))
	plt.tick_params(axis='both', which='major', labelsize=16)
	plt.xlim(xmin=0.01, xmax=1)
	plt.ylim(ymin=0, ymax=int(G)+1)
	ax1.set_xscale('log', basex=2)
	for item in ([ax1.xaxis.label, ax1.yaxis.label] +
	             ax1.get_xticklabels() + ax1.get_yticklabels()):
	    item.set_fontsize(24)
	#plt.show()
	plt.savefig('graph3-'+G+'G-q='+str([s])+'.png', bbox_inches='tight')
