#!/usr/bin/python
import pandas as pd
import matplotlib.pyplot as plt
from cycler import cycler
from math import log, ceil
import numpy as np

multichrome = cycler(color=list('bgrbgr')) + cycler(marker=['o','v','8','^','d','*']) + cycler(linestyle=['-', '--', ':' ,'-', '--', ':'])
multichrome2 = cycler(color=list('rcrc')) + cycler(marker=['8','|','*','^']) + cycler(linestyle=[':', '-.', ':' ,'-.'])

df_org = pd.read_csv('../results/timing_steps_random.raw_res', skipinitialspace=True)
df_org = df_org[(df_org['insertions'] % 10000000) == 0]
df_org['throughput'] = df_org['insertions'] / 1000000 / df_org['duration']
df_org['insertions'] = df_org['insertions'] / 100000000
del df_org['duration']
del df_org['dataset']
del df_org['type']

dfs_org = pd.read_csv('../results/stiming_steps_random.raw_res', skipinitialspace=True)
dfs_org = dfs_org[dfs_org['W'] == 10000000]
dfs_org = dfs_org[(dfs_org['insertions'] % 10000000) == 0]
dfs_org['throughput'] = dfs_org['insertions'] / 1000000 / dfs_org['duration']
dfs_org['insertions'] = dfs_org['insertions'] / 100000000
del dfs_org['duration']
del dfs_org['dataset']
del dfs_org['type']
del dfs_org['W']

heap_df_org = pd.read_csv('../results/timing_heap_steps_random.raw_res', skipinitialspace=True)
heap_df_org = heap_df_org[(heap_df_org['insertions'] % 10000000) == 0]
heap_df_org['throughput'] = heap_df_org['insertions'] / 1000000 / heap_df_org['duration']
heap_df_org['insertions'] = heap_df_org['insertions'] / 100000000
del heap_df_org['duration']
del heap_df_org['dataset']
del heap_df_org['type']
del heap_df_org['gamma']

skiplist_df_org = pd.read_csv('../results/timing_skiplist_steps_random.raw_res', skipinitialspace=True)
skiplist_df_org = skiplist_df_org[(skiplist_df_org['insertions'] % 10000000) == 0]
skiplist_df_org['throughput'] = skiplist_df_org['insertions'] / 1000000 / skiplist_df_org['duration']
skiplist_df_org['insertions'] = skiplist_df_org['insertions'] / 100000000
del skiplist_df_org['duration']
del skiplist_df_org['dataset']
del skiplist_df_org['type']
del skiplist_df_org['gamma']

#for g in [0.1, 0.05, 0.01, 0.005, 0.001]:
for g in [0.1]:
#        fig, ax1 = plt.subplots(1,1)
 #       ax1.set_prop_cycle(multichrome)
  #      ax1.grid()

	df_org_g = df_org[df_org['gamma'] == g]
	dfs_org_g = dfs_org[dfs_org['gamma'] == g]
	del df_org_g['gamma']
	del dfs_org_g['gamma']

	for s in [100000, 1000000]:
		fig, ax1 = plt.subplots(1,1)
        	ax1.set_prop_cycle(multichrome)
        	ax1.grid()

                size_heap_df = heap_df_org[(heap_df_org['size'] == s)]
                del size_heap_df['size']
                gh = size_heap_df.groupby(['insertions'])
                ax1.plot(gh.mean(), label='Heap', markersize=10)

                size_sl_df = skiplist_df_org[(skiplist_df_org['size'] == s)]
                del size_sl_df['size']
                gs = size_sl_df.groupby(['insertions'])
                ax1.plot(gs.mean(), label='SkipList', markersize=10)

	        size_df_g = df_org_g[(df_org_g['size'] == s)]
	        del size_df_g['size']
		gq = size_df_g.groupby(['insertions'])
		ax1.plot(gq.mean(), label='q-MAX', markersize=10)

		ax1.set_ylabel('CPU Throughput [MPPS]', fontsize=20)
       		ax1.set_xlabel(r'Trace length [$\times 10^8$ Packets]', fontsize=20)

        	# Put a legend to the right of the current axis
        	handles, labels = ax1.get_legend_handles_labels()
		if s == 100000:
      			ax1.legend(prop={'size': 16}, loc='right', framealpha=1.0, bbox_to_anchor=(1,0.5), ncol=2)
		else:
                        ax1.legend(prop={'size': 16}, loc='upper center', framealpha=1.0, bbox_to_anchor=(0.5,1), ncol=2)

        	ax1.set_ylim(ymin=0, ymax=250)
        	ax1.set_xlim(xmin=0)
        	plt.tick_params(axis='both', which='major', labelsize=16)
		plt.savefig('../figures/graph4-5-q='+str(s)+'.pdf', bbox_inches='tight')

	fig, ax2 = plt.subplots(1,1)
        ax2.set_prop_cycle(multichrome2)
        ax2.grid()

	for s in [100000, 1000000]:
                size_df_g = df_org_g[(df_org_g['size'] == s)]
                del size_df_g['size']
                gq = size_df_g.groupby(['insertions'])
                ax2.plot(gq.mean(), label=r'q-MAX,$q=10^{' + str(int(ceil(log(s,10)))) + r'},\gamma='+str(g)+r'$', markersize=10)

		size_df_g = dfs_org_g[dfs_org_g['size'] == s]
		del size_df_g['size']
		for t in [1]:
			size_df_g_t = size_df_g[(size_df_g['tau'] == t)]
			del size_df_g_t['tau']
		       	gq = size_df_g_t.groupby(['insertions'])
		       	ax2.plot(gq.mean(), label=r'$W=10^{7},q=10^{' + str(int(ceil(log(s,10)))) + r'}$', markersize=10)

	ax2.set_ylabel('CPU Throughput [MPPS]', fontsize=20)
	ax2.set_xlabel(r'Trace length [$\times 10^8$ Packets]', fontsize=20)

	# Put a legend to the right of the current axis
	handles, labels = ax1.get_legend_handles_labels()
	ax2.legend(prop={'size': 16}, loc='lower center', framealpha=1.0, bbox_to_anchor=(0.5,1), ncol=2)

	ax2.set_ylim(ymin=0)
	ax2.set_xlim(xmin=0)
	plt.tick_params(axis='both', which='major', labelsize=16)
        plt.savefig('../figures/graph7.pdf', bbox_inches='tight')

