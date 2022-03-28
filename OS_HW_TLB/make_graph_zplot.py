# import the library
from zplot import *
import sys

# describe the drawing surface
ctype = 'pdf' if len(sys.argv) < 2 else sys.argv[1]
c = canvas(ctype, title='TLB_Measurement_Graph', dimensions=['3in', '2.4in'])

# load some data
t = table(file='time.data')

# make a drawable region for a graph
d = drawable(canvas=c, xrange=[0,8192], yrange=[0,35],
             coord=['0.5in','0.4in'], dimensions=['2.3in','1.7in'])

# make some axes
axis(drawable=d, title='TLB Size Measurement', xtitle='Number Of Pages',
     ytitle='Time Per Access (ns)')

# plot the points
p = plotter()
p.points(drawable=d, table=t, xfield='x', yfield='y', style='circle',
         linecolor='red', fill=True, fillcolor='red')
p.line(drawable=d, table=t, xfield='x', yfield='y', linecolor='red', linejoin=1)

# finally, output the graph to a file
c.render()
