#include "homa_flow.h"

#include <cstdio>

#include "../coresim/channel.h"
#include "../coresim/event.h"
#include "../coresim/packet.h"
#include "../coresim/queue.h"
#include "../run/params.h"

extern double get_current_time();
extern void add_to_event_queue(Event *);
extern DCExpParams params;

HomaFlow::HomaFlow(uint32_t id, double start_time, uint32_t size, Host *s, Host *d,
    uint32_t flow_priority) : Flow(id, start_time, size, s, d, flow_priority) {}

void HomaFlow::start_flow() {
    run_priority = flow_priority;
    channel->add_to_channel(this);  // so we can do SRPT
}

void HomaFlow::receive_data_pkt(Packet* p) {
    p->flow->channel->receive_data_pkt(p);
}

void HomaFlow::receive_ack(uint64_t ack, std::vector<uint64_t> sack_list, double pkt_start_ts, uint32_t priority, uint32_t num_hops) {
    channel->receive_ack(ack, this, sack_list, pkt_start_ts);
}
