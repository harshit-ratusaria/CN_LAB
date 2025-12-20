# CN_LAB
Assignments of CN lab

Assignment1: write a program to run TCP client and server socket programs where client first says "Hi" and in response server says "Hello"

![cn_ass1-A](https://github.com/harshit-ratusaria/CN_LAB/blob/c6ffb2fa377a7c3799381f0497a4517a60d5948b/assignment1/cn_ass1-A.png)
![cn_ass1-B](https://github.com/harshit-ratusaria/CN_LAB/blob/c6ffb2fa377a7c3799381f0497a4517a60d5948b/assignment1/cn_ass1-B.png)

Assignment2: Write a program using TCP socket to implement the following: i. Server maintains records of fruits in the format: fruit-name, quantity Last-sold, (server timestamp), ii. iii. Multiple client purchase the fruits one at a time, The fruit quantity is updated each time any fruit is sold, iv. Send regret message to a client if therequested quantityof the fruit is not available. v. Display the customer ids <IP, port> who has done transactions already. This list should be updated in the server every time a transaction occurs. vi. The total number of unique customers who did some transaction will be displayed to the customer every time.

![cn_ass-2A](/assignment2/cn_ass-2A.png)
![cn_ass2-B](assignment2/cn_ass2-B.png)
![cn_ass2-C](assignment2/cn_ass2-C.png)

Assignment 3: Same as Assignment 2 using UDP socket. .

![cn_ass3-A](/assignment3/cn_ass3-A.png)
![cn_ass3-B](assignment3/cn_ass3-B.png)

Assignment 4: Install wireshark in a VM (Virtual Machine) environment. Draw a time diagram to show the steps in the protocols recorded in the captured file (saved in the .pcap file of wireshark) during a PING operation. List the L2, L3, L4 header fields that can be extracted from the .pcap file.

Capture ICMP

![cn_ass3-A](/assignment4/cn_ass4-A.png)

Time diagram using flow graph

![cn_ass3-A](/assignment4/cn_ass4-B.png)

L1 header

![cn_ass3-A](/assignment4/cn_ass4-C.png)

L2 headers

![cn_ass3-A](/assignment4/Cn_ass4-D.png)

L3 headers

![cn_ass3-A](/assignment4/CN_ass4-E.png)

L4 Headers

![cn_ass3-A](/assignment4/Cn_ass4-F.png)

Assignment 5: Learn and use maximum number of packet generation tools. Steps/ Hints: Install Iperf, D-ITG etc. and send traffic among mininet hosts. . Output for IPERF (TCP)

![cn_ass3-A](/assignment5/cn_ass5-A.png)

IPERF UDP

![cn_ass3-A](/assignment5/cn_ass5-B.png)

ITGSend

![cn_ass3-A](/assignment5/cn_ass5-C.png)

Assignment6: Develop a simple C based network simulator to analyze TCP traffic. Steps/ Hints: 1. Use TCP/IP based C libraries including socket to listen to the incoming packets at the Ethernet port. 2. Extract header and data of the incoming packets.

![cn_ass3-A](/assignment6/cn_ass6-A.png)
![cn_ass3-A](/assignment6/cn_ass6-C.png)

Assignment 7: Write UDP client server socket program where client sends one/two number(s)(integer or floating point) to server and a scientific calculator operation (like sin,cos,*,/, inv etc.) and server responds with the result after evaluating the value of operation as sent by the client. Server will maintain a scientific calculator. Detect in the mininet hosts with wireshark if there is any packet loss. .

![cn_ass3-A](/assignment7/cn_ass7-A.png)
![cn_ass3-A](/assignment7/cn_ass7-B.png)

Assignment 8: Write a program in C using thread library and TCP sockets to build a chat server which enable clients communicating to each other through the chat server. Message logs must be maintained in the server in a text file. Each client will see the conversations in real time. Clients must handled by a server thread. (Keep it like a group chatbox) .

![cn_ass3-A](/assignment8/ass8-A.png)

![cn_ass3-A](/assignment8/ass8-B.png)
![cn_ass3-A](/assignment8/ass8-C.png)

![cn_ass3-A](/assignment8/ass8-D.png)

Assignment 9: Write a client server socket program in TCP for uploading and downloading files between two different hosts. Also calculate the transfer time in both the cases. .

![cn_ass3-A](/assignment9/cn_ass-9A.png)
![cn_ass3-A](/assignment9/cn_ass-9B.png)

Assignment 10: Write two C programs using raw socket to send i. TCP packet where TCP payload will contain your roll number. ii. ICMP time stamp messages towards a target IP . Output (i):

![cn_ass3-A](/assignment10/cn_ass10-A.png)
![cn_ass3-A](/assignment10/cn_ass10-B.png)

Output(ii):

![cn_ass3-A](/assignment10/cn_ass10-C.png)
![cn_ass3-A](/assignment10/cn_ass10-D.png)

Assignment 11: Write a RAW socket program to generate TCP SYN flood based DDoS attack towards an IP address. Take four mininet hosts as agent devices. .

![cn_ass3-A](/assignment11/cn_ass11.png)
![cn_ass3-A](/assignment11/cn_ass11B.png)

Assignment 12: Same as Assignment 10 with ICMP packets using RAW sockets 
![cn_ass3-A](/assignment12/cn_ass12.png)

Assignment 13: Create a binary tree topology with 7 switches in mininet. Capture packets at the root switch. Write a C program to extract the headers and draw a time diagram to show the protocols displayed in the captured file (save the .pcap/.pcapng file of wireshark/tshark) during a PING operation. List the L2, L3, L4 protocols that can be extracted from the .pcap/.pcapng file. .

![cn_ass3-A](/assignment13/cn_ass13-A.png)
![cn_ass3-A](/assignment13/cn_ass13-B.png)

Assignment 14: Create a custom leaf-spine topology in mininet using python which can be scaled with increasing switch radix. .

![cn_ass3-A](/assignment14/cn_ass14.png)
