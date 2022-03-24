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
    #result = send_at_cmd("AT+CFUN=34,1", device, None, environment)
    result = send_at_cmd("AT+COPS=2", device, None, environment)
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
    print 'airplane mode on'
    #result = run_adb_command("adb shell settings put global airplane_mode_on 1")
    #result = run_adb_command("adb shell \"su -c 'am broadcast -a android.intent.action.AIRPLANE_MODE'\"")
    # cmd = ['adb', 'shell', 'su']
    # procId = subprocess.Popen(cmd, stdin=subprocess.PIPE)
    # procId.communicate('am broadcast -a android.intent.action.AIRPLANE_MODE\nexit\n')
    result2 = send_at_cmd("AT+COPS=2", device, None, environment)
    #result2 = send_at_cmd("AT+COPS=2", device, None, environment)
    result2 = send_at_cmd("AT", device, None, environment)
    time.sleep(2)
    result = send_at_cmd("AT+CFUN=0,0", device, None, environment)
    result2 = send_at_cmd("AT", device, None, environment)
    #result2 = send_at_cmd("AT", device, None, environment)
    #result2 = send_at_cmd("AT", device, None, environment)
    #ime.sleep(2) # TODO: Check if this can be removed
    return result


def airplane_mode_off():
    print 'airplane mode off'
    # result = run_adb_command("adb shell settings put global airplane_mode_on 0")
    # #result = run_adb_command("adb shell \"su -c 'am broadcast -a android.intent.action.AIRPLANE_MODE'\"")
    # cmd = ['adb', 'shell', 'su']
    # procId = subprocess.Popen(cmd, stdin=subprocess.PIPE)
    # procId.communicate('am broadcast -a android.intent.action.AIRPLANE_MODE\nexit\n')
    # time.sleep(5)
    #time.sleep(1)
    #result = send_at_cmd("AT+COPS=2", device, None, environment)
    #result2 = send_at_cmd("AT", device, None, environment)
    time.sleep(1)
    result = send_at_cmd("AT+CFUN=1,0", device, None, environment)
    time.sleep(2)
    #result = send_at_cmd("AT+CFUN=34", device, None, environment)
    #result2 = send_at_cmd("AT+CFUN=1,0", device, None, environment)
    #result2 = send_at_cmd("AT", device, None, environment)
    #result2 = send_at_cmd("AT", device, None, environment)
    time.sleep(1)
    result2 = send_at_cmd("AT+COPS=0", device, None, environment)
    #result2 = send_at_cmd("AT+COPS=0", device, None, environment)
    time.sleep(4)
    result2 = send_at_cmd("AT", device, None, environment)
    result2 = send_at_cmd("AT", device, None, environment)
    #result2 = send_at_cmd("AT", device, None, environment)
    #time.sleep(4) # TODO: Check if this can be removed
    return result

###################################################################################################

def handle_reset(client_socket):
    # turn off the airplane mode
    #print '--- START: Handling RESET command ---'
    #result = send_at_cmd("AT+COPS=2", device, None, environment)
    #result2 = send_at_cmd("AT", device, None, environment)
    airplane_mode_on()
    #result = send_at_cmd("AT+CFUN=32,1", device, None, environment)
    #time.sleep(3)
    #result = send_at_cmd("AT+CFUN=34,1", device, None, environment)
    #result = send_at_cmd("AT+CFUN=32", device, None, environment)
    #time.sleep(1)
    airplane_mode_on()
    #time.sleep(1) # # TODO: Check if this can be removed. This sleep will be handled by LTEUESUL
    client_socket.send('DONE\n')
    print '### DONE: Handling RESET command ###'



def handle_enable_s1(client_socket):
    # stop and start cellular connectivity => turn on and then off the airplane mode
    print '--- START: Handling ENABLE_S1 command ---'
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

    print 'Enabling airplane mode'
    airplane_mode_on()
    airplane_mode_off()

    #reboot()
    #print device +"yo"+environment
    #reboot_env(device,environment)
    #time.sleep(10)
    #time.sleep(5) # lets try with 5 second delay.

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
            handle_reset(client_socket)

        elif "adb_server_restart" in command:
            handle_adb_server_restart(client_socket)

    client_socket.close()
    print '--- AIRPLANE MODE ON BEFORE EXIT ---'
    airplane_mode_on()

def Main():
    global device
    global environment
    host = ""
    if (len(sys.argv)<2):
        print 'Usage: TCPServer-new.py <hostname> l,w'
        exit()

    if sys.argv[1] is "l":
		environment = "linux"
    else:
		environment = "windows"
    print str(sys.argv)
    device = 'nexus6'
    #print "life " + device + "hiiii" + sys.argv[2]
    #print "hi" + device+ environment
    print '#############################################'
    print '######### UE Controller started #############'
    print '#############################################'

    print 'Initializing the controller...'

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