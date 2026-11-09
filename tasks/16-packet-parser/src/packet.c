#include "packet.h"

#include <string.h>

void pkt_init(pkt_parser_t *p)
{
    if (!p) return;
    memset(p, 0, sizeof(*p));
}

pkt_result_t pkt_feed(pkt_parser_t *p, uint8_t byte)
{
    if (!p) return PKT_NONE;

    switch (p->state) {
        case PKT_STATE_WAIT_START:
            if (byte == PKT_START)
                p->state = PKT_STATE_LEN;
            return PKT_NONE;

        case PKT_STATE_LEN:
            if (byte > PKT_MAX_PAYLOAD) {
                p->errors++;
                p->state = PKT_STATE_WAIT_START;
                return PKT_ERR_LENGTH;
            }
            p->len      = byte;
            p->index    = 0;
            p->checksum = byte;
            p->state    = (byte == 0) ? PKT_STATE_CHECKSUM : PKT_STATE_PAYLOAD;
            return PKT_NONE;

        case PKT_STATE_PAYLOAD:
            p->payload[p->index++] = byte;
            p->checksum ^= byte;
            if (p->index == p->len)
                p->state = PKT_STATE_CHECKSUM;
            return PKT_NONE;

        case PKT_STATE_CHECKSUM:
            p->state = PKT_STATE_WAIT_START;
            if (byte != p->checksum) {
                p->errors++;
                return PKT_ERR_CHECKSUM;
            }
            p->done_len = p->len;
            memcpy(p->done, p->payload, p->len);
            return PKT_COMPLETE;
    }
    return PKT_NONE;
}

const uint8_t *pkt_payload(const pkt_parser_t *p, uint8_t *len_out)
{
    if (!p || !len_out) return NULL;
    *len_out = p->done_len;
    return p->done;
}

uint32_t pkt_error_count(const pkt_parser_t *p)
{
    if (!p) return 0;
    return p->errors;
}

size_t pkt_build(uint8_t *out, size_t out_size, const uint8_t *payload, uint8_t len)
{
    if (!out) return 0;
    if (len > PKT_MAX_PAYLOAD) return 0;
    if (payload == NULL && len > 0) return 0;
    size_t frame_size = (size_t)len + 3u;
    if (out_size < frame_size) return 0;

    uint8_t checksum = len;
    out[0] = PKT_START;
    out[1] = len;
    for (uint8_t i = 0; i < len; i++) {
        out[2 + i] = payload[i];
        checksum ^= payload[i];
    }
    out[2 + len] = checksum;
    return frame_size;
}
