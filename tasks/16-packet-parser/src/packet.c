#include "packet.h"

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
            break;

        case PKT_STATE_LEN:
            if (byte > PKT_MAX_PAYLOAD) {
                p->errors++;
                p->state = PKT_STATE_WAIT_START;
                return PKT_ERR_LENGTH;
            }

            p->len      = byte;
            p->index    = 0;
            p->checksum = byte;

            if (p->len == 0) {
                p->state = PKT_STATE_CHECKSUM;
            } else {
                p->state = PKT_STATE_PAYLOAD;
            }
            break;

        case PKT_STATE_PAYLOAD:
            p->payload[p->index] = byte;
            p->checksum ^= byte;
            p->index++;

            if (p->index >= p->len) {
                p->state = PKT_STATE_CHECKSUM;
            }
            break;

        case PKT_STATE_CHECKSUM:
            if (byte == p->checksum) {
                p->done_len = p->len;

                for (size_t i = 0; i < p->len; i++) {
                    p->done[i] = p->payload[i];
                }

                p->state = PKT_STATE_WAIT_START;
                return PKT_COMPLETE;
            }

            p->errors++;
            p->state = PKT_STATE_WAIT_START;
            return PKT_ERR_CHECKSUM;

        default:
            p->state = PKT_STATE_WAIT_START;
            break;
    }

    return PKT_NONE;
}

const uint8_t *pkt_payload(const pkt_parser_t *p, uint8_t *len_out)
{
    if ((p == NULL) || (len_out == NULL)) {
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

size_t pkt_build(uint8_t *out, size_t out_size,
                 const uint8_t *payload, uint8_t len)
{
    size_t frame_size;
    size_t i;
    uint8_t checksum;

    if ((out == NULL) ||
        (len > PKT_MAX_PAYLOAD) ||
        ((len > 0) && (payload == NULL))) {
        return 0;
    }

    frame_size = (size_t)len + 3;

    if (out_size < frame_size) {
        return 0;
    }

    checksum = len;

    out[0] = PKT_START;
    out[1] = len;

    for (i = 0; i < len; i++) {
        out[i + 2] = payload[i];
        checksum ^= payload[i];
    }

    out[frame_size - 1] = checksum;

    return frame_size;
}