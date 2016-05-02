<Qucs Schematic 0.0.19>
<Properties>
  <View=-115,-69,952,550,0.923977,55,0>
  <Grid=10,10,1>
  <DataSet=filter6-iter3b.dat>
  <DataDisplay=filter6-iter3b.dpl>
  <OpenDisplay=1>
  <Script=filter6-iter3b.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Название>
  <FrameText1=Чертил:>
  <FrameText2=Дата:>
  <FrameText3=Версия:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <Lib T1 1 260 250 8 -26 0 0 "NMOSFETs" 0 "IRL530N" 0>
  <VProbe V_MOSFET 1 330 250 -16 28 0 3>
  <R R2 1 170 300 15 -26 0 1 "2kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <VProbe PrIn 1 -40 300 -31 -44 0 1>
  <IProbe I_zero 1 670 400 -26 -32 1 0>
  <C C5 1 440 290 17 -26 0 1 "47uF" 1 "" 0 "neutral" 0>
  <Vfile V2 1 10 130 18 -26 0 1 "C:/msys64/home/Ilya/Projects/pwmfilter/sine.csv" 1 "linear" 0 "yes" 0 "1" 0 "0" 0>
  <.TR TR1 1 170 -40 0 55 0 0 "lin" 1 "0" 1 "20 ms" 1 "500001" 0 "Gear" 0 "3" 0 "0.2 ns" 0 "1e-16" 0 "150" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "26.85" 0 "1e-3" 0 "1e-6" 0 "1" 0 "CroutLU" 0 "no" 0 "yes" 0 "0" 0>
  <C C1 1 80 310 17 -26 0 1 "4.7 uF" 1 "" 0 "neutral" 0>
  <L L2 1 440 140 10 -26 0 1 "1 mH" 1 "" 0>
  <L L1 1 490 200 -26 10 0 0 "1 mH" 1 "" 0>
  <IProbe I_out 1 580 200 -26 16 1 2>
  <R R_out 1 650 200 -26 15 0 0 "4 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R5 1 260 350 -68 -26 0 3 "1 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <VProbe V_out 1 620 130 -62 -31 1 2>
  <GND * 1 780 400 0 0 0 0>
  <Vdc V3 1 740 90 -26 -48 0 2 "3V" 1>
</Components>
<Wires>
  <80 360 170 360 "" 0 0 0 "">
  <80 250 170 250 "" 0 0 0 "">
  <80 250 80 280 "" 0 0 0 "">
  <80 340 80 360 "" 0 0 0 "">
  <170 330 170 360 "" 0 0 0 "">
  <170 250 170 270 "" 0 0 0 "">
  <170 360 170 400 "" 0 0 0 "">
  <170 400 260 400 "" 0 0 0 "">
  <170 250 230 250 "" 0 0 0 "">
  <310 260 310 280 "" 0 0 0 "">
  <260 280 310 280 "" 0 0 0 "">
  <310 220 310 240 "" 0 0 0 "">
  <260 220 310 220 "" 0 0 0 "">
  <80 70 80 250 "" 0 0 0 "">
  <-20 310 -20 360 "" 0 0 0 "">
  <-20 70 -20 290 "" 0 0 0 "">
  <-20 360 10 360 "" 0 0 0 "">
  <10 360 80 360 "" 0 0 0 "">
  <10 160 10 360 "" 0 0 0 "">
  <-20 70 10 70 "" 0 0 0 "">
  <10 70 80 70 "" 0 0 0 "">
  <10 70 10 100 "" 0 0 0 "">
  <260 400 440 400 "" 0 0 0 "">
  <440 400 640 400 "" 0 0 0 "">
  <260 280 260 320 "" 0 0 0 "">
  <260 380 260 400 "" 0 0 0 "">
  <440 320 440 400 "" 0 0 0 "">
  <440 90 440 110 "" 0 0 0 "">
  <700 90 700 150 "" 0 0 0 "">
  <440 170 440 200 "" 0 0 0 "">
  <440 90 700 90 "" 0 0 0 "">
  <260 200 260 220 "" 0 0 0 "">
  <440 200 440 260 "" 0 0 0 "">
  <260 200 440 200 "" 0 0 0 "">
  <440 200 460 200 "" 0 0 0 "">
  <680 200 700 200 "" 0 0 0 "">
  <610 200 620 200 "" 0 0 0 "">
  <520 200 540 200 "" 0 0 0 "">
  <700 150 700 200 "" 0 0 0 "">
  <630 150 700 150 "" 0 0 0 "">
  <540 200 550 200 "" 0 0 0 "">
  <540 150 540 200 "" 0 0 0 "">
  <540 150 610 150 "" 0 0 0 "">
  <700 90 710 90 "" 0 0 0 "">
  <700 400 780 400 "" 0 0 0 "">
  <770 90 780 90 "" 0 0 0 "">
  <780 90 780 400 "" 0 0 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>