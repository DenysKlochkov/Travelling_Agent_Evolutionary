set   autoscale  
set   terminal pngcairo size 1000,600 
set   output 'optimalization.png'
	  
	  set style line 1 \
      linecolor rgb '#0060ad' \
      linetype 2 linewidth 0.5 \
      pointtype 1 pointsize 1

      unset log                            
      unset label                           
      set xtic auto                        
      set ytic auto
      set yrange [0:7000]
      set xlabel "# Generacji"
      set ylabel "Długość cyklu"
      set title "Przebieg optymalizacji"
      plot 'out.txt' using 1:2 with linespoints linestyle 1