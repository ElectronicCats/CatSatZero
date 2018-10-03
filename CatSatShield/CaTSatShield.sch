EESchema Schematic File Version 4
LIBS:CaTSatShield-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "CaTSatZero"
Date "2018-10-01"
Rev "v0.3"
Comp "Electronic Cats"
Comment1 "Andres Sabas"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Notes Line
	1980 1105 2380 1105
Wire Notes Line
	2380 1305 1980 1305
Wire Notes Line
	1770 1405 1770 1605
Wire Notes Line
	1580 1605 1580 1405
Wire Notes Line
	1680 3405 1680 1605
Wire Notes Line
	2380 1205 2680 1205
Wire Notes Line
	1680 1205 1980 1205
Wire Notes Line
	1680 1405 1680 1205
Wire Notes Line
	2780 1355 2780 1655
Wire Notes Line
	2580 1655 2580 1355
Wire Notes Line
	2580 1355 2780 1355
Wire Notes Line
	2680 1355 2680 1205
Wire Notes Line
	2680 3405 2680 1655
Wire Notes Line
	1680 3405 2680 3405
Wire Notes Line
	1980 1105 1980 1305
Wire Notes Line
	2380 1105 2380 1305
Wire Notes Line
	1580 1405 1770 1405
Wire Notes Line
	1580 1605 1770 1605
Wire Notes Line
	2580 1655 2780 1655
$Comp
L power:+5V #PWR?
U 1 1 5B6115B7
P 2830 1805
AR Path="/5B60DF55/5B6115B7" Ref="#PWR?"  Part="1" 
AR Path="/5B6115B7" Ref="#PWR02"  Part="1" 
F 0 "#PWR02" H 2830 1655 50  0001 C CNN
F 1 "+5V" V 2845 1933 50  0000 L CNN
F 2 "" H 2830 1805 50  0001 C CNN
F 3 "" H 2830 1805 50  0001 C CNN
	1    2830 1805
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5B6115BD
P 2830 2005
AR Path="/5B60DF55/5B6115BD" Ref="#PWR?"  Part="1" 
AR Path="/5B6115BD" Ref="#PWR04"  Part="1" 
F 0 "#PWR04" H 2830 1855 50  0001 C CNN
F 1 "+3.3V" V 2845 2133 50  0000 L CNN
F 2 "" H 2830 2005 50  0001 C CNN
F 3 "" H 2830 2005 50  0001 C CNN
	1    2830 2005
	0    1    1    0   
$EndComp
Text Label 2830 2205 0    50   ~ 0
RESET
Text Label 2830 2305 0    50   ~ 0
TX
Text Label 2830 2405 0    50   ~ 0
RX
Text Label 2830 2505 0    50   ~ 0
SCL
Text Label 2830 2605 0    50   ~ 0
SDA
Text Label 2830 2705 0    50   ~ 0
10-INTCCS
Text Label 2830 2805 0    50   ~ 0
9SCK
Text Label 2830 2905 0    50   ~ 0
8MOSI
Text Label 2830 3005 0    50   ~ 0
7
Text Label 2830 3105 0    50   ~ 0
6
Text Label 1530 1815 2    50   ~ 0
AREF
Text Label 1530 1915 2    50   ~ 0
DAC0/A0
Text Label 1530 2015 2    50   ~ 0
A1
Text Label 1530 2415 2    50   ~ 0
A5
Text Label 1530 2515 2    50   ~ 0
WAKEA6
Text Label 1530 2615 2    50   ~ 0
0
Text Label 1530 2715 2    50   ~ 0
1
Text Label 1530 2815 2    50   ~ 0
~~2
Text Label 1530 2915 2    50   ~ 0
~~3
Text Label 1530 3015 2    50   ~ 0
~~4
Text Label 1530 3115 2    50   ~ 0
~~5
$Comp
L Connector_Generic:Conn_01x14 J?
U 1 1 5B6115DB
P 1730 2415
AR Path="/5B60DF55/5B6115DB" Ref="J?"  Part="1" 
AR Path="/5B6115DB" Ref="J1"  Part="1" 
F 0 "J1" H 1690 1605 50  0000 L CNN
F 1 " " H 1670 1515 50  0000 L CNN
F 2 "Pines:61301411121" H 1730 2415 50  0001 C CNN
F 3 "~" H 1730 2415 50  0001 C CNN
	1    1730 2415
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x14 J?
U 1 1 5B6115E2
P 2630 2505
AR Path="/5B60DF55/5B6115E2" Ref="J?"  Part="1" 
AR Path="/5B6115E2" Ref="J2"  Part="1" 
F 0 "J2" H 2635 3215 50  0000 C CNN
F 1 " " H 2640 3295 50  0000 C CNN
F 2 "Pines:61301411121" H 2630 2505 50  0001 C CNN
F 3 "~" H 2630 2505 50  0001 C CNN
	1    2630 2505
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5B6115E9
P 2830 2105
AR Path="/5B60DF55/5B6115E9" Ref="#PWR?"  Part="1" 
AR Path="/5B6115E9" Ref="#PWR05"  Part="1" 
F 0 "#PWR05" H 2830 1855 50  0001 C CNN
F 1 "GND" V 2835 1977 50  0000 R CNN
F 2 "" H 2830 2105 50  0001 C CNN
F 3 "" H 2830 2105 50  0001 C CNN
	1    2830 2105
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 5B6115EF
P 2830 1905
AR Path="/5B60DF55/5B6115EF" Ref="#PWR?"  Part="1" 
AR Path="/5B6115EF" Ref="#PWR03"  Part="1" 
F 0 "#PWR03" H 2830 1755 50  0001 C CNN
F 1 "VCC" V 2847 2033 50  0000 L CNN
F 2 "" H 2830 1905 50  0001 C CNN
F 3 "" H 2830 1905 50  0001 C CNN
	1    2830 1905
	0    1    1    0   
$EndComp
$Comp
L electroniccats:L80-M39 MODULE1
U 1 1 5B61EB84
P 5240 2460
F 0 "MODULE1" H 5240 3327 50  0000 C CNN
F 1 "L80-M39" H 5240 3236 50  0000 C CNN
F 2 "Rf:QUECTEL_L80-M39" H 5240 2460 50  0001 L CNN
F 3 "9.01 USD" H 5240 2460 50  0001 L CNN
F 4 "L80-M39" H 5240 2460 50  0001 L CNN "MP"
F 5 "Warning" H 5240 2460 50  0001 L CNN "Availability"
F 6 "GPS Receiver 1575.42MHz 3.3V 12-Pin" H 5240 2460 50  0001 L CNN "Description"
F 7 "None" H 5240 2460 50  0001 L CNN "Package"
F 8 "Quectel" H 5240 2460 50  0001 L CNN "MF"
	1    5240 2460
	1    0    0    -1  
$EndComp
Wire Wire Line
	5840 2360 5985 2360
Wire Wire Line
	5840 1960 5915 1960
Wire Wire Line
	5915 1960 5915 1860
Wire Wire Line
	5840 1860 5915 1860
Connection ~ 5915 1860
Wire Wire Line
	5915 1860 5915 1780
Wire Wire Line
	4640 2360 4485 2360
Wire Wire Line
	5840 3060 5910 3060
Wire Wire Line
	5910 3060 5910 3125
$Comp
L power:GND #PWR06
U 1 1 5B61F79D
P 5910 3125
F 0 "#PWR06" H 5910 2875 50  0001 C CNN
F 1 "GND" H 5915 2952 50  0000 C CNN
F 2 "" H 5910 3125 50  0001 C CNN
F 3 "" H 5910 3125 50  0001 C CNN
	1    5910 3125
	1    0    0    -1  
$EndComp
Text Label 5985 2360 0    50   ~ 0
RX
Text Label 4485 2360 2    50   ~ 0
TX
NoConn ~ 4640 2760
NoConn ~ 5840 2460
NoConn ~ 5840 2560
$Comp
L power:+3.3V #PWR01
U 1 1 5B61FF92
P 5915 1780
F 0 "#PWR01" H 5915 1630 50  0001 C CNN
F 1 "+3.3V" H 5930 1953 50  0000 C CNN
F 2 "" H 5915 1780 50  0001 C CNN
F 3 "" H 5915 1780 50  0001 C CNN
	1    5915 1780
	1    0    0    -1  
$EndComp
NoConn ~ 4640 2260
$Comp
L Connector_Generic:Conn_01x08 J4
U 1 1 5B67C5D5
P 8435 2240
F 0 "J4" H 8514 2232 50  0000 L CNN
F 1 "CCS811" H 8514 2141 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x07_Pitch2.54mm" H 8435 2240 50  0001 C CNN
F 3 "~" H 8435 2240 50  0001 C CNN
	1    8435 2240
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J3
U 1 1 5B67CECC
P 10360 2195
F 0 "J3" H 10440 2187 50  0000 L CNN
F 1 "BME280" H 10440 2096 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 10360 2195 50  0001 C CNN
F 3 "~" H 10360 2195 50  0001 C CNN
	1    10360 2195
	1    0    0    -1  
$EndComp
Text Label 9955 2395 0    50   ~ 0
SDA
Text Label 9960 2295 0    50   ~ 0
SCL
Wire Wire Line
	8235 1940 8010 1940
Wire Wire Line
	8235 2140 8015 2140
Wire Wire Line
	8235 2240 8015 2240
Wire Wire Line
	8235 2340 8020 2340
Wire Wire Line
	8235 2440 8015 2440
Wire Wire Line
	8235 2540 8015 2540
Text Label 8015 2540 0    50   ~ 0
RESET
Text Label 8015 2440 2    50   ~ 0
10-INTCCS
Text Label 8020 2340 2    50   ~ 0
WAKEA6
Text Label 8015 2240 0    50   ~ 0
SDA
Text Label 8015 2140 0    50   ~ 0
SCL
$Comp
L power:GND #PWR0101
U 1 1 5B6B7DE3
P 7915 2040
F 0 "#PWR0101" H 7915 1790 50  0001 C CNN
F 1 "GND" H 7920 1867 50  0000 C CNN
F 2 "" H 7915 2040 50  0001 C CNN
F 3 "" H 7915 2040 50  0001 C CNN
	1    7915 2040
	1    0    0    -1  
$EndComp
Wire Wire Line
	7915 2040 8235 2040
Wire Wire Line
	10160 2295 9960 2295
Wire Wire Line
	10160 2395 9955 2395
Wire Wire Line
	10160 2095 9950 2095
$Comp
L power:GND #PWR0104
U 1 1 5B6B9425
P 9855 2200
F 0 "#PWR0104" H 9855 1950 50  0001 C CNN
F 1 "GND" H 9860 2027 50  0000 C CNN
F 2 "" H 9855 2200 50  0001 C CNN
F 3 "" H 9855 2200 50  0001 C CNN
	1    9855 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9855 2195 9855 2200
Wire Wire Line
	9855 2195 10160 2195
Wire Notes Line
	3570 490  3570 7760
Wire Notes Line
	3570 7760 3580 7760
Wire Notes Line
	6970 530  6970 6520
Wire Notes Line
	9305 515  9305 6535
Text Notes 1715 700  0    50   ~ 0
MKR1300
Text Notes 5140 700  0    50   ~ 0
GPS
Text Notes 10365 660  2    50   ~ 0
BME280
Text Notes 9465 865  0    50   ~ 0
TEMPERATURE, HUMIDITY AND PRESSION
Text Notes 8010 660  0    50   ~ 0
CCS811
Text Notes 7680 835  0    50   ~ 0
Volatile Organic Compounds
Text Notes 1520 3765 0    50   ~ 0
NOTE: \nD32 Arduino is Level Battery
NoConn ~ 8235 2640
Text Label 1530 2315 2    50   ~ 0
A4
Text Label 1530 2215 2    50   ~ 0
A3
Text Label 1530 2115 2    50   ~ 0
A2
NoConn ~ 4640 2460
$Comp
L power:+3.3V #PWR0102
U 1 1 5BB2D67F
P 8010 1940
F 0 "#PWR0102" H 8010 1790 50  0001 C CNN
F 1 "+3.3V" H 8025 2113 50  0000 C CNN
F 2 "" H 8010 1940 50  0001 C CNN
F 3 "" H 8010 1940 50  0001 C CNN
	1    8010 1940
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0103
U 1 1 5BB2D707
P 9950 2095
F 0 "#PWR0103" H 9950 1945 50  0001 C CNN
F 1 "+3.3V" H 9965 2268 50  0000 C CNN
F 2 "" H 9950 2095 50  0001 C CNN
F 3 "" H 9950 2095 50  0001 C CNN
	1    9950 2095
	1    0    0    -1  
$EndComp
$EndSCHEMATC
