EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "ECE3720 Final Project - Space Time"
Date "2020-03-09"
Rev "1"
Comp ""
Comment1 "Matthew James Bellafaire "
Comment2 "Nicholas Musienko"
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 600  7700 0    50   ~ 0
Note: the component "From-Dragon-12" is not a full description of all pins available on the dragon-12 board\n
$Comp
L CustomParts:MTK3339-GPS-Module A2
U 1 1 5E998316
P 6550 3700
F 0 "A2" H 6900 4000 50  0000 C CNN
F 1 "MTK3339-GPS-Module" H 7300 4100 50  0000 C CNN
F 2 "" H 5900 4450 50  0001 C CNN
F 3 "" H 5900 4450 50  0001 C CNN
	1    6550 3700
	1    0    0    -1  
$EndComp
$Comp
L CustomParts:Adafruit-1.2"-7-Seg A3
U 1 1 5E99997D
P 7150 2300
F 0 "A3" H 6150 2950 50  0000 L CNN
F 1 "Adafruit-1.2\"-7-Seg" H 6150 3050 50  0000 L CNN
F 2 "" H 6250 3100 50  0001 C CNN
F 3 "" H 6250 3100 50  0001 C CNN
	1    7150 2300
	1    0    0    -1  
$EndComp
$Comp
L CustomParts:Dragon-12-Board A1
U 1 1 5E99B2FC
P 5000 5050
F 0 "A1" H 4750 8300 50  0000 R CNN
F 1 "Dragon-12-Board" H 5350 8400 50  0000 R CNN
F 2 "" H 4600 8000 50  0001 C CNN
F 3 "" H 4600 8000 50  0001 C CNN
	1    5000 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 3150 5650 3150
Wire Wire Line
	5650 3850 6200 3850
Wire Wire Line
	5450 3050 5750 3050
Wire Wire Line
	5750 3750 6200 3750
NoConn ~ 6200 3550
NoConn ~ 5150 2150
$Comp
L power:GND #PWR02
U 1 1 5E99D9CE
P 5000 3550
F 0 "#PWR02" H 5000 3300 50  0001 C CNN
F 1 "GND" H 5005 3377 50  0000 C CNN
F 2 "" H 5000 3550 50  0001 C CNN
F 3 "" H 5000 3550 50  0001 C CNN
	1    5000 3550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR01
U 1 1 5E99DDD8
P 4850 2150
F 0 "#PWR01" H 4850 2000 50  0001 C CNN
F 1 "+5V" H 4865 2323 50  0000 C CNN
F 2 "" H 4850 2150 50  0001 C CNN
F 3 "" H 4850 2150 50  0001 C CNN
	1    4850 2150
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR06
U 1 1 5E99E874
P 7150 1650
F 0 "#PWR06" H 7150 1500 50  0001 C CNN
F 1 "+5V" H 7165 1823 50  0000 C CNN
F 2 "" H 7150 1650 50  0001 C CNN
F 3 "" H 7150 1650 50  0001 C CNN
	1    7150 1650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR05
U 1 1 5E99EC7F
P 6600 3150
F 0 "#PWR05" H 6600 3000 50  0001 C CNN
F 1 "+5V" H 6615 3323 50  0000 C CNN
F 2 "" H 6600 3150 50  0001 C CNN
F 3 "" H 6600 3150 50  0001 C CNN
	1    6600 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 3050 5750 3750
Wire Wire Line
	5650 3150 5650 3850
NoConn ~ 6900 3600
NoConn ~ 6900 3700
NoConn ~ 6900 3800
NoConn ~ 6500 3150
$Comp
L power:GND #PWR07
U 1 1 5E9A080F
P 7150 2850
F 0 "#PWR07" H 7150 2600 50  0001 C CNN
F 1 "GND" H 7155 2677 50  0000 C CNN
F 2 "" H 7150 2850 50  0001 C CNN
F 3 "" H 7150 2850 50  0001 C CNN
	1    7150 2850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5E9A13F7
P 6550 4200
F 0 "#PWR04" H 6550 3950 50  0001 C CNN
F 1 "GND" H 6555 4027 50  0000 C CNN
F 2 "" H 6550 4200 50  0001 C CNN
F 3 "" H 6550 4200 50  0001 C CNN
	1    6550 4200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR03
U 1 1 5E9A36FA
P 5950 2350
F 0 "#PWR03" H 5950 2200 50  0001 C CNN
F 1 "+5V" H 5965 2523 50  0000 C CNN
F 2 "" H 5950 2350 50  0001 C CNN
F 3 "" H 5950 2350 50  0001 C CNN
	1    5950 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 2600 6050 2600
Wire Wire Line
	5450 2700 6050 2700
Wire Wire Line
	5950 2500 6050 2500
Wire Wire Line
	5950 2350 5950 2500
$EndSCHEMATC
