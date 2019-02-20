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
ax1.set_ylabel('CPU Throughput [MPPS]', fontsize=20)

df_org = pd.read_csv('../results/timing_random.raw_res', skipinitialspace=True)
df_org['throughput'] = df_org['insertions'] / 1000000 / df_org['duration']
del df_org['insertions']
del df_org['duration']
del df_org['dataset']

df_org = df_org[df_org['size'] >= 10000]

trace_h = df_org[(df_org['type'] == 'Heap')]
del trace_h['gamma']
gh = trace_h.groupby(['size'])
gh_mean = gh.mean()
ax1.plot(gh_mean, label='Heap', markersize=10)

trace_s = df_org[(df_org['type'] == 'SkipList')]
del trace_s['gamma']
gs = trace_s.groupby(['size'])
gs_mean = gs.mean()
ax1.plot(gs_mean, label='SkipList', markersize=10)

#combined_df = pd.concat([gh_mean, gs_mean])
#min_df = combined_df.min(level=0)
#max_df = combined_df.max(level=0)

for l in [0.05, 0.1, 0.25]:
#for l in [0.5, 2]:
    trace_a = df_org[(df_org['type'] == 'AmortizedQMax')]
    trace_a = trace_a[(trace_a['gamma'] == l)]
    del trace_a['gamma']
    ga = trace_a.groupby(['size'])
    print ("gamma" + str(l))
    ga_mean = ga.mean()
    heap_speedups = (ga_mean / gh_mean)['throughput']
    sl_speedups = (ga_mean / gs_mean)['throughput']
    print("Heap min " + str(heap_speedups.min()))
    print("Heap max " + str(heap_speedups.max()))
    print("SkipList min " + str(sl_speedups.min()))
    print("SkipList max " + str(sl_speedups.max()))

#    print("min" + str((ga_mean / max_df)['throughput'].min()))
#    print("max" + str((ga_mean / min_df)['throughput'].max()))
    ax1.plot(ga_mean, label=r'$q$-MAX,$\gamma=$' + str(l), markersize=10)

handles, labels = ax1.get_legend_handles_labels()
ax1.legend(handles, labels, loc=1, prop={'size': 16})
#ax1.legend(handles, labels, loc=3, prop={'size': 16})
fig.tight_layout()
plt.tick_params(axis='both', which='major', labelsize=16)
#ax1.set_yscale('log', basey=2)
#plt.yscale('log')
#plt.show()
plt.savefig('graph4.png', bbox_inches='tight')

