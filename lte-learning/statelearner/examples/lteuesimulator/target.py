import socket
import random

deregistered = 0
registered_intiaited = 1
registered_intiaited_authenticated = 2
registered = 3



s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("localhost", 48888))
s.listen(1)

(client, address) = s.accept()

state = 0



state = deregistered
while 1:
  cmd = client.recv(1024).strip()

  if cmd == "":
    break

  response = "null_action"

  if cmd == "RESET":
    state = deregistered
    response = "DONE"

  elif cmd == "enable_s1":
    state = registered_intiaited
    response = "attach_request"

  elif cmd == "attach_reject":
    state = deregistered

  elif cmd == "auth_reject":
    state = deregistered

  elif cmd == "identity_request":
      if state == registered_intiaited or state == registered:
        response = "identity_response"

  elif cmd == "auth_request":
    if state == registered_intiaited or state == registered_intiaited_authenticated:
      response = "auth_response"
      state = registered_intiaited_authenticated
    elif state == registered:
      response = "auth_response"

  elif cmd == "security_mode_command":
    if state == registered_intiaited_authenticated:
      response = "security_mode_complete"
    elif state == registered:
      response = "security_mode_complete"

  elif cmd == "attach_accept":
    if state == registered_intiaited_authenticated:
      response = "attach_complete"
      state = registered
    if state == registered:
      response = "attach_complete"
      state = registered
    '''
    # What happens if UE is already attached and receives attach_complete message
    elif state == registered:
      if (random.choice([True, False])):
        response = "security_mode_complete"
      else:
        response = "security_mode_reject"
    '''
  elif cmd == "detach_request":
    if state == registered_intiaited or state == registered_intiaited_authenticated or state == deregistered:
      state = deregistered
      response = "detach_accept "
  print (state, cmd, '/', response)

  if response != "null_action":
    client.sendall(response + "\n")
