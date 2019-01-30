#!/usr/bin/python
import pandas as pd
import matplotlib.pyplot as plt
from cycler import cycler

#multichrome = cycler(color=list('bgrcmyk')) + cycler(marker=['.',',','v','d','|','*','+']) + cycler(linestyle=['-', '--', '-.', ':', '-' ,'--', '-.'])
multichrome = cycler(color=list('bgrcmyk')) + cycler(marker=['o','^','v','d','8','*','X']) + cycler(linestyle=['-', '--', '-.', ':', '-' ,'--', '-.'])
fig, ax1 = plt.subplots(1,1)
ax1.set_prop_cycle(multichrome)
ax1.grid()

ax1.set_xlabel(r'Reservior size ($q$)', fontsize=20)
#ax1.set_yscale('log')
ax1.set_xscale('log')
ax1.set_ylabel('Throughput [MPPS]', fontsize=20)

df_org = pd.read_csv('../results/timing_random.raw_res', skipinitialspace=True)
df_org['throughput'] = df_org['insertions'] / 1000000 / df_org['duration']
del df_org['insertions']
del df_org['duration']
del df_org['dataset']

df_org = df_org[df_org['size'] >= 10000]

trace_h = df_org[(df_org['type'] == 'Heap')]
del trace_h['gamma']
gh = trace_h.groupby(['size'])
print("heap")
print(gh.mean())
ax1.plot(gh.mean(), label='Heap', markersize=10)

trace_s = df_org[(df_org['type'] == 'SkipList')]
del trace_s['gamma']
gs = trace_s.groupby(['size'])
print("skiplist")
print(gs.mean())
ax1.plot(gs.mean(), label='SkipList', markersize=10)

for l in [0.01, 0.05, 0.1]:
#for l in [0.5, 2]:
    trace_a = df_org[(df_org['type'] == 'AmortizedQMax')]
    trace_a = trace_a[(trace_a['gamma'] == l)]
    del trace_a['gamma']
    ga = trace_a.groupby(['size'])
    print ("gamma" + str(l))
    print(ga.mean())
    ax1.plot(ga.mean(), label=r'$q$-MAX,$\gamma=$' + str(l), markersize=10)

handles, labels = ax1.get_legend_handles_labels()
ax1.legend(handles, labels, loc=1, prop={'size': 16})
#ax1.legend(handles, labels, loc=3, prop={'size': 16})
fig.tight_layout()
plt.tick_params(axis='both', which='major', labelsize=16)
#ax1.set_yscale('log', basey=2)
#plt.yscale('log')
plt.show()
