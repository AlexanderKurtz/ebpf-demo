#include <bcc/helpers.h>
#include <bcc/proto.h>

int filter(struct __sk_buff *skb) {
	/* We are testing this over the loopback interface which uses raw UDP
	   packets without encapsulating them into Ethernet frames or IP
	   packets, so the offset of the UDP length field is simply 4.
	 */
	unsigned short length = load_half (skb, 4);

	if (length == 8) {
		// Drop this empty packet.
		return 0;
	} else {
		// Forward this packet userspace, do not modify.
		return -1;
	}
}
