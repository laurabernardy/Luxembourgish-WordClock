import socket
import struct
import sys
from datetime import datetime
import queue

# ip address of network interface
MCAST_IF_IP = '192.168.0.227'

multicast_group = '230.120.10.2'
server_address = ('', 8123)

# Create the socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind to the server address
sock.bind(server_address)

print("Start")

# Tell the operating system to add the socket to the multicast group
# on all interfaces.
group = socket.inet_aton(multicast_group)
mreq = struct.pack('4s4s', group, socket.inet_aton(MCAST_IF_IP))
sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

print("Ready")
saveCounter = 0

buffer = queue.Queue(20)

# Receive/respond loop
while True:
    data, address = sock.recvfrom(1024)
    data_str = data.decode("utf-8").strip()
    print(address, ": ", data_str)
    data_str = datetime.now().strftime('%b-%d-%Y_%H%M%S') + ": " + data_str
    buffer.put(data_str)
    if buffer.full():
        buffer.get()


    if "NTP-Update not successful" in data_str or "Start program" in data_str:
        f = open("log.txt",'a')
        while not buffer.empty():    
            f.write(buffer.get())
            f.write("\n")
        f.close()
        saveCounter = 20
        
    if saveCounter > 0:
        f = open("log.txt",'a')
        f.write(data_str)
        f.write("\n")
        if saveCounter == 1:
            f.write("\n")
        f.close()
        saveCounter -= 1

