#!/usr/bin/python
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from cycler import cycler
import sys
import pylab

fig, ax = plt.subplots(1,1)
bar_cycle = cycler(color=list('bgrcmyw')) + cycler(hatch=['--', '++', 'xX', '\\\\', '**', '..', 'OO'])
styles = bar_cycle()
ax.set_ylabel('OVS Throughput [Gbps]', fontsize=20)
ax.yaxis.grid(True)

G=str(sys.argv[1])

df_c = pd.read_csv('../ovs_results/graph5_'+G+'_caida16.txt', skipinitialspace=True)
df_univ = pd.read_csv('../ovs_results/graph5_'+G+'_univ1.txt', skipinitialspace=True)
df_c18 = pd.read_csv('../ovs_results/graph5_'+G+'_caida18.txt', skipinitialspace=True)
df_org_1 = pd.concat([df_c, df_univ, df_c18])
df_org_1 = df_org_1.replace(np.nan, 0)
df_org_1['throughput'] = df_org_1['throughput'] / 1000000000

sizes=map(lambda s : 10**int(s), sys.argv[2:])
for s in sizes:
	df_org = df_org_1[df_org_1['size'] == s]
	del df_org['size']

	df_org['type'] = df_org.apply(
	    lambda row: r'$q$-MAX,$\gamma=$' + str(row['gamma']) if row['type'] == 'QMax'
	    else row['type'], axis=1)
	j = df_org.groupby(['type', 'dataset'])['throughput']
	jmean = j.mean().unstack(0)
	jstd = j.sem().unstack(0)
	jlen = j.size().unstack(0)
	print j
	print jmean
	print jstd
	print jlen
	traces = ["caida16", "univ1", "caida18"]

	w=-0.6
	for c in ["Heap", "Skiplist", r'$q$-MAX,$\gamma=$0.05', r'$q$-MAX,$\gamma=$0.1', r'$q$-MAX,$\gamma=$0.25', r'$q$-MAX,$\gamma=$0.5', 'OVS']:
	    print c
	    y = []
	    yerr = []
	    for t in traces:
		print t
		mean = jmean[c][t]
		size = jlen[c][t]
#		print mean
#		print size
		measurments = df_org[(df_org['type']==c) & (df_org['dataset']==t)]['throughput']
#		print measurments
		sos = sum([(x - mean)**2 for x in measurments])
#		print sos
	        stdev = (sos / (size - 1) ) ** 0.5
	        t_9 = 3.25 # t statistic for 9 degrees of freedom @ 0.99 confidence two tail
	        err = max(stdev * t_9 / pow(size, 0.5),1e-7)
#		print err
#		print jstd[c][t]
		y.append(mean)
	        yerr.append(err)
	    ax.bar([x+w for x in [1,3,5]],y, yerr=yerr, error_kw=dict(ecolor='r', lw=3, capsize=7, capthick=1), width=0.2, alpha=1, edgecolor='k', linewidth=1, label=c, **next(styles))
	    w+=0.2
	ax.set_xticks([1,3,5])
	ax.set_xticklabels([x.upper() for x in traces], fontsize=16)
	plt.tick_params(axis='y', which='major', labelsize=16)
#	ax.legend(prop={'size': 16}, loc='lower center', framealpha=1.0, bbox_to_anchor=(0.5,1), ncol=2)
	fig.tight_layout()
#	plt.show()
	plt.savefig('../ovs_figures/graph5-'+G+'-q='+str(sizes)+'.pdf', bbox_inches='tight')

figlegend=pylab.figure()
pylab.figlegend(*ax.get_legend_handles_labels(), fontsize=20, loc = 'upper left', ncol=4)
figlegend.savefig("../ovs_figures/ovs-legend.pdf", bbox_inches='tight')


