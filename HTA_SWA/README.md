# Hiking Tour Assistant Smartwatch Application

## Description

HTA SWA is a firmware application for TTGO smartwatch.

## TTGO_FW Diagram

Note: This is not a class UML Diagram. More like a software layer diagram. Class Diagram is TBD.

```mermaid
classDiagram
   class ESP32_V3_WIFI_BL
   << HW >> ESP32_V3_WIFI_BL
   class L76K_GPS
   << HW >> L76K_GPS
   class SideButton
   << HW >> SideButton
   class BMA423_ACC_ODO
   << HW >> BMA423_ACC_ODO
   class AXP202_PMU
   << HW >> AXP202_PMU
   class TFT_SCREEN
   << HW >> TFT_SCREEN
   class FT6336_TOUCHSCREEN
   << HW >> FT6336_TOUCHSCREEN
   class PCF853_RTC
   << HW >> PCF853_RTC

   class TFT_eSPI
   << Driver >> TFT_eSPI
   class buttonIRQ
   << Driver >> buttonIRQ
   class bma423
   << Driver >> bma423
   class rtc
   << Driver >> rtc
   class TinyGPS
   << Driver >> TinyGPS
   class axp20x
   << Driver >> axp20x
   class BluetoothSerial
   << Driver >> BluetoothSerial

   TFT_eSPI -- FT6336_TOUCHSCREEN
   TFT_eSPI -- TFT_SCREEN
   buttonIRQ -- SideButton
   bma423 -- BMA423_ACC_ODO
   rtc -- PCF853_RTC
   TinyGPS -- L76K_GPS
   axp20x -- AXP202_PMU
   BluetoothSerial -- ESP32_V3_WIFI_BL

   class TTGO
   << Object >> TTGO


   TTGO -- TFT_eSPI
   TTGO -- bma423
   TTGO -- TinyGPS
   TTGO -- rtc
   TTGO -- axp20x

   class HTA 
   << Application >> HTA

   HTA -- TTGO
   HTA -- buttonIRQ
   HTA -- BluetoothSerial
```