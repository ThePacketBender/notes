#/bin/sh

ping -b 192.168.1.255
	#ping own broadcast address to populate arp table
arp -n

x=1; while [$x -lt 255]; do ping -t 0.3 -c 2 192.168.1.$x | grep awk '{print $4}' | sort | uniq; let x++; done
	#ping all addresses in network range (for type C) network

nmap -v -p 139,445 -oG smb.txt 10.11.1.1-254

nbtscan -r 10.11.1.0/24
