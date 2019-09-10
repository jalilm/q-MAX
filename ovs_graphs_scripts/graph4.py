#!/usr/bin/python
import pandas as pd
import matplotlib.pyplot as plt
from cycler import cycler
import sys

G = sys.argv[1]
sizes=map(lambda s : 10**int(s), sys.argv[2:])
ss = len(sizes)

def mfunc(syms):
    while True:
        for s in syms:
            yield s

if G == "CPU":
        y_label='CPU Throughput [MPPS]'
	df_org = pd.read_csv('../results/timing_random.raw_res', skipinitialspace=True)
	df_org['throughput'] = df_org['insertions'] / 1000000 / df_org['duration']
	del df_org['insertions']
	del df_org['duration']
	del df_org['dataset']
	save_dir="../figures/"
	markers = ['o','^','*','d','8']
	multichrome_list = list('bgrcm')
	legend_location='upper right'
	bbox_place=(1,1)
else:
        y_label='OVS Throughput [Gbps]'
	df_org = pd.read_csv('../ovs_results/graph3_' + G + '.txt', skipinitialspace=True)
	df_org['throughput'] = df_org['throughput'] / 1000000000
	save_dir="../ovs_figures/"
	markers = ['v','o','^','*','d','8']
	multichrome_list = list('ybgrcm')
	legend_location='lower left'
	bbox_place=(0,0)

mark = mfunc(markers)

multichrome = cycler(color=multichrome_list) #+ cycler(linestyle=['-.','-', '--', ':', '-.', '-'])
fig, ax1 = plt.subplots(1,1)
ax1.set_prop_cycle(multichrome)
#ax1.grid()
ax1.set_xlabel(r'Reservior size ($q$)', fontsize=20)
ax1.set_xscale('log')
ax1.set_ylabel(y_label, fontsize=20)


if G != "CPU":
	# OVS
	trace_o = df_org[(df_org['type'] == 'OVS')]
	#ax1.axhline(y=oh['throughput'], label='OVS', markersize=10, color='y', marker='d', linestyle='-.')
	ax1.errorbar(sizes,[trace_o.mean()['throughput']]*ss, yerr=[trace_o.std()['throughput']]*ss, label='OVS', markersize=10, marker=next(mark), fmt='o', linewidth=2, capsize=10, elinewidth=3, capthick=1.5)

df_org = df_org[df_org['size'].isin(sizes)]

# Heap
trace_h = df_org[(df_org['type'] == 'Heap')]
del trace_h['gamma']
gh = trace_h.groupby(['size']).agg(['mean', 'std']).reset_index()['throughput']
#ax1.plot(gh_mean, label='Heap', markersize=10)
ax1.errorbar(sizes, gh['mean'], yerr=gh['std'], label='Heap', markersize=10, marker=next(mark), fmt='o', linewidth=2, capsize=10, elinewidth=3, capthick=1.5)

# SkipList
trace_s = df_org[(df_org['type'] == 'SkipList')]
del trace_s['gamma']
gs = trace_s.groupby(['size']).agg(['mean', 'std']).reset_index()['throughput']
#ax1.plot(gs_mean, label='SkipList', markersize=10)
ax1.errorbar(sizes, gs['mean'], yerr=gs['std'], label='SkipList', markersize=10, marker=next(mark), fmt='o', linewidth=2, capsize=10, elinewidth=3, capthick=1.5)

for l in [0.05, 0.1, 0.25]:
    if G == "CPU":
	q_max_filter_string='AmortizedQMax'
    else:
	q_max_filter_string='QMax'
    trace_a = df_org[(df_org['type'] == q_max_filter_string)]
    trace_a = trace_a[(trace_a['gamma'] == l)]
    del trace_a['gamma']
    ga = trace_a.groupby(['size']).agg(['mean', 'std']).reset_index()['throughput']
    #ax1.plot(ga_mean, label=r'$q$-MAX,$\gamma=$' + str(l), markersize=10)
    ax1.errorbar(sizes, ga['mean'], yerr=ga['std'], label=r'$q$-MAX,$\gamma=$' + str(l), markersize=10, marker=next(mark), fmt='o', linewidth=2, capsize=10, elinewidth=3, capthick=1.5)

handles, labels = ax1.get_legend_handles_labels()
ax1.legend(handles, labels, loc=legend_location, bbox_to_anchor=bbox_place, ncol=1, prop={'size': 16})
plt.xlim(xmin=10**4, xmax=10**7)
fig.tight_layout()
plt.tick_params(axis='both', which='major', labelsize=16)
#plt.show()
plt.savefig(save_dir+'graph4-'+G+'.pdf', bbox_inches='tight')

