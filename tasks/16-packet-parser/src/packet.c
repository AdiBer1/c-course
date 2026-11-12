#include "packet.h"

/* Go back to waiting for a start byte (the frame being received is dropped,
 * but the last COMPLETE payload in done[] is kept). */
static void restart(pkt_parser_t *p)
{
    p->state = PKT_STATE_WAIT_START;
    p->len = 0u;
    p->index = 0u;
    p->checksum = 0u;
}

void pkt_init(pkt_parser_t *p)
{
    if (p == NULL) {
        return;
    }
    restart(p);
    p->done_len = 0u;
    p->errors = 0u;
}

pkt_result_t pkt_feed(pkt_parser_t *p, uint8_t byte)
{
    if (p == NULL) {
        return PKT_NONE;
    }

    switch (p->state) {
    case PKT_STATE_WAIT_START:
        /* Anything other than the start byte is ignored: this is how the
         * parser resynchronises after garbage or lost bytes. */
        if (byte == PKT_START) {
            p->state = PKT_STATE_LEN;
        }
        return PKT_NONE;

    case PKT_STATE_LEN:
        /* Never trust a length from the wire: check it BEFORE any payload
         * byte is stored, so payload[] can never be overrun. */
        if (byte > PKT_MAX_PAYLOAD) {
            p->errors++;
            restart(p);
            return PKT_ERR_LENGTH;
        }
        p->len = byte;
        p->index = 0u;
        p->checksum = byte; /* the checksum covers LEN too */
        p->state = (byte == 0u) ? PKT_STATE_CHECKSUM : PKT_STATE_PAYLOAD;
        return PKT_NONE;

    case PKT_STATE_PAYLOAD:
        /* 0x7E here is just data: the length says where the payload ends. */
        p->payload[p->index] = byte;
        p->index++;
        p->checksum ^= byte;
        if (p->index == p->len) {
            p->state = PKT_STATE_CHECKSUM;
        }
        return PKT_NONE;

    case PKT_STATE_CHECKSUM:
        if (byte != p->checksum) {
            p->errors++;
            restart(p);
            return PKT_ERR_CHECKSUM;
        }
        /* Valid frame: copy it to done[], which only changes here, so the
         * caller's last good payload survives later broken frames. */
        for (uint8_t i = 0u; i < p->len; i++) {
            p->done[i] = p->payload[i];
        }
        p->done_len = p->len;
        restart(p);
        return PKT_COMPLETE;

    default:
        /* Corrupted state value: recover safely. */
        restart(p);
        return PKT_NONE;
    }
}

const uint8_t *pkt_payload(const pkt_parser_t *p, uint8_t *len_out)
{
    if (p == NULL || len_out == NULL) {
        return NULL;
    }
    *len_out = p->done_len;
    return p->done;
}

uint32_t pkt_error_count(const pkt_parser_t *p)
{
    return (p == NULL) ? 0u : p->errors;
}

size_t pkt_build(uint8_t *out, size_t out_size, const uint8_t *payload, uint8_t len)
{
    size_t frame_size = (size_t)len + 3u; /* START + LEN + payload + CHECKSUM */

    if (out == NULL || len > PKT_MAX_PAYLOAD || (payload == NULL && len > 0u) ||
        out_size < frame_size) {
        return 0u;
    }

    uint8_t checksum = len;
    out[0] = PKT_START;
    out[1] = len;
    for (uint8_t i = 0u; i < len; i++) {
        out[2u + i] = payload[i];
        checksum ^= payload[i];
    }
    out[2u + len] = checksum;
    return frame_size;
}