import admin
import sys
import time
import socket
import subprocess
import threading
import os
import coloredlogs, logging
from atsend import *
#from utility import *

if not admin.isUserAdmin():
   admin.runAsAdmin()

# GLOBAL VARIABLES
WIN_RUNTIME = ['cmd.exe', '/C']
OS_LINUX_RUNTIME = ['/bin/bash', '-l', '-c']

TASKLIT = 'tasklist'
#KILL = 'taskkill /F /IM'
KILL = ['taskkill', '/F', '/IM']


bind_ip = '127.0.0.1'
bind_port = 61000

isWindows = True

# Global vars
environment = ''
DEFAULT_TIMEOUT=1

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




def airplane_mode_on():
    print 'airplane mode on'
    #result = run_adb_command("adb shell settings put global airplane_mode_on 1")
    #result = run_adb_command("adb shell \"su -c 'am broadcast -a android.intent.action.AIRPLANE_MODE'\"")
    # cmd = ['adb', 'shell', 'su']
    # procId = subprocess.Popen(cmd, stdin=subprocess.PIPE)
    # procId.communicate('am broadcast -a android.intent.action.AIRPLANE_MODE\nexit\n')
    # time.sleep(5)
    result = send_at_cmd("AT+CFUN=0", 'Nexus6', None, environment)
    time.sleep(2)
    return result


def airplane_mode_off():
    print 'airplane mode off'
    # result = run_adb_command("adb shell settings put global airplane_mode_on 0")
    # #result = run_adb_command("adb shell \"su -c 'am broadcast -a android.intent.action.AIRPLANE_MODE'\"")
    # cmd = ['adb', 'shell', 'su']
    # procId = subprocess.Popen(cmd, stdin=subprocess.PIPE)
    # procId.communicate('am broadcast -a android.intent.action.AIRPLANE_MODE\nexit\n')
    # time.sleep(5)
    result = send_at_cmd("AT+CFUN=1", 'Nexus6', None, environment)
    time.sleep(2)
    return result



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
        command = "ps -ef | grep" + serviceName + " | grep -v grep | awk '{print $2}' | xargs sudo kill -9"
        command = OS_LINUX_RUNTIME + [command]
    else:
        raise Exception('EnvironmentError: unknow OS')
    subprocess.check_output(command)
    return


def run_command(command):
    p = subprocess.Popen(command,
                         stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT)
    return iter(p.stdout.readline, b'')



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


############## restart adb server ###############
def restart_adb_server():
    result = ''
    print 'Killing Server'
    result = run_adb_command('adb kill-server')
    time.sleep(1)
    print 'Starting server'
    result = run_adb_command('adb start-server')
    print 'Server Restart done.'
    return True

'''
def reboot():
    #TODO: Implement reboot
    print 'Rebooting UE'
    run_adb_command('adb reboot')

    if environment == 'windows' and isWindows == True:
        wait_command = "C:\\research\\lte\\code\\LTEUE-State-Fuzzing\\TCPServer-x86\\src\\adb-reboot-wait.sh"
        #wait_command = "C:\\Users\\Mitziu\\Documents\\Research\\LTEUE-State-Fuzzing\\TCPServer-x86\\src\\adb-reboot-wait.sh"
    elif environment == 'linux' and isWindows == False:
        wait_command = '/home/mitziu/LTE/LTEUE-State-Fuzzing/TCPServer-x86/src/adb-reboot-wait.sh'
    else:
        raise Exception('EnvironmentError: unknow OS.')

    runProcess(wait_command)

    if not isWindows:
        print 'Sleeping while UE reboots'
        time.sleep(50)
    return
'''

def handle_reset(client_socket):
    # turn off the airplane mode
    print '--- START: Handling RESET command ---'
    airplane_mode_on()
    time.sleep(1)
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
    print 'Sleeping for 1 second'
    time.sleep(1)

    client_socket.send('DONE\n')
    print '### DONE: Handling ENABLE_S1 command ###'
    return

def handle_ue_reboot(client_socket):
    print '--- START: Handling UE REBOOT command ----'

    print 'Enabling airplane mode'
    airplane_mode_on()

    #reboot()
    reboot_env(environment)

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




def handle_adb_process_kill_and_start(client_socket):

    if isWindows:
        processName = 'adb.exe'
    else:
        processName = 'adb'

    if isProcessRunning(processName):
        print '### Killing the ADB process ###'
        killProess(processName)

        if isProcessRunning(processName):
            print 'The ADB process is still running'
        else:
            print 'The ADB process is killed'

    adb_usb = 'adb usb'
    print '### Trying to fix adb error with adb usb ###'
    run_command(adb_usb)

    print 'Starting server'
    run_command('adb start-server')
    print 'Server Restart done'



def handle_client_connection(client_socket, server_socket):
    print "Client Handler initiated"


    try:
        while True:
            # Obtains command from state learner
            request = client_socket.recv(1024)
            request = request.lower()

            if "reset" in request:
                handle_reset(client_socket)

            elif "enable_s1" in request:
                request = request.strip()
                print '\n------------', request, '--------------\n'
                handle_enable_s1(client_socket)

            elif "ue_reboot" in request:
                handle_ue_reboot(client_socket)

            elif "adb_server_restart" in request:
                handle_adb_server_restart(client_socket)

            elif "test_adb_process_kill_and_start" in request:
                handle_adb_process_kill_and_start(client_socket)
    except KeyboardInterrupt:
        print 'Keyboard Interrupt'
        print 'Closing connection'
        client_socket.close()
        print "Turning Airplane mode on at Client Handler"
        airplane_mode_on()

    except:
        print 'ERROR Occurred'
        print 'Closing Connection'
        client_socket.close()
        print "Turning Airplane mode on at Client Handler"
        airplane_mode_on()

    return

def test_reboot_env():
    reboot_env()
    restart_adb_server()
    return

def test_adb_exe_kill():
    killProess('adb.exe')

def main():
    print 'UE Controller Server has started...'
    try:
        set_environment()
        killProess("adb.exe")
        print 'ADB has been killed'
        time.sleep(1)
    except:
        print 'ADB is not currently running'

    #test_reboot_env()

    print 'Binding and attempting to reuse socket'


    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind(('', bind_port))

    server.listen(5)  # max backlog of connections

    try:
        # TODO: Parse argument for is windows

        global isWindows

        if len(sys.argv) < 2:
            print 'Illegal number of arguments'
            print '-l - Linux, -w - Windows'
            exit(1)

        if sys.argv[1] == '-w':
            isWindows = True
        elif sys.argv[1] == '-l':
            isWindows = False
        else:
            print 'Illegal argument'
            print '-l - Linux, -w - Windows'

        client_sock, address = server.accept()
        print 'Accepted connection from {}:{}'.format(address[0], address[1])
        print 'Handling Client Connection'
        handle_client_connection(client_sock)
        print 'Closing Server socket ...'
        server.close()
    except KeyboardInterrupt:
        print 'Keyboard Interrupt'
        print 'Closing connection'
        server.close()
    except:
        print 'ERROR Occurred'
        print 'Closing Connection'
        server.close()


if __name__ == '__main__':
    main()

