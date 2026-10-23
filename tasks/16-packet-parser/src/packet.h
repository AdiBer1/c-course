/*
 * packet.h - receive framed packets one byte at a time.
 *
 * Serial links deliver a stream of bytes with no boundaries. A protocol adds
 * framing so the receiver knows where a message starts and ends and whether
 * it arrived intact. This one is typical of sensor and motor-controller
 * protocols:
 *
 *     +------+-----+----------------------+----------+
 *     | 0x7E | LEN | PAYLOAD (LEN bytes)  | CHECKSUM |
 *     +------+-----+----------------------+----------+
 *
 *   START     always 0x7E
 *   LEN       number of payload bytes, 0..PKT_MAX_PAYLOAD
 *   PAYLOAD   LEN bytes of any value (0x7E inside the payload is just data)
 *   CHECKSUM  XOR of LEN and every payload byte
 *
 * The parser is a state machine fed one byte at a time, exactly as a UART
 * interrupt or a polling loop would deliver them:
 *
 *   WAIT_START --0x7E--> LEN --LEN ok--> PAYLOAD --last byte--> CHECKSUM
 *        ^                |  (LEN==0 goes straight to CHECKSUM)      |
 *        +----------------+---- error or complete -------------------+
 *
 *   - bytes other than 0x7E while waiting for a start are ignored
 *   - LEN > PKT_MAX_PAYLOAD -> PKT_ERR_LENGTH, back to WAIT_START
 *   - wrong checksum         -> PKT_ERR_CHECKSUM, back to WAIT_START
 *   - right checksum         -> PKT_COMPLETE, back to WAIT_START
 *
 * Do not change this file.
 */
#ifndef PACKET_H
#define PACKET_H

#include <stddef.h>
#include <stdint.h>

#define PKT_START       0x7Eu
#define PKT_MAX_PAYLOAD 32u

typedef enum {
    PKT_STATE_WAIT_START = 0,
    PKT_STATE_LEN,
    PKT_STATE_PAYLOAD,
    PKT_STATE_CHECKSUM
} pkt_state_t;

typedef enum {
    PKT_NONE = 0,      /* byte consumed, no packet finished yet */
    PKT_COMPLETE,      /* a valid packet was just received */
    PKT_ERR_LENGTH,    /* LEN was too big; frame dropped */
    PKT_ERR_CHECKSUM   /* checksum mismatch; frame dropped */
} pkt_result_t;

typedef struct {
    pkt_state_t state;
    uint8_t len;                       /* LEN of the frame being received */
    uint8_t index;                     /* payload bytes received so far */
    uint8_t checksum;                  /* running XOR */
    uint8_t payload[PKT_MAX_PAYLOAD];  /* payload of the frame being received */
    uint8_t done_len;                  /* length of the last COMPLETE payload */
    uint8_t done[PKT_MAX_PAYLOAD];     /* copy of the last COMPLETE payload */
    uint32_t errors;                   /* number of ERR_LENGTH + ERR_CHECKSUM so far */
} pkt_parser_t;

/* Reset to WAIT_START, no completed packet, error counter 0. NULL: nothing. */
void pkt_init(pkt_parser_t *p);

/* Feed one received byte. NULL -> PKT_NONE. */
pkt_result_t pkt_feed(pkt_parser_t *p, uint8_t byte);

/*
 * The payload of the last COMPLETE packet; its length in *len_out.
 * Stays valid (and unchanged) until the next PKT_COMPLETE.
 * Before any packet completed: returns a valid pointer and *len_out = 0.
 * NULL p or len_out -> returns NULL.
 */
const uint8_t *pkt_payload(const pkt_parser_t *p, uint8_t *len_out);

/* Number of dropped frames so far. NULL -> 0. */
uint32_t pkt_error_count(const pkt_parser_t *p);

/*
 * Build a frame for `payload` (len bytes) into out. Returns the frame size
 * (len + 3), or 0 if out is NULL, out_size is too small, len > PKT_MAX_PAYLOAD,
 * or payload is NULL while len > 0.
 */
size_t pkt_build(uint8_t *out, size_t out_size, const uint8_t *payload, uint8_t len);

#endif /* PACKET_H */
