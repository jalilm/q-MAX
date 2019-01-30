#!/usr/bin/python
import pandas as pd
import matplotlib.pyplot as plt
from cycler import cycler
from math import log, ceil
import numpy as np

multichrome = cycler(color=list('rcmyk')) + cycler(marker=['v','d','|','*','8']) + cycler(linestyle=['-.', ':', '-' ,'--', '-.'])

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

#for g in [0.1, 0.05, 0.01, 0.005, 0.001]:
for g in [0.1]:
        fig, ax1 = plt.subplots(1,1)
        ax1.set_prop_cycle(multichrome)
        ax1.grid()

	df_org_g = df_org[df_org['gamma'] == g]
	dfs_org_g = dfs_org[dfs_org['gamma'] == g]
	del df_org_g['gamma']
	del dfs_org_g['gamma']

	for s in [100000, 1000000]:
	        size_df_g = df_org_g[(df_org_g['size'] == s)]
	        del size_df_g['size']
		gq = size_df_g.groupby(['insertions'])
		ax1.plot(gq.mean(), label=r'$q$=$10^{' + str(int(ceil(log(s,10)))) + r'}$,$\gamma=$'+str(g), markersize=10)

	for s in [100000, 1000000]:
		size_df_g = dfs_org_g[dfs_org_g['size'] == s]
		del size_df_g['size']
		for t in [1]:
			size_df_g_t = size_df_g[(size_df_g['tau'] == t)]
			del size_df_g_t['tau']
		       	gq = size_df_g_t.groupby(['insertions'])
		       	ax1.plot(gq.mean(), label=r'$W$=$10^{7}$,$q$=$10^{' + str(int(ceil(log(s,10)))) + r'}$', markersize=10)

	ax1.set_ylabel('Throughput [MPPS]', fontsize=20)
	ax1.set_xlabel(r'Trace length [$\times 10^8$ Packets]', fontsize=20)

	# Put a legend to the right of the current axis
	handles, labels = ax1.get_legend_handles_labels()
	ax1.legend(prop={'size': 16}, loc='lower center', framealpha=1.0, bbox_to_anchor=(0.5,1), ncol=2)

	ax1.set_ylim(ymin=0)
	ax1.set_xlim(xmin=0)
	plt.tick_params(axis='both', which='major', labelsize=16)
	plt.show()

