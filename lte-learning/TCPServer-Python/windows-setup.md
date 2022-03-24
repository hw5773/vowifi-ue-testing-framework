### Windows setup

Steps to setup Windows environment for ATFuzzer

#### Python
- download python 2.7.15 from https://www.python.org/downloads/release/python-2715/
- execute the python<version>.msi file
- add C:\Python27 (installation directory path) to the path env variable 
- download pip from https://bootstrap.pypa.io/get-pip.py and run "python get-pip.py"
- add C:\Python27\Scripts to the path env variable
- make sure to install the following modules with "pip install <module_name>":
  - serial
  - pyserial
  - pyreadline

#### ADB and Fastboot
- download the zip file from https://dl.google.com/android/repository/platform-tools-latest-windows.zip
- add adb path to the path env variable
