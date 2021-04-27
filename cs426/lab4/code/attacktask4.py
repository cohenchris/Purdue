#!/usr/bin/python3

from scapy.all import *

# A DNS Question Record:
# DNSQR(
#		qname	-	Domain name
#	   )
Qdsec = DNSQR(qname="www.example.com")

# A DNS query packet:
# DNS  (
#		id		-	hex id to match outstanding queries to responses
#		qr		-	0 = query, 1 = response
#		qdcount	-	16-bit int representing # entries in question section
#		ancount	-	16-bit int representing # records in answer section
#		nscount	-	16-bit int representing # NS records in authority records section
#		arcount	-	16-bit int representing # records in additional records section 
#		qd		-	DNS Question Record
#	   )
dns = DNS(id=0xAAAA, qr=0, qdcount=1, ancount=0, nscount=0, arcount=0, qd=Qdsec)

# An IP packet from 'src' to 'dst':
# IP   (
#		dst		-	destination of packet
#		src		-	source of packet
#	   )
ip = IP(dst="10.0.2.7", src="10.0.2.8")

# A UDP request packet:
# UDP  (
#		dport	-	destination port
#		sport	-	source port
#		chksum	-	checksum of packet
#	   )
udp = UDP(dport=53, sport=33333, chksum=0)

# Craft the request packet
# '/' is a composition operator between two layers
request=ip/udp/dns

# send 1 request
res = sr1(request)

print(res.show())