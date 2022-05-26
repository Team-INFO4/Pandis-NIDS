#include "pch.h"

hdr_t PacketAnalyzing(const bit8_t* packet_data)
{
	hdr_t headers;
	headers.eth = (eth_t*)packet_data;
	headers.ip4h = NULL;
	headers.ip6h = NULL;
	headers.arph = NULL;
	headers.icmph = NULL;
	headers.tcph = NULL;
	headers.udph = NULL;

	switch (ntohs(headers.eth->eth_type))
	{
	case 0x0800:
		headers.ip4h = (ipv4h_t*)(packet_data + sizeof(eth_t));
		switch (headers.ip4h->ip4_protocol)
		{
		case 6:		// TCP
			headers.tcph = (tcph_t*)(packet_data + sizeof(eth_t) + headers.ip4h->ip4_hdr_len * 4);
			break;
		case 17:	// UDP
			headers.udph = (udph_t*)(packet_data + sizeof(eth_t) + headers.ip4h->ip4_hdr_len * 4);
			break;
		case 1:		// ICMP
			headers.icmph = (icmph_t*)(packet_data + sizeof(eth_t) + headers.ip4h->ip4_hdr_len * 4);
			break;
		}
		break;

	case 0x86DD:
		headers.ip6h = (ipv6h_t*)(packet_data + sizeof(eth_t));
		switch (headers.ip6h->ip6_next)
		{
		case 6:		// TCP
			headers.tcph = (tcph_t*)(packet_data + sizeof(eth_t) + sizeof(ipv6h_t));
			break;
		case 17:	// UDP
			headers.udph = (udph_t*)(packet_data + sizeof(eth_t) + sizeof(ipv6h_t));
			break;
		case 58:	// ICMPv6
			headers.icmph = (icmph_t*)(packet_data + sizeof(eth_t) + sizeof(ipv6h_t));
			break;
		}
		break;

	case 0x0806:
		headers.arph = (arph_t*)(packet_data + sizeof(eth_t));
		break;
	}

	return headers;
}

int PrintPacketData(hdr_t packet_headers, CString& strPrintString) // TODO : ��� �ϼ�
{
	strPrintString = "/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-/\r\n";

	if (packet_headers.eth)
	{
		strPrintString.AppendFormat(_T("Ethernet Layer\r\n"));
		strPrintString.AppendFormat(_T("Source MAC Address:	%02X:%02X:%02X:%02X:%02X:%02X\r\n"),
			packet_headers.eth->eth_src_mac[0],
			packet_headers.eth->eth_src_mac[1],
			packet_headers.eth->eth_src_mac[2],
			packet_headers.eth->eth_src_mac[3],
			packet_headers.eth->eth_src_mac[4],
			packet_headers.eth->eth_src_mac[5]);

		strPrintString.AppendFormat(_T("Destination MAC Address:	%02X:%02X:%02X:%02X:%02X:%02X\r\n"),
			packet_headers.eth->eth_dst_mac[0],
			packet_headers.eth->eth_dst_mac[1],
			packet_headers.eth->eth_dst_mac[2],
			packet_headers.eth->eth_dst_mac[3],
			packet_headers.eth->eth_dst_mac[4],
			packet_headers.eth->eth_dst_mac[5]);

		strPrintString.AppendFormat(_T("Ethernet Type:		0x%04X\r\n"), ntohs(packet_headers.eth->eth_type));
	}

	if (packet_headers.arph)
	{
		strPrintString.AppendFormat(_T("ARP protocol \r\n"));
		strPrintString.AppendFormat(_T("Source IP Address:  %3D.%3D.%3D.%3D\r\n"), 
			packet_headers.arph->arp_src_ip[0],
			packet_headers.arph->arp_src_ip[1],
			packet_headers.arph->arp_src_ip[2],
			packet_headers.arph->arp_src_ip[3]);
		strPrintString.AppendFormat(_T("->   Source MAC Address:	%02X:%02X:%02X:%02X:%02X:%02X\r\n"),
			packet_headers.arph->arp_src_mac[0],
			packet_headers.arph->arp_src_mac[1],
			packet_headers.arph->arp_src_mac[2],
			packet_headers.arph->arp_src_mac[3],
			packet_headers.arph->arp_src_mac[4],
			packet_headers.arph->arp_src_mac[5]);

		strPrintString.AppendFormat(_T("Destination IP Address:  %3D\.%3D\.%3D\.%3D\r\n"),
			packet_headers.arph->arp_dst_ip[0],
			packet_headers.arph->arp_dst_ip[1],
			packet_headers.arph->arp_dst_ip[2],
			packet_headers.arph->arp_dst_ip[3]);
		strPrintString.AppendFormat(_T("->   Destination MAC Address:	%02X:%02X:%02X:%02X:%02X:%02X\r\n"),
			packet_headers.arph->arp_dst_mac[0],
			packet_headers.arph->arp_dst_mac[1],
			packet_headers.arph->arp_dst_mac[2],
			packet_headers.arph->arp_dst_mac[3],
			packet_headers.arph->arp_dst_mac[4],
			packet_headers.arph->arp_dst_mac[5]);
	}

	if (packet_headers.ip4h)
	{
		strPrintString.AppendFormat(_T("Ipv4 Protocal \r\n"));
		strPrintString.AppendFormat(_T("Source IP Address:   %3D\.%3D\.%3D\.%3D\r\n"),
			packet_headers.ip4h->ip4_src_ip[0],
			packet_headers.ip4h->ip4_src_ip[1],
			packet_headers.ip4h->ip4_src_ip[2],
			packet_headers.ip4h->ip4_src_ip[3]);
		strPrintString.AppendFormat(_T("Destination IP Address:   %3D\.%3D\.%3D\.%3D\r\n"),
			packet_headers.ip4h->ip4_dst_ip[0],
			packet_headers.ip4h->ip4_dst_ip[1],
			packet_headers.ip4h->ip4_dst_ip[2],
			packet_headers.ip4h->ip4_dst_ip[3]);
	}

	if (packet_headers.ip6h)
	{
		strPrintString.AppendFormat(_T("Ipv6 Protocal \r\n"));
		strPrintString.AppendFormat(_T("Source IP Address:   %4x:%4x:%4x:%4x:%4x:%4x:%4x:%4x\r\n"),
			packet_headers.ip6h->ip6_src_ip[0], 
			packet_headers.ip6h->ip6_src_ip[1], 
			packet_headers.ip6h->ip6_src_ip[2], 
			packet_headers.ip6h->ip6_src_ip[3], 
			packet_headers.ip6h->ip6_src_ip[4], 
			packet_headers.ip6h->ip6_src_ip[5], 
			packet_headers.ip6h->ip6_src_ip[6], 
			packet_headers.ip6h->ip6_src_ip[7]);
		strPrintString.AppendFormat(_T("Destination IP Address:   %4x:%4x:%4x:%4x:%4x:%4x:%4x:%4x\r\n"),
			packet_headers.ip6h->ip6_dst_ip[0],
			packet_headers.ip6h->ip6_dst_ip[1],
			packet_headers.ip6h->ip6_dst_ip[2],
			packet_headers.ip6h->ip6_dst_ip[3],
			packet_headers.ip6h->ip6_dst_ip[4],
			packet_headers.ip6h->ip6_dst_ip[5],
			packet_headers.ip6h->ip6_dst_ip[6],
			packet_headers.ip6h->ip6_dst_ip[7]);
	}

	if (packet_headers.tcph)
	{
		strPrintString.AppendFormat(_T("tcp Protocal \r\n"));
		strPrintString.AppendFormat(_T("Source Port:   %5d \r\n"),
			packet_headers.tcph->tcp_src_port);
		strPrintString.AppendFormat(_T("Destination Port:   %5d \r\n"),
			packet_headers.tcph->tcp_dst_port);
	}

	if (packet_headers.udph)
	{
		strPrintString.AppendFormat(_T("udp Protocal \r\n"));
		strPrintString.AppendFormat(_T("Source Port:   %5d \r\n"),
			packet_headers.udph->udp_src_port);
		strPrintString.AppendFormat(_T("Destination Port:   %5d \r\n"),
			packet_headers.udph->udp_dst_port);
	}

	if (packet_headers.icmph)
	{
		strPrintString.AppendFormat(_T("icmp Protocal \r\n"));
		strPrintString.AppendFormat(_T("icmp type:   0x%04X \r\n"),
			ntohs(packet_headers.icmph->icmp_type));
		strPrintString.AppendFormat(_T("icmp code:   0x%04X \r\n"),
			ntohs(packet_headers.icmph->icmp_code));
	}

	strPrintString += "/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-/\r\n";
	return 0;
}