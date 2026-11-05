#include "packet.h"

/* TODO: implement the state machine and the helpers. */

void pkt_init(pkt_parser_t *p)
{
    if (p == NULL) {
        return;
    }

    p->state    = PKT_STATE_WAIT_START;
    p->len      = 0;
    p->index    = 0;
    p->checksum = 0;
    p->done_len = 0;
    p->errors   = 0;
}

pkt_result_t pkt_feed(pkt_parser_t *p, uint8_t byte)
{
    if (p == NULL) {
        return PKT_NONE;
    }

    switch (p->state) {
    case PKT_STATE_WAIT_START:
        if (byte == PKT_START) {
            p->state = PKT_STATE_LEN;
        }
        return PKT_NONE;

    case PKT_STATE_LEN:
        p->len = byte;
        if (p->len > PKT_MAX_PAYLOAD) {
            p->errors++;
            p->state = PKT_STATE_WAIT_START;
            return PKT_ERR_LENGTH;
        }
        p->checksum = p->len;
        p->index    = 0;
        if (p->len == 0) {
            p->state = PKT_STATE_CHECKSUM;
        } else {
            p->state = PKT_STATE_PAYLOAD;
        }
        return PKT_NONE;

    case PKT_STATE_PAYLOAD:
        p->payload[p->index] = byte;
        p->checksum          = (uint8_t)(p->checksum ^ byte);
        p->index++;
        if (p->index == p->len) {
            p->state = PKT_STATE_CHECKSUM;
        }
        return PKT_NONE;

    case PKT_STATE_CHECKSUM:
        p->state = PKT_STATE_WAIT_START;
        if (byte == p->checksum) {
            for (uint8_t i = 0; i < p->len; i++) {
                p->done[i] = p->payload[i];
            }
            p->done_len = p->len;
            return PKT_COMPLETE;
        } else {
            p->errors++;
            return PKT_ERR_CHECKSUM;
        }
    }

    p->state = PKT_STATE_WAIT_START;
    return PKT_NONE;
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
    if (p == NULL) {
        return 0;
    }

    return p->errors;
}

size_t pkt_build(uint8_t *out, size_t out_size, const uint8_t *payload, uint8_t len)
{
    if (out == NULL || len > PKT_MAX_PAYLOAD) {
        return 0;
    }
    if (payload == NULL && len > 0) {
        return 0;
    }

    size_t needed_size = (size_t)len + 3u;
    if (out_size < needed_size) {
        return 0;
    }

    out[0] = PKT_START;
    out[1] = len;
    uint8_t checksum = len;

    for (uint8_t i = 0; i < len; i++) {
        out[2 + i] = payload[i];
        checksum   = (uint8_t)(checksum ^ payload[i]);
    }

    out[2 + len] = checksum;

    return needed_size;
}