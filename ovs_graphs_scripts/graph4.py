#!/usr/bin/python
import pandas as pd
import matplotlib.pyplot as plt
from cycler import cycler

#multichrome = cycler(color=list('bgrcmyk')) + cycler(marker=['.',',','v','d','|','*','+']) + cycler(linestyle=['-', '--', '-.', ':', '-' ,'--', '-.'])
multichrome = cycler(color=list('bgrcmyk')) + cycler(marker=['o','^','*','d','8','v','X']) + cycler(linestyle=['-', '--', ':', '-.', '-' ,'--', '-.'])
fig, ax1 = plt.subplots(1,1)
ax1.set_prop_cycle(multichrome)
ax1.grid()

ax1.set_xlabel(r'Reservior size ($q$)', fontsize=20)
#ax1.set_yscale('log')
ax1.set_xscale('log')
ax1.set_ylabel('OVS Throughput [Gbps]', fontsize=20)

df_org = pd.read_csv('../qmax_ovs_res/graph3_40G.txt', skipinitialspace=True)
df_org['throughput'] = df_org['throughput'] / 1000000000

trace_o = df_org[(df_org['type'] == 'OVS')]
oh = trace_o.mean()
ax1.axhline(y=oh['throughput'], label='OVS', markersize=10, color='y', marker='d', linestyle='-.')

trace_h = df_org[(df_org['type'] == 'Heap')]
del trace_h['gamma']
gh = trace_h.groupby(['size'])
gh_mean = gh.mean()
ax1.plot(gh_mean, label='Heap', markersize=10)

trace_s = df_org[(df_org['type'] == 'Skiplist')]
del trace_s['gamma']
gs = trace_s.groupby(['size'])
gs_mean = gs.mean()
ax1.plot(gs_mean, label='SkipList', markersize=10)

for l in [0.05, 0.1, 0.25]:
    trace_a = df_org[(df_org['type'] == 'QMax')]
    trace_a = trace_a[(trace_a['gamma'] == l)]
    del trace_a['gamma']
    ga = trace_a.groupby(['size'])
    ga_mean = ga.mean()
    ax1.plot(ga_mean, label=r'$q$-MAX,$\gamma=$' + str(l), markersize=10)

handles, labels = ax1.get_legend_handles_labels()
ax1.legend(handles, labels, loc='lower left', bbox_to_anchor=(0,0), ncol=1, prop={'size': 16})
plt.xlim(xmin=10**4, xmax=10**7)
#ax1.legend(handles, labels, loc=3, prop={'size': 16})
fig.tight_layout()
plt.tick_params(axis='both', which='major', labelsize=16)
#ax1.set_yscale('log', basey=2)
#plt.yscale('log')
#plt.show()
plt.savefig('graph4-40G.png', bbox_inches='tight')

