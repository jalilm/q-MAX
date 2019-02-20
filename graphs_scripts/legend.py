import pylab

data = []

figData = pylab.figure()
ax = pylab.gca()
for algName in algOrder: #Change here to plotting your data
	spaceAxis = sorted(data[measurement][logNforSpaceFig][algName][threshold])
	recallVals = [100*float(data[measurement][logNforSpaceFig][algName][threshold][space].split('(')[1].split(',')[0]) for space in spaceAxis]
	pylab.plot(spaceAxis, recallVals, label=renamer.get(algName,algName), linestyle=lineStyles[algName], marker=markers[algName], markersize=markersizes[algName], color=colors[algName]) 
figLegend = pylab.figure()
pylab.figlegend(*ax.get_legend_handles_labels(), fontsize=20, loc = 'upper left', ncol=10) 
figLegend.savefig("legend.pdf", bbox_inches='tight') 
figLegend.savefig("legend.png", bbox_inches='tight') 
pylab.clf()

