#!/usr/bin/python



import sys
import time
import logging
import serial
import subprocess
from subprocess import Popen, PIPE
import os
import signal
from stat import *


try:
	import readline
except ImportError:
	import pyreadline as readline


# Global vars
# environment = ''

DEFAULT_BAUD=115200
DEFAULT_TIMEOUT=1

log_file = "log/atsend.log"


log_level = logging.DEBUG
baud_rate = 115200
time_out = 1
at_port = None
max_poll = 4
poll_delay = 2
format_string = ["<value>", "%d", "%u", "%s", "%c", "%x", "<n>", "<index>", "<args>", "<gain>"]
debug_cmd_gen = True


def create_serial(port, baud):
	#print("Creating serial port %s @ %d baud" % (port, baud))
	return serial.Serial(port, baud, timeout=DEFAULT_TIMEOUT)
	

def recv():
	my_poll = 0
	lines = []
	# Make sure we dont go _too_ fast
	start_time = time.time()
	# To deal with the response delay
	while my_poll < max_poll:
		line = at_port.readline()
		#print line
		# timeout
		if line == "":
			my_poll += 1
			#time.sleep(poll_delay)
			continue

		# clean up the output for comparison
		line_clean = line.strip('\r\n')
		lines += [line]
		#print line_clean
		# a terminal response. end NOW
		if 'ERROR' == line_clean:
			break
		elif 'CME ERROR' in line_clean:
			break
		elif 'OK' == line_clean:
			break
		elif 'NO CARRIER' == line_clean:
			break
		elif 'ABORTED' == line_clean:
			break
		elif 'NOT SUPPORTED' == line_clean:
			break
		else:
			continue

	# "Do you know how fast you were going?"
	#if time.time() - start_time < 1.0:
		#time.sleep(2)
	
	# post-processing
	lines2 = []
	for ln in lines:
		if ln == '\r\n':
			continue
		if ln.endswith('\r\n'):
			lines2.append(ln[:-1])
	return lines2


def send(cmd):
	'''
	True - sending failed
	False - sending successful
	'''
	cmd2 = cmd
	return at_port.write(cmd2 + '\r\r')


def at_probe():
	found = []
	if environment == 'linux':
		print 'Probing for ttyACM devices...'
		for i in range(10):
			devname = '/dev/ttyACM%d' % i
			if not os.path.exists(devname):
				continue
			mode = os.stat(devname).st_mode
			if S_ISCHR(mode):
				found += [devname]
	
	elif environment == 'windows':
		print 'Probing for COM ports...'
		for i in range(256):
			port = 'COM%d' % i
			try:
				s = serial.Serial(port)
				s.close()
				found.append(port)
			except (OSError, serial.SerialException):
				pass
	else:
		raise Exception('EnvironmentError: unknown OS')
	return found


def check_at_interface(ser, cmd):
	start = time.time()
	ser.write(cmd + "\r")
	lines = []

	while True:
		line = ser.readline()

		# timeout
		if line == "":
			break

		# clean up the output (we dont want line endings)
		line_clean = line.strip('\r\n')

		lines += [line_clean]

		if 'ERROR' == line_clean:
			break
		elif 'CME ERROR' in line_clean:
			break
		elif 'OK' == line_clean:
			break
		elif 'NO CARRIER' == line_clean:
			break
		elif 'ABORTED' == line_clean:
			break
	end = time.time()
	print end-start
	return lines


def at_connect(dev, baud=DEFAULT_BAUD):
	try:
		ser = create_serial(dev, baud)
	except:
		ser = None
	
	if ser != None:
		resp = check_at_interface(ser, 'AT')
		if len(resp) > 0 and resp[-1] == 'OK':
			return ser
		ser.close()
	return None


		 
def run_command(command):
	p = subprocess.Popen(command,
						 stdout=subprocess.PIPE,
						 stderr=subprocess.STDOUT)
	return iter(p.stdout.readline, b'')


def set_environment():
	global environment
	if os.name == 'posix':
		environment = 'linux'
	elif os.name == 'nt':
		environment = 'windows'
	else:
		raise Exception('EnvironmentError: unknow OS')


def get_serial_connection(port):
	set_environment()
	devices = at_probe() if (port == None) else [port]
	print 'Devices = ', devices
	if len(devices) == 0:
		print "No devices found"
		return
	for d in devices:
		print 'Trying device ', d
		ser = at_connect(d)
		if ser is not None:
			return ser
	return None


# reboot adb and the connected device
def reboot_env(device,env):
	print "my" + device
	global environment, at_port
	environment = env
	at_port = None
	print '--------Rebooting the Environment ------------'

	# time.sleep(1)
	# subprocess.call("adb kill-server",shell= True)

	time.sleep(1)
	subprocess.call("adb reboot",shell = True)

	print 'Now its time for waiting...'
	if environment == 'linux':
		print 'LINUX'
		subprocess.call(['./test.sh'],shell = True)
	elif environment == 'windows':

		print 'Waiting for the device to reboot!'
		subprocess.call(['test.bat'],shell = True)
	else:
		print 'NO ENVIRONMENT SELECTED :('
		raise Exception('EnvironmentError: unknow OS')
	time.sleep(3)
	subprocess.call("adb kill-server",shell=True)
	time.sleep(2)

	if device is "nexus6":
		cmd = ['adb', 'shell','su']
		procId = subprocess.Popen(cmd, stdin = subprocess.PIPE)
		procId.communicate('setprop sys.usb.config diag,adb\nexit\n')
	if device in "s8plus" or device in "galaxys3":
		print "yes!"
		at_port = get_serial_connection(port=None)
		#procId = subprocess.Popen(cmd, stdin=subprocess.PIPE)
		#procId.communicate('./usbswitcher -s')
	time.sleep(10)


def send_at_cmd(cmd, device=None, port=None, env='windows'):
	# Open the serial port
	global at_port, environment
	environment = env
	print "IK\n"
	if at_port is None:
		at_port = get_serial_connection(port)
		port = at_port
		print "AT Port = " + str(at_port)

	if at_port is None:
		print 'AT PORT NOT FOUND'
		#reboot_env(device,environment)
		time.sleep(5)
		return send_at_cmd(cmd, device, port)
	print 'Serial port: ', at_port.port
	logging.info("port is opened for %s" % at_port.port)

	timer_for_check = 10
	print cmd
	cmd = str(cmd)
	start = time.time()
	try:
		send(cmd)
	except serial.serialutil.SerialException as e:
		print 'AT Command Failed!!!'
		print 'Exception: ', e
		at_port.close()
		if device == "LGg3":
			return send_at_cmd(cmd, device, port, environment)
		#reboot_env(device,environment)
		return send_at_cmd(cmd, device, port, environment)

	r = recv()
	print r 
	if len(r) == 0:
	    at_port = get_serial_connection(port)
	    port = at_port
	    print "hiiii"
	    return send_at_cmd(cmd, device, port)
	print 'AT COMMAND Status = ', r
	

	return True


	
def main():
	status = send_at_cmd("AT", 'test_dev')
	status = send_at_cmd("ATI", 'test_dev')

	################ WE NEED THESE ONES #################
	status = send_at_cmd("AT+CFUN=0", 'test_dev') # airplane mode on
	status = send_at_cmd("AT+CFUN=1", 'test_dev') # airplane mode off

if __name__ == "__main__":
	print 'Test running...'
	main()
	print '\nTest Completed\n'
