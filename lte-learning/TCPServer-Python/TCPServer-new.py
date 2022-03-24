#!/usr/bin/python
# import socket programming library
import socket
import os
import time
import sys
import serial
from stat import *
import logging
import subprocess
# import thread module
from thread import *
import threading
from atsend import *
global device
global environment
print_lock = threading.Lock()
'''
huwaeiy5: subprocess.call("adb shell input keyevent KEYCODE_WAKEUP & adb shell am start -a android.settings.AIRPLANE_MODE_SETTINGS & adb shell input tap 300 1100",shell=True)
huwaeihonor: subprocess.call("adb shell input keyevent KEYCODE_WAKEUP & adb shell am start -a android.settings.AIRPLANE_MODE_SETTINGS & adb shell input tap 300 300",shell=True)
'''
# GLOBAL VARIABLES
WIN_RUNTIME = ['cmd.exe', '/C']
OS_LINUX_RUNTIME = ['/bin/bash', '-l', '-c']

TASKLIT = 'tasklist'
KILL = ['taskkill', '/F', '/IM']

isWindows = True
environment = ''
DEFAULT_BAUD=115200
DEFAULT_TIMEOUT=1

##################################################################################################
def set_environment():
    global environment, isWindows
    if os.name == 'posix':
        environment = 'linux'
        isWindows = False
    elif os.name == 'nt':
        environment = 'windows'
        isWindows = True
    else:
        raise Exception('EnvironmentError: unknow OS')


# Check if a process is running
def isProcessRunning(serviceName):
    command = ''
    if isWindows:
        command = TASKLIT
    else:
        command = "pidof " + serviceName
        command = OS_LINUX_RUNTIME + [command]

    result = subprocess.check_output(command)
    return serviceName in result


# Used for killing (ADB) process
def killProess(serviceName):
    command = ''
    if isWindows and environment == 'windows':
        command = KILL + [serviceName]
        print command

    elif isWindows == False and environment == 'linux':
        command = "ps -ef | grep " + serviceName + " | grep -v grep | awk '{print $2}' | xargs sudo kill -9"
        command = OS_LINUX_RUNTIME + [command]
    else:
        raise Exception('EnvironmentError: unknow OS')
    subprocess.check_output(command)
    return


def run_adb_command(command):

    result = str(run_command(command))

    while True:
        if "error" in result.lower():
            print 'reboot adb server'
            if isWindows and environment == 'windows':
                processName = "adb.exe"
            elif isWindows == False and environment == 'linux':
                processName = "adb"
            else:
                raise Exception('Cannot find the OS')

            if isProcessRunning(processName):
                print '### Killing the ADB process ###'
                killProess(processName)

            time.sleep(1)

            adb_usb_command = 'adb usb'
            print '### Trying to fix adb error with adb usb'
            run_command(adb_usb_command)

            print 'ADB server stopping'
            run_command('adb kill-server')
            print 'ADB Server killed'

            print 'ADB server starting'
            run_command('adb start-server')
            print 'ADB Server started'
            print 'ADB Server restart done'

        else:
            break

    return result

def restart_adb_server():
    result = ''
    print 'Killing Server'
    result = run_adb_command('adb kill-server')
    time.sleep(1)
    print 'Starting server'
    result = run_adb_command('adb start-server')
    print 'Server Restart done.'
    return True


def airplane_mode_on():
    result = 1
    print 'airplane mode on\n'
    time.sleep(1)
    #result = run_adb_command("adb shell settings put global airplane_mode_on 1")
    #result = run_adb_command("adb shell \"su -c 'am broadcast -a android.intent.action.AIRPLANE_MODE'\"")
    if device == "usb":
        #subprocess.call("sudo sh -c \"echo 0 > /sys/bus/usb/devices/1-2.4/authorized\"",shell=True)
        #subprocess.call("sudo sh -c \"echo '1-1.4' |sudo tee /sys/bus/usb/drivers/usb/unbind\"",shell=True)
        subprocess.call("sudo sh -c \"echo disabled | sudo tee /sys/bus/usb/devices/usb1/power/wakeup\"",shell=True)
    elif device == "galaxys3" or device== "huwaeip8":
        cmd = ['adb', 'shell', 'su']
        procId = subprocess.Popen(cmd, stdin=subprocess.PIPE)
        procId.communicate('settings put global airplane_mode_on 1\nam broadcast -a android.intent.action.AIRPLANE_MODE --ez state true\nexit\n')
        #procId.communicate('settings put global airplane_mode_on 1\nexit\n')
        #procId.communicate('am broadcast -a android.intent.action.AIRPLANE_MODE --ez state true\nexit\n')
        #result = run_adb_command("adb shell settings put global airplane_mode_on 1")
        #time.sleep(1)
        #result = run_adb_command("adb shell am broadcast -a android.intent.action.AIRPLANE_MODE --ez state true")
    # time.sleep(5)
    elif device == "pixel3":
        print "in pixel3\n"
        subprocess.call("adb shell settings put global airplane_mode_on 1",shell=True)
        subprocess.call("adb shell su -c am broadcast -a android.intent.action.AIRPLANE_MODE --ez state true",shell=True)
        #procId.communicate('settings put global airplane_mode_on 1\nexit\n')
        #procId.communicate('am broadcast -a android.intent.action.AIRPLANE_MODE --ez state true\nexit\n')
        #result = run_adb_command("adb shell settings put global airplane_mode_on 1")
        #time.sleep(1)
        #result = run_adb_command("adb shell am broadcast -a android.intent.action.AIRPLANE_MODE --ez state true")
    # time.sleep(5)
    elif device == "all":
        cmd = ['adb shell am start -a android.settings.AIRPLANE_MODE_SETTINGS & adb shell input tap 300 1100']
        #print 'first adb call\n'
        #subprocess.call("adb shell input keyevent KEYCODE_WAKEUP",shell=True)
        print 'second adb call\n'
        #subprocess.call("adb shell am start -a android.settings.AIRPLANE_MODE_SETTINGS",shell=True)
        subprocess.call("adb shell input tap 300 1100",shell=True) #honorp8 300 300

        print 'second adb call 2\n'
    else:
        result = send_at_cmd("AT+CFUN=0", device, None, environment)
    #ime.sleep(2) # TODO: Check if this can be removed
    return result


def airplane_mode_off():
    result = 1
    print 'airplane mode off'
    time.sleep(1)
    # result = run_adb_command("adb shell settings put global airplane_mode_on 0")
    # #result = run_adb_command("adb shell \"su -c 'am broadcast -a android.intent.action.AIRPLANE_MODE'\"")
    if device == "usb":
        print "usb on\n"
        #subprocess.call("sudo sh -c \"echo '1-1.4' |sudo tee /sys/bus/usb/drivers/usb/bind\"",shell=True)
        #subprocess.call("sudo sh -c \"echo 1 > /sys/bus/usb/devices/1-2.4/authorized\"",shell=True)
        subprocess.call("sudo sh -c \"echo enabled | sudo tee /sys/bus/usb/devices/usb1/power/wakeup\"",shell=True)
    elif device == "galaxys3" or device== "huwaeip8":
        cmd = ['adb', 'shell', 'su']
        procId = subprocess.Popen(cmd, stdin=subprocess.PIPE)
        procId.communicate('settings put global airplane_mode_on 0\nam broadcast -a android.intent.action.AIRPLANE_MODE --ez state false\nexit\n')
        #procId.communicate('settings put global airplane_mode_on 0\nexit\n')
        #procId.communicate('am broadcast -a android.intent.action.AIRPLANE_MODE --ez state false\nexit\n')
        #procId.communicate('am broadcast -a android.intent.action.AIRPLANE_MODE\nexit\n')
        #result = run_adb_command("adb shell settings put global airplane_mode_on 0")
        #time.sleep(1)
        #result = run_adb_command("adb shell am broadcast -a android.intent.action.AIRPLANE_MODE --ez state false")
    # time.sleep(5)
    elif device == "pixel3":
        print "in pixel3\n"
        subprocess.call("adb shell settings put global airplane_mode_on 0",shell=True)
        subprocess.call("adb shell su -c am broadcast -a android.intent.action.AIRPLANE_MODE --ez state false",shell=True)
    elif device == "all":
        cmd = ['adb shell am start -a android.settings.AIRPLANE_MODE_SETTINGS & adb shell input tap 300 1100']
        #subprocess.call("adb shell input keyevent KEYCODE_WAKEUP",shell=True)
        #subprocess.call("adb shell am start -a android.settings.AIRPLANE_MODE_SETTINGS",shell=True)
        subprocess.call("adb shell input tap 300 1100",shell=True)
    else:
        result = send_at_cmd("AT+CFUN=1", device, None, environment)
    #time.sleep(2) # TODO: Check if this can be removed
    return result

###################################################################################################

def handle_reset(client_socket):
   
    # turn off the airplane mode
    print '--- START: Handling RESET command ---'
    if device == "iphonexs" or device == "usb":
        print 'nothing done in reset\n'
        client_socket.send('DONE\n')
        return 
    airplane_mode_on()
    time.sleep(1) # # TODO: Check if this can be removed. This sleep will be handled by LTEUESUL
    client_socket.send('DONE\n')
    print '### DONE: Handling RESET command ###'



def handle_enable_s1(client_socket):
    # stop and start cellular connectivity => turn on and then off the airplane mode
    print '--- START: Handling ENABLE_S1 command ---'
    if device == "iphonexs":
        print 'Iphone detected\n'
        cmd = ['idevicediagnostics', 'restart']
        procId = subprocess.Popen(cmd, stdin=subprocess.PIPE)
        client_socket.send('DONE\n')
        return 
    print 'Enabling airplane mode'
    airplane_mode_on()

    print 'Sleeping for 1 second'
    time.sleep(1)

    print 'Disabling airplane mode'
    airplane_mode_off()
    #print 'Sleeping for 1 second'
    #time.sleep(1)

    client_socket.send('DONE\n')
    print '### DONE: Handling ENABLE_S1 command ###'
    return


def handle_ue_reboot(client_socket):
    print '--- START: Handling UE REBOOT command ----'
    if device == "iphonexs":
        print 'Iphone detected\n'
        cmd = ['idevicediagnostics', 'restart']
        procId = subprocess.Popen(cmd, stdin=subprocess.PIPE)
        client_socket.send('DONE\n')
        return 
    print 'Enabling airplane mode'
    airplane_mode_on()


    #reboot()
    #print device +"yo"+environment
    reboot_env(device,environment)
    #time.sleep(10)
    time.sleep(5) # lets try with 5 second delay.

    print '@@@@@@@@@@@@@@@ SENDING DONE @@@@@@@@@@@@@@@@@@@'
    client_socket.send('DONE\n')
    print '### DONE: Handling UE REBOOT command ###'
    return


def handle_adb_server_restart(client_socket):
    #TODO: Implement
    print '-- START: Handling ADB SERVER RESTART  command ---'

    result = restart_adb_server()

    client_socket.send('DONE\n')
    print '### DONE: Handling ADB SERVER RESTART command ###'
    return

####################################################################################################
# thread fuction
def client_handler(client_socket):
    #reboot_env(device,environment)
    while True:

        # data received from client
        data = client_socket.recv(1024)

        if not data:
            print('Bye')
            # lock released on exit
            print_lock.release()
            break

        command = data.lower()

        if "reset" in command:
            handle_reset(client_socket)

        elif "enable_s1" in command:
            handle_enable_s1(client_socket)

        elif "ue_reboot" in command:
            handle_ue_reboot(client_socket)

        elif "adb_server_restart" in command:
            handle_adb_server_restart(client_socket)

    client_socket.close()
    print '--- AIRPLANE MODE ON BEFORE EXIT ---'
    airplane_mode_on()

def Main():
    global device
    global environment
    host = ""
    if (len(sys.argv)<3):
        print 'Usage: TCPServer-new.py <hostname> l,w <device name> s8plus, nexus6, galaxys3, huwaeip8, iphonexs, pixel3, all, usb'
        exit()

    if sys.argv[1] is "l":
		environment = "linux"
    else:
		environment = "windows"
    print str(sys.argv)
    device = sys.argv[2]
    #print "life " + device + "hiiii" + sys.argv[2]
    #print "hi" + device+ environment
    print '#############################################'
    print '######### UE Controller started #############'
    print '#############################################'

    print 'Initializing the controller...'
    #airplane_mode_on()
    try:
        set_environment()
    except:
        print 'ERROR: In setting the environment!'
    try:
        killProess("adb")
        print 'ADB has been killed'
        time.sleep(1)
    except:
        print 'ERROR: In killing adb process!'

    # AIRPLANE MODE ON.
    '''
    print "Trying to turn AIRPLANE mode ON "
    subprocess.call("adb shell settings put global airplane_mode_on 1", shell=True)
    time.sleep(2)
    cmd = ['adb', 'shell', 'su']
    procId = subprocess.Popen(cmd, stdin=subprocess.PIPE)
    procId.communicate('am broadcast -a android.intent.action.AIRPLANE_MODE\nexit\n')
    print 'Done with AIRPLANE mode ON'
    '''
    # reverse a port on your computer
    # in our case it is 12345 but it
    # can be anything

    port = 61000
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((host, port))
    print("socket binded to post", port)

    # put the socket into listening mode
    s.listen(5)
    print("socket is listening")
    if device == "all":
        subprocess.call("adb shell exit",shell=True)
        #airplane_mode_on()
    # a forever loop until client wants to exit
    while True:
        # establish connection with client
        client_socket, addr = s.accept()

        # lock acquired by client
        print_lock.acquire()
        print('Connected to :', addr[0], ':', addr[1])

        # Start a new thread and return its identifier
        start_new_thread(client_handler, (client_socket,))
    s.close()


if __name__ == '__main__':
    Main()