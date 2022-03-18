## Huawei Nexus 6P

### How to root the phone
1. Unlock *Developer options* in phone settings and enable *USB debugging*.

2. Unlock bootloader with the following commands:
    - *adb reboot bootloader* (it is also possible to put the phone in boot mode by holding power button and volume down button)
    - *fastboot flashing unlock*
    - *fastboot reboot*.
    
3. Download SuperSU ([SuperSu download page](https://download.chainfire.eu/1220/SuperSU/SR5-SuperSU-v2.82-SR5-20171001224502.zip)) and push the downloaded file into the phone with the command *adb push SuperSu-file.zip /sdcard/*
    
4. Download TWRP recovery ([TWRP download page](https://dl.twrp.me/angler/)) (the latest version works with android 7 or later)
   and then execute:
    - *adb reboot bootloader* (or hold the buttons as explained above)
    - *fastboot flash recovery twrp-file.img*.
   
   Then use the volume keys to scroll and power key to select the *Reboot Bootloader* option. Once the phone has booted back into
   the bootloader you can use the volume keys to scroll and the power key to boot into your newly flashed recovery by selecting 
   *Recovery mode* option.
 
5. Once in TWRP recovery menu select the *Install* menu. Navigate to where the SuperSu zip is stored (it should be already in the
    sdcard directory) and select it.
    
    Swipe to install.
    
    Once the zip has installed you'll have an option to *wipe cache/dalvik* and an option to *Reboot system*: 
    hit *Reboot system* button. 

    Do not flash the version of SuperSU that TWRP offers to flash for you in order to give you root, it is not compatible and 
    will cause problems. When you attempt to reboot out of TWRP it will tell you that it's detecting that you don't have root 
    and it will offer to root for you, skip past this. You do have root, ignore this every time it comes up.
    
    At the end of the installation it will be asked to install TWRP application, it is not necessary.
    
SuperSu whould be installed on the device and it should be possible to execute *su* command in adb shell.


### AT Commands set up
To configure the phone in order to use AT Commands the following steps are required:

1. Unlock *Developer options* in phone settings and enable *USB debugging*.

2. Connect the phone to the computer and run the following commands (it can be done both on Linux and Windows):
    - *adb reboot fastboot*
    - *fastboot oem enable-bp-tools*
    - *fastboot reboot*
    
    This will set the property *sys.usb.config* to *diag,serial_smd,rmnet_ipa,adb*.
    
3. Connect the phone to a Windows computer (tried on Windows 7 only) with *Putty* installed ([Putty download page](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html)).

4. On *Control Panel > Hardware and Sound > Device and Printers*, double click *Nexus 6p* icon. The properties window should open.

5. On the Hardware panel of the property window the following devices should be listed:
    - Android Composite ADB Interface
    - Qualcomm HS-USB Diagnostics 
    - Qualcomm HS-USB Modem
    - Qualcomm Wireless HS-USB Ethernet Adapter
    - USB Composite Devide
    
    Qualcomm Modem device is the one we need to use AT Commands. On the device's properties, Modem panel we can read which 
    *Port* is used (e.g. COM3) and the *Maximum Port Speed*.
    
    If Qualcomm Modem is not listed and only *Nexus 6P* is listed, one problem could be Windows drivers. In this case it is necessary to 
    unistall the drivers for all the *Nexus 6P* devices listed (more then one could be listed). The drivers can be unistalled by 
    clicking on the specific device > properties > Driver > Unistall. Once the previous step is completed, reconnect the phone to 
    the computer and wait for the drivers to be automtically installed.
    
6. Open Putty, select *Serial* among *Connection type*, insert the right port name in *Serial line* and speed value in *Speed* and 
    then click *Open*. Putty window should open and AT Commands should work. 
    
    (It is possible that the typed text is not shown, but the connection works.  Try to type *AT* and press *Enter* to check if 
    any reply is received).
