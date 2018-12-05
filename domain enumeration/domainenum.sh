#/bin/sh

#USAGE: ./dns domain.dom

whois $1

dig $1

nslookup $1

#DNS ZONE TRANSFER
#ns = $(telnet 10.11.1.115 25)
ns = $(dig ns $1 | egrep 'ns.*\.' | sort -u) 
case $(type $ns | cut -f-1) in:
	list)	for f in $ns; do dig "@$f" $1 axfr; done ;;
	string) dig "@$f" $1 axfr	;;
	*)	echo "type error dig zone transfer"; break ;;
esac
#host -t ns $1
#host -l $1 $ns
nmap --script dns-zone-transfer.nse --script-args dns-zone-transfer.domain=$1

#!/bin/bash
# Simple Zone Transfer Bash Script
# $1 is the first argument given after the bash script
# Check if argument was given, if not, print usage
if [ -z "$1" ]; then
echo "[*] Simple Zone transfer script"
echo "[*] Usage : $0 <domain name> "
exit 0
fi
# if argument was given, identify the DNS servers for the domain
for server in $(host -t ns $1 |cut -d" " -f4);do
# For each of these servers, attempt a zone transfer
host -l $1 $server |grep "has address"
done

dnsrecon -d $1 -t axfr

dnsenum $1

