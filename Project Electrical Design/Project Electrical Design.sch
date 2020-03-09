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
$Comp
L CustomParts:MTK3339-GPS-Module A?
U 1 1 5E668337
P 4300 2700
F 0 "A?" H 3950 3250 50  0000 C CNN
F 1 "MTK3339-GPS-Module" H 3550 3350 50  0000 C CNN
F 2 "" H 3650 3450 50  0001 C CNN
F 3 "" H 3650 3450 50  0001 C CNN
	1    4300 2700
	1    0    0    -1  
$EndComp
$Comp
L Adafruit-1.2-7-seg:Adafruit-1.2"-7-Seg A?
U 1 1 5E66F4C1
P 7750 2700
F 0 "A?" H 8780 2796 50  0000 L CNN
F 1 "Adafruit-1.2\"-7-Seg" H 8780 2705 50  0000 L CNN
F 2 "" H 6850 3500 50  0001 C CNN
F 3 "" H 6850 3500 50  0001 C CNN
	1    7750 2700
	1    0    0    -1  
$EndComp
$Comp
L CustomParts:Adafruit_8x8_Matrix A?
U 1 1 5E66D79F
P 6750 4500
F 0 "A?" H 8178 4521 50  0000 L CNN
F 1 "Adafruit_8x8_Matrix" H 8178 4430 50  0000 L CNN
F 2 "" H 7650 5200 50  0001 C CNN
F 3 "" H 7650 5200 50  0001 C CNN
	1    6750 4500
	1    0    0    -1  
$EndComp
$Comp
L CustomParts:From-Dragon-12-Board A?
U 1 1 5E677287
P 1000 3850
F 0 "A?" H 1300 6800 50  0000 R CNN
F 1 "From-Dragon-12-Board" H 2150 6700 50  0000 R CNN
F 2 "" H 600 6850 50  0001 C CNN
F 3 "" H 600 6850 50  0001 C CNN
	1    1000 3850
	1    0    0    -1  
$EndComp
Text Notes 550  7750 0    50   ~ 0
Note: the component "From-Dragon-12" is not a full description of all pins available on the dragon-12 board\n
$EndSCHEMATC
