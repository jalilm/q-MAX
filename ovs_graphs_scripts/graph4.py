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

markers = ['v','o','^','*','d','8']
mark = mfunc(markers)

multichrome = cycler(color=list('ybgrcm')) + cycler(linestyle=['-.','-', '--', ':', '-.', '-'])
fig, ax1 = plt.subplots(1,1)
ax1.set_prop_cycle(multichrome)
ax1.grid()

ax1.set_xlabel(r'Reservior size ($q$)', fontsize=20)
ax1.set_xscale('log')
ax1.set_ylabel('OVS Throughput [Gbps]', fontsize=20)

df_org = pd.read_csv('../ovs_results/graph3_' + G+ 'G.txt', skipinitialspace=True)
df_org['throughput'] = df_org['throughput'] / 1000000000

# OVS
trace_o = df_org[(df_org['type'] == 'OVS')]
oh = trace_o.mean()
#ax1.axhline(y=oh['throughput'], label='OVS', markersize=10, color='y', marker='d', linestyle='-.')
ax1.errorbar(sizes,[trace_o.mean()['throughput']]*ss, yerr=[trace_o.std()['throughput']]*ss, label='OVS', markersize=10, marker=next(mark))

df_org = df_org[df_org['size'].isin(sizes)]

# Heap
trace_h = df_org[(df_org['type'] == 'Heap')]
del trace_h['gamma']
gh = trace_h.groupby(['size']).agg(['mean', 'std']).reset_index()['throughput']
#ax1.plot(gh_mean, label='Heap', markersize=10)
ax1.errorbar(sizes, gh['mean'], yerr=gh['std'], label='Heap', markersize=10, marker=next(mark))

# SkipList
trace_s = df_org[(df_org['type'] == 'Skiplist')]
del trace_s['gamma']
gs = trace_s.groupby(['size']).agg(['mean', 'std']).reset_index()['throughput']
#ax1.plot(gs_mean, label='SkipList', markersize=10)
ax1.errorbar(sizes, gs['mean'], yerr=gs['std'], label='SkipList', markersize=10, marker=next(mark))

for l in [0.05, 0.1, 0.25]:
    trace_a = df_org[(df_org['type'] == 'QMax')]
    trace_a = trace_a[(trace_a['gamma'] == l)]
    del trace_a['gamma']
    ga = trace_a.groupby(['size']).agg(['mean', 'std']).reset_index()['throughput']
    #ax1.plot(ga_mean, label=r'$q$-MAX,$\gamma=$' + str(l), markersize=10)
    ax1.errorbar(sizes, ga['mean'], yerr=ga['std'], label=r'$q$-MAX,$\gamma=$' + str(l), markersize=10, marker=next(mark))

handles, labels = ax1.get_legend_handles_labels()
ax1.legend(handles, labels, loc='lower left', bbox_to_anchor=(0,0), ncol=1, prop={'size': 16})
plt.xlim(xmin=10**4, xmax=10**7)
#ax1.legend(handles, labels, loc=3, prop={'size': 16})
fig.tight_layout()
plt.tick_params(axis='both', which='major', labelsize=16)
#ax1.set_yscale('log', basey=2)
#plt.yscale('log')
#plt.show()
plt.savefig('graph4-'+G+'G.png', bbox_inches='tight')

