#include "packet.h"

/* TODO: implement the state machine and the helpers. */

void pkt_init(pkt_parser_t *p)
{
    if (p == NULL)
        return;
    p->state = PKT_STATE_WAIT_START;
    p->done_len = 0;
    p->errors = 0;
    p->index = 0;
}

pkt_result_t pkt_feed(pkt_parser_t *p, uint8_t byte)
{
    if (p == NULL)
        return PKT_NONE;
    switch (p->state) {
    case PKT_STATE_WAIT_START:
        if (byte == PKT_START)
            p->state = PKT_STATE_LEN;
        break;
    case PKT_STATE_LEN:
        if (byte > PKT_MAX_PAYLOAD) {
            p->errors++;
            p->state = PKT_STATE_WAIT_START;
            return PKT_ERR_LENGTH;
        }
        if (byte == 0) {
            p->state = PKT_STATE_CHECKSUM;
        } else
            p->state = PKT_STATE_PAYLOAD;
        p->checksum = byte;
        p->index = 0;
        p->len = byte;
        break;
    case PKT_STATE_PAYLOAD:
        if (p->index < p->len) {
            p->payload[p->index] = byte;
            p->index++;
            p->checksum ^= byte;
            if (p->index == p->len)
                p->state = PKT_STATE_CHECKSUM;
        }
        break;
    case PKT_STATE_CHECKSUM:
        if (p->checksum == byte) {
            for (int i = 0; i < p->len; i++) {
                p->done[i] = p->payload[i];
            }
            p->done_len = p->len;
            p->state = PKT_STATE_WAIT_START;
            return PKT_COMPLETE;
        }
        p->errors++;
        p->state = PKT_STATE_WAIT_START;
        return PKT_ERR_CHECKSUM;
    default:
        break;
    }
    return PKT_NONE;
}

const uint8_t *pkt_payload(const pkt_parser_t *p, uint8_t *len_out)
{
    if (p == NULL || len_out == NULL)
        return NULL;
    *len_out = p->done_len;
    return p->done;
}

uint32_t pkt_error_count(const pkt_parser_t *p)
{
    if (p == NULL)
        return 0;
    return p->errors;
}

size_t pkt_build(uint8_t *out, size_t out_size, const uint8_t *payload, uint8_t len)
{
    if (out == NULL || out_size < (size_t)len + 3u|| (len > 0 && payload == NULL)||len > PKT_MAX_PAYLOAD)
        return 0;
    out[0] = PKT_START;
    out[1] = len;
    uint8_t checksum = len;
    for (int i = 2; i < len + 2; i++) {
        out[i] = payload[i - 2];
        checksum ^= payload[i - 2];
    }
    out[len + 2] = checksum;
    return (size_t)len + 3u;
}
