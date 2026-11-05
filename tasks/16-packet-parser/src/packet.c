
#include "packet.h"

void pkt_init(pkt_parser_t *p)
{
    if (p == NULL) {
        return;
    }

    p->state = PKT_STATE_WAIT_START;
    p->len = 0u;
    p->index = 0u;
    p->checksum = 0u;
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

            p->len = byte;
            p->index = 0u;
            p->checksum = byte;

            if (p->len == 0u) {
                p->state = PKT_STATE_CHECKSUM;
            } else {
                p->state = PKT_STATE_PAYLOAD;
            }
            break;

        case PKT_STATE_PAYLOAD:
            p->payload[p->index] = byte;
            p->checksum = (uint8_t)(p->checksum ^ byte);
            p->index++;

            if (p->index == p->len) {
                p->state = PKT_STATE_CHECKSUM;
            }
            break;

        case PKT_STATE_CHECKSUM:
            p->state = PKT_STATE_WAIT_START;

            if (byte != p->checksum) {
                p->errors++;
                return PKT_ERR_CHECKSUM;
            }

            p->done_len = p->len;

            for (uint8_t i = 0u; i < p->len; i++) {
                p->done[i] = p->payload[i];
            }

            return PKT_COMPLETE;

        default:
            p->state = PKT_STATE_WAIT_START;
            break;
    }

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
        return 0u;
    }

    return p->errors;
}

size_t pkt_build(uint8_t *out, size_t out_size,
                 const uint8_t *payload, uint8_t len)
{
    size_t frame_size = (size_t)len + 3u;
    uint8_t checksum = len;

    if (out == NULL ||
        out_size < frame_size ||
        len > PKT_MAX_PAYLOAD ||
        (payload == NULL && len > 0u)) {
        return 0u;
    }

    out[0] = (uint8_t)PKT_START;
    out[1] = len;

    for (size_t i = 0u; i < (size_t)len; i++) {
        out[i + 2u] = payload[i];
        checksum = (uint8_t)(checksum ^ payload[i]);
    }

    out[frame_size - 1u] = checksum;

    return frame_size;
}
