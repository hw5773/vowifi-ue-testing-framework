import admin
import sys
import time
import socket
import subprocess
import threading
import atsend
import os
# if not admin.isUserAdmin():
#    admin.runAsAdmin()

# GLOBAL VARIABLES
WIN_RUNTIME = ['cmd.exe', '/C']
OS_LINUX_RUNTIME = ['/bin/bash', '-l', '-c']

TASKLIT = 'tasklist'
# KILL = 'taskkill /F /IM'
KILL = ['taskkill', '/F', '/IM']

bind_ip = '0.0.0.0'
bind_port = 61000

isWindows = True

print 'Listening on {}:{}'.format(bind_ip, bind_port)

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
    # TODO: Implement AT command for airplane mode on
    print 'airplane mode on'
    # run_adb_command("adb shell settings put global airplane_mode_on 1")
    # run_adb_command("adb shell \"su -c 'am broadcast -a android.intent.action.AIRPLANE_MODE'\"")
    atsend.send_at_cmd("AT+CFUN=0", 'Nexus6')


def airplane_mode_off():
    # TODO: Implement AT command for airplane mode off
    print 'airplane mode off'
    # run_adb_command("adb shell settings put global airplane_mode_on 0")
    # run_adb_command("adb shell \"su -c 'am broadcast -a android.intent.action.AIRPLANE_MODE'\"")
    atsend.send_at_cmd("AT+CFUN=1", 'Nexus6')


def reboot():
    # TODO: Implement reboot
    print 'Rebooting UE'
    run_adb_command('adb reboot')

    if isWindows:
        wait_command = "C:\\research\\lte\\code\\LTEUE-State-Fuzzing\\TCPServer-x86\\src\\adb-reboot-wait.sh"
        wait_command = "C:\\Users\\Mitziu\\Documents\\Research\\LTEUE-State-Fuzzing\\TCPServer-x86\\src\\adb-reboot-wait.sh"
    else:
        wait_command = '/home/mitziu/LTE/LTEUE-State-Fuzzing/TCPServer-x86/src/adb-reboot-wait.sh'

    runProcess(wait_command)

    if not isWindows:
        print 'Sleeping while UE reboots'
        time.sleep(50)


def restart_server():
    print 'Killing Server'
    run_adb_command('adb kill-server')
    time.sleep(1)
    print 'Starting server'
    run_adb_command('adb start-server')
    print 'Server Restart done.'


def runProcess(command):
    print 'command to run: ' + command

    if isWindows:
        command = WIN_RUNTIME + [command]
    else:
        command = OS_LINUX_RUNTIME + [command]

    out = subprocess.check_output(command)

    return out


def isProcessRunning(serviceName):
    command = ''

    if isWindows:
        command = TASKLIT
    else:
        command = "pidof " + serviceName
        command = OS_LINUX_RUNTIME + [command]

    result = subprocess.check_output(command)
    return serviceName in result


def killProess(serviceName):
    command = ''

    if isWindows:
        command = KILL + [serviceName]
        print command
    else:
        command = "ps -ef | grep" + serviceName + " | grep -v grep | awk '{print $2}' | xargs sudo kill -9"
        command = OS_LINUX_RUNTIME + [command]

    subprocess.check_output(command)


def run_adb_command(command):
    result = ""
    while True:
        result = runProcess(command).lower()
        return
        if "error" in result:
            print 'reboot adb server'
            if isWindows:
                processName = "adb.exe"
            else:
                processName = "adb"

            if isProcessRunning(processName):
                print '### Killing the ADB process ###'
                killProess(processName)

            time.sleep(1)

            adb_usb_command = 'adb usb'
            print '### Trying to fix adb error with adb usb'
            runProcess(adb_usb_command)

            print 'Starting server'
            runProcess('adb start-server')
            print 'Server restart done'

        else:
            break

    return result


def handle_reset(client_socket):
    print '--- START: Handling RESET command ---'
    airplane_mode_on()
    client_socket.send('DONE\n')
    print '### DONE: Handling RESET command ###'


def handle_enable_s1(client_socket):
    print '--- START: Handling ENABLE_S1 command ---'
    print 'Enabling airplane mode'
    airplane_mode_on()

    print 'Sleeping for 1 second'
    time.sleep(1)

    print 'Disabling airplane mode'
    airplane_mode_off()
    print '### DONE: Handling ENABLE_S1 command ###'


def handle_ue_reboot(client_socket):
    print '--- START: Handling UE REBOOT command ----'

    print 'Enabling airplane mode'
    airplane_mode_on()

    reboot()

    client_socket.send('DONE\n')
    print '### DONE: Handling UE REBOOT command ###'


def handle_adb_server_restart(client_socket):
    # TODO: Implement
    print '-- START: Handling ADB SERVER RESTART  command ---'

    restart_server()

    client_socket.send('DONE\n')
    print '### DONE: Handling ADB SERVER RESTART command ###'


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
    runProcess(adb_usb)

    print 'Starting server'
    runProcess('adb start-server')
    print 'Server Restart done'


def handle_client_connection(client_socket):
    print "Client Handler initiated"

    print "Turning Airplane mode on at Client Handler"
    airplane_mode_on()

    print "Replying to state learner"
    client_socket.send('.')
    try:
        while True:
            # Obtains command from state learner
            request = client_socket.recv(33)
            request = request.lower()

            if "reset" in request:
                handle_reset(client_socket)

            elif "enable_s1" in request:
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
    except:
        print 'ERROR Occurred'
        print 'Closing Connection'
        client_socket.close()
    return


def main():
    try:
        set_environment()
        killProess("adb.exe")
        print 'ADB has been killed'
        time.sleep(1)
    except:
        print 'ADB is not currently running'

    print 'Binding and attempting to reuse socket'

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((bind_ip, bind_port))
    server.listen(1)  # max backlog of connections

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
