#!/usr/bin/python
import pandas as pd
import matplotlib.pyplot as plt
from cycler import cycler
from math import log, ceil

multichrome = cycler(color=['r','c','m','y','k','burlywood']) + cycler(marker=['v','d','|','*','8','^']) + cycler(linestyle=['-.', ':', '-' ,'--', '-.', '-'])

fig, ax1 = plt.subplots(1,1)
ax1.set_prop_cycle(multichrome)
ax1.grid()
ax1.set_ylabel('CPU Throughput [MPPS]', fontsize=20)
ax1.set_xlabel(r'Slack parameter ($\tau$)', fontsize=20)

df_org = pd.read_csv('../results/stiming_random.raw_res', skipinitialspace=True)
df_org = df_org[df_org['insertions'] == 150000000]
df_org['throughput'] = df_org['insertions'] / 1000000 / df_org['duration']
del df_org['duration']
del df_org['dataset']
del df_org['type']
del df_org['size']
del df_org['insertions']

for g in [0.1, 0.05, 0.01]:
        g_df = df_org[(df_org['gamma'] == g)]
        del g_df['gamma']
	print(g_df)
	for w in [50000000, 100000000]:
		w_df = g_df[(g_df['W'] == w)]
	       	del w_df['W']
        	gq = w_df.groupby(['tau'])
		w_label = r'W=$' + (r'10^{8}$' if w == 100000000 else r'5\cdot10^{7}$')
        	ax1.plot(gq.mean(), label=w_label+r',$\gamma=$'+str(g), markersize=10)

ax1.set_ylim(ymin=0)
handles, labels = ax1.get_legend_handles_labels()
ax1.legend(handles, labels, prop={'size': 14}, loc='lower center', framealpha=1.0, bbox_to_anchor=(0.5,1), ncol=2)
fig.tight_layout()
plt.tick_params(axis='both', which='major', labelsize=16)
#plt.show()
plt.savefig('../figures/graph8.pdf', bbox_inches='tight')
